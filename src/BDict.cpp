#include "BItem.h"
#include "BStr.h"
#include "BInt.h"
#include "BDict.h"
#include "BList.h"

/**
 * BDict implements
 */
Php::Value BDict::getType() const {
    return "BDict";
}

BDict* BDict::clone() const {
    BDict* clone = new BDict(this);
    return clone;
}

std::map<std::string, BItem*> BDict::getDataD() const {
    return BData;
}

BItem* BDict::getPath(const std::string &key) const {
    std::string path = trimKey(key);
    std::string field = splitKey(path);

    if (path == "") {
        auto search = BData.find(field);
        if(search != BData.end()) {
            BItem *found = search->second;
            return found->me();
        } else {
            return nullptr;
        }
    }

    auto search = BData.find(field);
    if(search != BData.end()) {
        BItem *found = search->second;
        return found->getPath(path);
    } else {
        return nullptr;
    }
}

Php::Value BDict::has(Php::Parameters &params) const {
    std::string key = params[0];
    if (getPath(key) == nullptr) {
        return false;
    } else {
        return true;
    }
}

bool BDict::chas(const std::string &key) const {
    if (getPath(key) == nullptr) {
        return false;
    } else {
        return true;
    }
}

void BDict::set(Php::Parameters &params) {
    std::string key = params[0];

    Php::Value item = params[1];
    if (!(item.instanceOf("BDict") || item.instanceOf("BList") ||
            item.instanceOf("BStr") || item.instanceOf("BInt"))) {
        throw Php::Exception("Error adding to BDict");
    }
    BItem *cppItem = (BItem*)item.implementation();
    if (cppItem->getType().stringValue() == "BDict") {
        BDict *cppItem1 = (BDict*)item.implementation();
        //BDict *cppItemCpy = new BDict(*cppItem1);
        setPath(key, cppItem1);
    } else if (cppItem->getType().stringValue() == "BList") {
        BList *cppItem1 = (BList*)item.implementation();
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
void BDict::setPath(const std::string &key, T *BItem) {
    std::string path = trimKey(key);
    // according to https://en.wikipedia.org/wiki/Bencode
    // all keys must be byte strings
    // so field is treated as a string even it contains only numbers
    std::string field = splitKey(path);

    // all parents are ensured, set the item
    if (path == "") {
        BData.erase(field);
        if (BItem == nullptr)
            return;
        // make a copy explicitly
        T *item = new T(*BItem);
        BData.insert({field, item});
        return;
    }

    // path not completely consumed, check the existance of the next level
    if (BData.count(field) == 0) {
        // fetch the next field to determin which type to insert
        // by the way, negative numbers are treated as string here
        // you can pre-create the parents explicitly at PHP level to avoid
        //      the auto-decision here
        std::string path_n = path;
        std::string field_n = splitKey(path_n);
        if (isSizet(field_n)) {
            BList *newList = new BList();
            newList->setPath(path, BItem);
            BData.insert({field, newList});
        } else {
            BDict *newDict = new BDict();
            newDict->setPath(path, BItem);
            BData.insert({field, newDict});
        }
    } else {
        std::string subLevel = BData[field]->getType();
        if (subLevel == "BList") {
            BList *sub = new BList(BData[field]);
            sub->setPath(path, BItem);
            BData.erase(field);
            BData.insert({field, sub});
        } else if (subLevel == "BDict") {
            BDict *sub = new BDict(BData[field]);
            sub->setPath(path, BItem);
            BData.erase(field);
            BData.insert({field, sub});
        } else {
            throw Php::Exception(subLevel + " cannot own childs");
        }
    }
}

Php::Value BDict::del(Php::Parameters &params) {
    std::string key = params[0];
    if (!chas(key))
        return false;
    setPath(key, (BStr*)nullptr);
    return true;
}

Php::Value BDict::getKeys() const {
    Php::Value retval;
    size_t index = 0;
    auto iter = BData.begin();
    while (iter != BData.end()) {
        retval[index] = iter->first;
        ++index; ++iter;
    }
    return retval;
}

Php::Value BDict::length() const {
    return (int64_t)(__toString().stringValue().length());
}

Php::Value BDict::size() const {
    return (int64_t)BData.size();
}

BDict* BDict::parseD(const std::string &ben, size_t &pt) {
    if (ben[pt] != 'd')
        throw Php::Exception("Error parsing BDict");
    ++pt;
    BDict *retval = new BDict();
    while (ben[pt] != 'e') {
        size_t start = pt;
        while (ben[pt] >= '0' && ben[pt] <= '9') ++pt;
        std::string len = ben.substr(start, pt - start);
        ++pt;
        std::string key = ben.substr(pt, std::stoull(len));
        pt += std::stoull(len);
        if (ben[pt] == 'd') {
            retval->BData.insert({key, BDict::parseD(ben, pt)});
        } else if (ben[pt] == 'l') {
            retval->BData.insert({key, BList::parseL(ben, pt)});
        } else if (ben[pt] >= '0' && ben[pt] <= '9') {
            retval->BData.insert({key, BStr::parseS(ben, pt)});
        } else if (ben[pt] == 'i') {
            retval->BData.insert({key, BInt::parseI(ben, pt)});
        } else throw Php::Exception("Error parsing BDict");
    }
    ++pt;
    return retval;
}

Php::Value BDict::toArray() const {
    Php::Value retval;
    auto iter = BData.begin();
    while (iter != BData.end()) {
        std::string key = iter->first;
        std::string type = iter->second->getType();
        if (type == "BDict") {
            BDict *current = new BDict(iter->second);
            retval[key] = current->toArray();
        } else if (type == "BList") {
            BList *current = new BList(iter->second);
            retval[key] = current->toArray();
        } else if (type == "BStr") {
            BStr *current = new BStr(iter->second);
            retval[key] = current->toArray();
        } else if (type == "BInt") {
            BInt *current = new BInt(iter->second);
            retval[key] = current->toArray();
        }
        ++iter;
    }
    return retval;
}

Php::Value BDict::toMetaArray() const {
    Php::Value retval;
    retval["_type"] = "BDict";
    retval["_length"] = length();
    retval["_size"] = size();
    auto iter = BData.begin();
    while (iter != BData.end()) {
        std::string key = iter->first;
        std::string type = iter->second->getType();
        if (type == "BDict") {
            BDict *current = new BDict(iter->second);
            retval["_data"][key] = current->toMetaArray();
        } else if (type == "BList") {
            BList *current = new BList(iter->second);
            retval["_data"][key] = current->toMetaArray();
        } else if (type == "BStr") {
            BStr *current = new BStr(iter->second);
            retval["_data"][key] = current->toMetaArray();
        } else if (type == "BInt") {
            BInt *current = new BInt(iter->second);
            retval["_data"][key] = current->toMetaArray();
        }
        ++iter;
    }
    return retval;
}

void BDict::csearch(const std::string &needle, const char &mode,
                    std::vector<std::string> &pathStack, std::vector<std::string> &result) const {
    auto iter = BData.begin();
    while (iter != BData.end()) {
        pathStack.push_back(escapeKey(iter->first) + "/");
        if (mode == 'k' && iter->first.find(needle) < iter->first.length()) {
            std::string path;
            for (size_t i = 0; i < pathStack.size(); i++)
                path += pathStack[i];
            result.push_back(trimKey(path));
        }
        if (iter->second->getType() == "BDict") {
            BDict *current = new BDict(iter->second);
            current->csearch(needle, mode, pathStack, result);
        } else if (iter->second->getType() == "BList") {
            BList *current = new BList(iter->second);
            current->csearch(needle, mode, pathStack, result);
        } else if (iter->second->getType() == "BStr") {
            BStr *current = new BStr(iter->second);
            current->csearch(needle, mode, pathStack, result);
        } else if (iter->second->getType() == "BInt") {
            BInt *current = new BInt(iter->second);
            current->csearch(needle, mode, pathStack, result);
        }
        pathStack.pop_back();
        ++iter;
    }
}

Php::Value BDict::__toString() const {
    std::string retval = "d";
    auto iter = BData.begin();
    while (iter != BData.end()) {
        BStr key(iter->first);
        retval += key.__toString().stringValue();
        if (iter->second->getType() == "BDict") {
            BDict *current = new BDict(iter->second);
            retval += current->__toString().stringValue();
        } else if (iter->second->getType() == "BList") {
            BList *current = new BList(iter->second);
            retval += current->__toString().stringValue();
        } else if (iter->second->getType() == "BStr") {
            BStr *current = new BStr(iter->second);
            retval += current->__toString().stringValue();
        } else if (iter->second->getType() == "BInt") {
            BInt *current = new BInt(iter->second);
            retval += current->__toString().stringValue();
        }
        ++iter;
    }
    retval += "e";
    return retval;
}

void BDict::__construct() {
    BData.clear();
}

void BDict::__destruct() {
    BData.clear();
}
