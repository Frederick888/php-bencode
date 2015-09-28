#include "BItem.h"
#include "BStr.h"
#include "BInt.h"
#include "BDict.h"
#include "BList.h"

/**
 * BItem implements
 */
Php::Value BItem::getType() const {
    return "BItem";
}

Php::Value BItem::get(Php::Parameters &params) const {
    if (getType() == "BItem" || getType() == "BStr" || getType() == "BInt")
        return (Php::Value)nullptr;

    std::string key = params[0];

    BItem *found = getPath(key);
    if (found == nullptr) {
        return (Php::Value)nullptr;
    } else {
        return Php::Object(found->getType(), found->clone());
    }
}

BItem* BItem::getPath(const std::string &key) const {
    return nullptr;
}

std::map<std::string, BItem*> BItem::getDataD() const {
    std::map<std::string, BItem*> dummy;
    return dummy;
}

std::vector<BItem*> BItem::getDataL() const {
    std::vector<BItem*> dummy;
    return dummy;
}

std::string BItem::getDataS() const {
    std::string dummy;
    return dummy;
}

int64_t BItem::getDataI() const {
    int64_t dummy = std::numeric_limits<int64_t>::min();
    return dummy;
}

std::string BItem::trimKey(std::string key) const {
    while (key[0] == '/') {
        key = key.substr(1);
    }
    while (key[key.length() - 1] == '/' && key[key.length() - 2] != '\\') {
        key.pop_back();
    }
    return key;
}

std::string BItem::escapeKey(std::string key) const {
    for (size_t i = 0; i < key.length(); i++) {
        if (key[i] == '/') key.replace(i++, 1, "\\/");
    }
    return key;
}

std::string BItem::parseKey(std::string key) const {
    for (size_t i = 0; i < key.length(); i++) {
        if (key.substr(i, 2) == "\\/") key.replace(i, 2, "/");
    }
    return key;
}

std::string BItem::splitKey(std::string &key) const {
    // Find the first slash which is not escaped
    size_t pos = key.find('/');
    while (pos < key.length() && key[pos - 1] == '\\') {
        if (pos == key.length() - 1) {
            break;
        } else {
            pos = key.find('/', pos + 1);
        }
    }
    // Get key for searching this time and edit key itself
    std::string searchKey;
    if (pos >= key.length() - 1) {
        searchKey = key;
        key = "";
    } else {
        searchKey = key.substr(0, pos);
        key = key.substr(pos + 1);
    }
    searchKey = parseKey(searchKey);

    return searchKey;
}

bool BItem::isSizet(const std::string &intstr) const {
    for (size_t i = 0; i < intstr.length(); ++i) {
        if (intstr[i] < '0' || intstr[i] > '9') {
            return false;
        }
    }
    return true;
}

Php::Value BItem::parse(Php::Parameters &params) {
    std::string ben = params[0];
    size_t pt = 0;
    if (ben[0] == 'd') {
        return Php::Object("BDict", BDict::parseD(ben, pt));
    } else if (ben[0] == 'l') {
        return Php::Object("BList", BList::parseL(ben, pt));
    } else if (ben[0] >= '0' && ben[0] <= '9') {
        return Php::Object("BStr", BStr::parseS(ben, pt));
    } else if (ben[0] == 'i') {
        return Php::Object("BInt", BInt::parseI(ben, pt));
    } else {
        throw Php::Exception("Error parsing: " + ben[0]);
    }
}

Php::Value BItem::load(Php::Parameters &params) {
    std::string file = params[0];
    
    std::ifstream benFile(file);
    if (!benFile.is_open()) throw Php::Exception("Error opening file");

    std::string ben((std::istreambuf_iterator<char>(benFile)),
                    (std::istreambuf_iterator<char>()));
    benFile.close();

    size_t pt = 0;
    if (ben[0] == 'd') {
        return Php::Object("BDict", BDict::parseD(ben, pt));
    } else if (ben[0] == 'l') {
        return Php::Object("BList", BList::parseL(ben, pt));
    } else if (ben[0] >= '0' && ben[0] <= '9') {
        return Php::Object("BStr", BStr::parseS(ben, pt));
    } else if (ben[0] == 'i') {
        return Php::Object("BInt", BInt::parseI(ben, pt));
    } else {
        throw Php::Exception("Error parsing: " + ben[0]);
    }
}

void BItem::save(Php::Parameters &params) const {
    std::string file = params[0];

    std::ofstream benFile(file);
    if (!benFile.is_open()) throw Php::Exception("Error opening file");
    benFile << this->__toString().stringValue();
    benFile.close();
}

Php::Value BItem::search(Php::Parameters &params) const {
    std::string needle = params[0];
    std::string mode = params[1];
    if (mode[0] != 's' && mode[0] != 'i' && mode[0] != 'k') {
        throw Php::Exception("Unsupported mode");
    }
    std::vector<std::string> pathStack;
    std::vector<std::string> result;
    csearch(needle, mode[0], pathStack, result);
    Php::Value presult; presult[0] = ""; presult.unset(0);
    for (size_t i = 0; i < result.size(); i++) presult[i] = result[i];
    return presult;
}
