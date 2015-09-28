#include "BItem.h"
#include "BStr.h"
#include "BInt.h"
#include "BDict.h"
#include "BList.h"

/**
 * BList implements
 */
Php::Value BList::getType() const {
    return "BList";
}

std::vector<BItem*> BList::getDataL() const {
    return BData;
}

BItem* BList::getPath(const std::string &key) const {
    std::string path = trimKey(key);
    std::string field = splitKey(path);

    if (!isSizet(field))
        return nullptr;
    size_t ifield = std::stoull(field);
    if (ifield >= BData.size())
        return nullptr;

    if (path == "") {
        BItem *found = BData[ifield];
        return found->me();
    }

    BItem *found = BData[ifield];
    return found->me();
}

Php::Value BList::has(Php::Parameters &params) const {
    std::string key = params[0];
    if (getPath(key) == nullptr) {
        return false;
    } else {
        return true;
    }
}

bool BList::chas(const std::string &key) const {
    if (getPath(key) == nullptr) {
        return false;
    } else {
        return true;
    }
}

void BList::set(Php::Parameters &params) {
    std::string key = params[0];
    
    Php::Value item = params[1];
    if (!(item.instanceOf("BDict") || item.instanceOf("BList") ||
            item.instanceOf("BStr") || item.instanceOf("BInt"))) {
        throw Php::Exception("Error adding to BList");
    }
    BItem *cppItem = (BItem*)item.implementation();
    if (cppItem->getType().stringValue() == "BDict") {
        BDict *cppItem1 = (BDict*)item.implementation();
        //BDict *cppItemCpy = new BDict(*cppItem1);
        setPath(key, cppItem1);
    } else if (cppItem->getType().stringValue() == "BList") {
        BList *cppItem1 =  (BList*)item.implementation();
        setPath(key, cppItem1);
    } else if (cppItem->getType().stringValue() == "BStr") {
        BStr *cppItem1 = (BStr*)item.implementation();
        setPath(key, cppItem1);
    } else if (cppItem->getType().stringValue() == "BInt") {
        BInt *cppItem1 = (BInt*)item.implementation();
        setPath(key, cppItem1);
    }
}

template<typename T>
void BList::setPath(const std::string &key, T *BItem) {
    std::string path = trimKey(key);
    std::string field = splitKey(path);
    if (!isSizet(field)) {
        throw Php::Exception("BList only accept unsigned integers as keys");
    }
    size_t ifield = std::stoull(field);

    // all parents are ensured, set the item
    if (path == "") {
        if (BItem == nullptr) {
            BData.erase(BData.begin() + ifield);
            return;
        }
        // make a copy explicitly
        T *item = new T(*BItem);
        if (ifield < BData.size()) {
            BData[ifield] = item;
        } else if (ifield >= BData.size()) {
            BData.push_back(item);
        }
        return;
    }

    // path not completely consumed, check the existance of the next level
    if (ifield > BData.size()) {
        throw Php::Exception("the key is too large for the current BList");
    }
    if (ifield == BData.size()) {
        std::string path_n = path;
        std::string field_n = splitKey(path_n);
        if (isSizet(field_n)) {
            BList *newList = new BList();
            newList->setPath(path, BItem);
            BData.push_back(newList);
        } else {
            BDict *newDict = new BDict();
            newDict->setPath(path, BItem);
            BData.push_back(newDict);
        }
    } else {
        std::string subLevel = BData[ifield]->getType();
        if (subLevel == "BList") {
            BList *sub = new BList(BData[ifield]);
            sub->setPath(path, BItem);
            BData.erase(BData.begin() + ifield);
            BData.insert(BData.begin() + ifield, sub);
        } else if (subLevel == "BDict") {
            BDict *sub = new BDict(BData[ifield]);
            sub->setPath(path, BItem);
            BData.erase(BData.begin() + ifield);
            BData.insert(BData.begin() + ifield, sub);
        } else {
            throw Php::Exception(subLevel + " cannot own childs");
        }
    }
}

Php::Value BList::del(Php::Parameters &params) {
    std::string key =  params[0];
    if (!chas(key))
        return false;
    setPath(key, (BStr*)nullptr);
    return true;
}

