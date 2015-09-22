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

Php::Value BItem::get() const {
    return (Php::Value)nullptr;
}

std::map<std::string, BItem*> BItem::getDataD() const {
    std::map<std::string, BItem*> dummy;
    return dummy;
}

std::vector<BItem*> BItem::getDataL() const {
    std::vector<BItem*> dummy;
    return dummy;
}

std::string BItem::trimKey(const std::string &key) const {
    std::string retval = key;
    while (retval[0] == '/') {
        retval = retval.substr(1);
    }
    while (retval[retval.length() - 1] == '/' && retval[retval.length() - 2] != '\\') {
        retval = retval.substr(0, retval.length() - 2);
    }
    return retval;
}

std::string BItem::splitKey(std::string &key) const {
    // Find the first slash which is not escaped
    int pos = key.find('/');
    if (pos > 0) {
        while (key[pos - 1] == '\\') {
            if ((size_t)pos == key.length() - 1) {
                break;
            } else {
                pos = key.find('/', pos + 1);
                if (pos < 0) {
                    pos = key.length() - 1;
                    break;
                }
            }
        }
    }
    // Get key for searching this time and edit key itself
    std::string searchKey;
    if (pos < 0 || (size_t)pos == key.length() - 1) {
        searchKey = key;
        key = "";
    } else {
        searchKey = key.substr(0, pos);
        key = key.substr(pos + 1);
    }
    while (int i = searchKey.find("\\/")) {
        if (i < 0) break;
        searchKey.replace(i, 2, "/");
    }

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

Php::Value BItem::__toString() const {
    return "this is a BItem";
}