Php::Value BList::length() const {
    return (int64_t)(__toString().stringValue().length());
}

Php::Value BList::size() const {
    return (int64_t)BData.size();
}

BList* BList::parseL(const std::string &ben, size_t &pt) {
    if (ben[pt] != 'l')
        throw Php::Exception("Error parsing BList");
    ++pt;
    BList *retval = new BList();
    while (ben[pt] != 'e') {
        if (ben[pt] == 'd') {
            retval->BData.push_back(BDict::parseD(ben, pt));
        } else if (ben[pt] == 'l') {
            retval->BData.push_back(BList::parseL(ben, pt));
        } else if (ben[pt] >= '0' && ben[pt] <= '9') {
            retval->BData.push_back(BStr::parseS(ben, pt));
        } else if (ben[pt] == 'i') {
            retval->BData.push_back(BInt::parseI(ben, pt));
        } else throw Php::Exception("Error parsing BDict");
    }
    ++pt;
    return retval;
}

Php::Value BList::toArray() const {
    Php::Value retval;
    for (size_t i = 0; i < BData.size(); i++) {
        std::string type = BData[i]->getType();
        if (type == "BDict") {
            BDict *current = new BDict(BData[i]);
            retval[i] = current->toArray();
        } else if (type == "BList") {
            BList *current = new BList(BData[i]);
            retval[i] = current->toArray();
        } else if (type == "BStr") {
            BStr *current = new BStr(BData[i]);
            retval[i] = current->toArray();
        } else if (type == "BInt") {
            BInt *current = new BInt(BData[i]);
            retval[i] = current->toArray();
        }
    }
    return retval;
}

Php::Value BList::toMetaArray() const {
    Php::Value retval;
    retval["_type"] = "BList";
    retval["_length"] = length();
    retval["_size"] = size();
    for (size_t i = 0; i < BData.size(); i++) {
        std::string type = BData[i]->getType();
        if (type == "BDict") {
            BDict *current = new BDict(BData[i]);
            retval["_data"][i] = current->toMetaArray();
        } else if (type == "BList") {
            BList *current = new BList(BData[i]);
            retval["_data"][i] = current->toMetaArray();
        } else if (type == "BStr") {
            BStr *current = new BStr(BData[i]);
            retval["_data"][i] = current->toMetaArray();
        } else if (type == "BInt") {
            BInt *current = new BInt(BData[i]);
            retval["_data"][i] = current->toMetaArray();
        }
    }
    return retval;
}

void BList::csearch(const std::string &needle, const char &mode,
                    std::vector<std::string> &pathStack, std::vector<std::string> &result) const {
    for (size_t i = 0; i < BData.size(); i++) {
        pathStack.push_back(numtos(i) + "/");
        if (BData[i]->getType() == "BDict") {
            BDict *current = new BDict(BData[i]);
            current->csearch(needle, mode, pathStack, result);
        } else if (BData[i]->getType() == "BList") {
            BList *current = new BList(BData[i]);
            current->csearch(needle, mode, pathStack, result);
        } else if (BData[i]->getType() == "BStr") {
            BStr *current = new BStr(BData[i]);
            current->csearch(needle, mode, pathStack, result);
        } else if (BData[i]->getType() == "BInt") {
            BInt *current = new BInt(BData[i]);
            current->csearch(needle, mode, pathStack, result);
        }
        pathStack.pop_back();
    }
}

Php::Value BList::__toString() const {
    std::string retval = "l";
    for (size_t i = 0; i < BData.size(); i++) {
        if (BData[i]->getType() == "BDict") {
            BDict *current = new BDict(BData[i]);
            retval += current->__toString().stringValue();
        } else if (BData[i]->getType() == "BList") {
            BList *current = new BList(BData[i]);
            retval += current->__toString().stringValue();
        } else if (BData[i]->getType() == "BStr") {
            BStr *current = new BStr(BData[i]);
            retval += current->__toString().stringValue();
        } else if (BData[i]->getType() == "BInt") {
            BInt *current = new BInt(BData[i]);
            retval += current->__toString().stringValue();
        }
    }
    retval += "e";
    return retval;
}

void BList::__construct() {
    BData.clear();
}

void BList::__destruct() {
    BData.clear();
}
