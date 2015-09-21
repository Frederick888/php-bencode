#include <phpcpp.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

class BItem : public Php::Base {
    protected:
        template<typename T>
        std::string numtos(const T &value) const;

        std::string trimKey(const std::string &key) const;

        std::string splitKey(std::string &key) const;

        bool isSizet(const std::string &intstr) const;

    public:
        /**
         * C++ constructor and destructor
         */
        BItem() {}
        BItem(const BItem &that) {}
        virtual ~BItem() {}

        /**
         * Regular functions
         */
        virtual Php::Value getType() const {
            return "BItem";
        }

        virtual Php::Value get() const {
            return (Php::Value)nullptr;
        }

        virtual std::map<std::string, BItem*> getDataD() const {
            std::map<std::string, BItem*> dummy;
            return dummy;
        }

        virtual std::vector<BItem*> getDataL() const {
            std::vector<BItem*> dummy;
            return dummy;
        }

        static Php::Value parse(Php::Parameters &params);

        static Php::Value load(Php::Parameters &params);

        /**
         * Magic methods
         */
        virtual Php::Value __toString() {
            return "this is a BItem";
        }
};

class BStr : public BItem {
    public:
        std::string _value;

        /**
         * C++ constructor and destructor
         */
        BStr() : BItem() {}
        
        BStr(std::string value) :
            BItem(), _value( value ) {}

        BStr(const BStr &that) :
            BItem(), _value( that._value ) {}

        BStr(const BStr *that) :
            BItem(), _value( that->_value ) {}

        BStr(const BItem *that) {
            if (that->getType().stringValue() != "BStr")
                throw Php::Exception("Error converting BItem to BStr, the original type is " + that->getType());
            _value = that->get().stringValue();
        }

        virtual ~BStr() {}

        /**
         * Regular functions
         */
        Php::Value getType() const {
            return "BStr";
        }

        Php::Value get() const {
            return _value;
        }

        void set(Php::Parameters &params) {
            std::string value = params[0];
            _value = value;
        }

        Php::Value length() const {
            return (int64_t)(__toString().stringValue().length());
        }

        static BStr* parseS(const std::string &ben, size_t &pt);

        /**
         * Magic methods
         */
        Php::Value __toString() const {
            return numtos(_value.size()) + ":" + _value;
        }

        virtual void __construct(Php::Parameters &params) {
            std::string value = params[0];
            _value = value;
        }

        /**
         * Operators
         */
        BStr &operator=(const std::string &that) { _value = that; return *this; }

        bool operator==(const BStr &that) { return _value == that._value; }
        bool operator!=(const BStr &that) { return !operator==(that); }
        bool operator<=(const BStr &that) { return _value <= that._value; }
        bool operator>=(const BStr &that) { return _value >= that._value; }
        bool operator< (const BStr &that) { return !operator>=(that); }
        bool operator> (const BStr &that) { return !operator<=(that); }

        bool operator==(const std::string &that) { return _value == that; }
        bool operator!=(const std::string &that) { return !operator==(that); }
        bool operator<=(const std::string &that) { return _value <= that; }
        bool operator>=(const std::string &that) { return _value >= that; }
        bool operator< (const std::string &that) { return !operator>=(that); }
        bool operator> (const std::string &that) { return !operator<=(that); }

        BStr &operator+(const BStr &that) { _value += that._value; return *this; }

        BStr &operator+(const std::string &that) { _value += that; return *this; }
};

class BInt : public BItem {
    public:
        int64_t _value;

        /**
         * C++ constructor and destructor
         */
        BInt() : BItem() {}

        BInt(int64_t value) :
            BItem(), _value( value ) {}

        BInt(const BInt &that) :
            BItem(), _value( that._value ) {}

        BInt(const BInt *that) :
            BItem(), _value( that->_value ) {}

        BInt(const BItem *that) {
            if (that->getType().stringValue() != "BInt")
                throw Php::Exception("Error converting BItem to BInt, the original type is " + that->getType());
            _value = that->get().numericValue();
        }

        virtual ~BInt() {}

        /**
         * Regular functions
         */
        Php::Value getType() const {
            return "BInt";
        }

        Php::Value get() const {
            return _value;
        }

        void set(Php::Parameters &params) {
            int64_t value = params[0];
            _value = value;
        }

        Php::Value length() const {
            return (int64_t)(__toString().stringValue().length());
        }

        static BInt* parseI(const std::string &ben, size_t &pt);

        /**
         * Magic methods
         */
        Php::Value __toString() const {
            return "i" + numtos(_value) + "e";
        }

        virtual void __construct(Php::Parameters &params) {
            int64_t value = params[0];
            _value = value;
        }

       /**
         * Operators
         */
        BInt &operator=(const int64_t &that) { _value = that; return *this; }

        bool operator==(const BInt &that) { return _value == that._value; }
        bool operator!=(const BInt &that) { return !operator==(that); }
        bool operator<=(const BInt &that) { return _value <= that._value; }
        bool operator>=(const BInt &that) { return _value >= that._value; }
        bool operator< (const BInt &that) { return !operator>=(that); }
        bool operator> (const BInt &that) { return !operator<=(that); }

        bool operator==(const int64_t &that) { return _value == that; }
        bool operator!=(const int64_t &that) { return !operator==(that); }
        bool operator<=(const int64_t &that) { return _value <= that; }
        bool operator>=(const int64_t &that) { return _value >= that; }
        bool operator< (const int64_t &that) { return !operator>=(that); }
        bool operator> (const int64_t &that) { return !operator<=(that); }

        BInt &operator+(const BInt &that) { _value += that._value; return *this; }
        BInt &operator-(const BInt &that) { _value -= that._value; return *this; }
        BInt &operator*(const BInt &that) { _value *= that._value; return *this; }

        BInt &operator+(const int64_t &that) { _value += that; return *this; }
        BInt &operator-(const int64_t &that) { _value -= that; return *this; }
        BInt &operator*(const int64_t &that) { _value *= that; return *this; }
};

class BDict : public BItem {
    public:
        std::map<std::string, BItem*> BData;

        /**
         * C++ constructor and destructor
         */
        BDict() : BItem() {}

        BDict(const BDict &that) :
            BItem(), BData( that.BData ) {}

        BDict(const BDict *that) :
            BItem(), BData( that->BData ) {}

        BDict(const BItem *that) :
            BItem(), BData( that->getDataD() ) {}

        virtual ~BDict() {}

        /**
         * Regular functions
         */
        Php::Value getType() const {
            return "BDict";
        }

        std::map<std::string, BItem*> getDataD() const {
            return BData;
        }

        Php::Value get(Php::Parameters &params) const;

        BItem* getPath(const std::string &key) const;

        void set(Php::Parameters &params);

        template<typename T>
        void setPath(const std::string &key, T *BItem);

        Php::Value del(Php::Parameters &params);

        Php::Value getKeys() const;

        Php::Value length() const {
            return (int64_t)(__toString().stringValue().length());
        }

        Php::Value size() const {
            return (int64_t)BData.size();
        }

        static BDict* parseD(const std::string &ben, size_t &pt);

        /**
         * Magic methods
         */
        virtual void __construct() {
            BData.clear();
        }

        virtual void __destruct() {
            BData.clear();
        }

        Php::Value __toString() const;
};

class BList : public BItem {
    public:
        std::vector<BItem*> BData;

        /**
         * C++ constructor and destructor
         */
        BList() : BItem() {}

        BList(const BList &that) :
            BItem(), BData( that.BData ) {}

        BList(const BList *that) :
            BItem(), BData( that->BData ) {}

        BList(const BItem *that) :
            BItem(), BData( that->getDataL() ) {}

        virtual ~BList() {}

        /**
         * Regular functions
         */
        Php::Value getType() const {
            return "BList";
        }

        std::vector<BItem*> getDataL() const {
            return BData;
        }

        Php::Value get(Php::Parameters &params) const;
        BItem* getPath(const std::string &key) const;

        void set(Php::Parameters &params);

        template<typename T>
        void setPath(const std::string &key, T *BItem);

        Php::Value del(Php::Parameters &params);

        void add(Php::Parameters &params);

        Php::Value length() const {
            return (int64_t)(__toString().stringValue().length());
        }

        Php::Value size() const {
            return (int64_t)BData.size();
        }

        static BList* parseL(const std::string &ben, size_t &pt);

        /**
         * Magic methods
         */
        virtual void __construct() {
            BData.clear();
        }

        virtual void __destruct() {
            BData.clear();
        }

        Php::Value __toString() const;
};

/**
 * BItem implements
 */
template<typename T>
std::string BItem::numtos(const T &value) const {
    std::string retval;
    std::stringstream sstream;
    sstream << value;
    sstream >> retval;
    return retval;
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

/**
 * BStr implements
 */
BStr* BStr::parseS(const std::string &ben, size_t &pt) {
    if (ben[pt] < '0' || ben[pt] > '9')
        throw Php::Exception("Error parsing BStr");
    const size_t start = pt;
    while (ben[pt] >= '0' && ben[pt] <= '9') ++pt;
    std::string len = ben.substr(start, pt - start);
    ++pt;
    BStr *retval = new BStr(ben.substr(pt, std::stoull(len)));
    pt += std::stoull(len);
    return retval;
}

/**
 * BInt implements
 */
BInt* BInt::parseI(const std::string &ben, size_t &pt) {
    if (ben[pt] != 'i')
        throw Php::Exception("Error parsing BInt");
    ++pt;
    std::string strint = "";
    while (ben[pt] != 'e') {
        strint += ben[pt];
        ++pt;
    }
    ++pt;
    BInt *retval = new BInt(std::stoll(strint));
    return retval;
}

/**
 * BDict implements
 */
Php::Value BDict::get(Php::Parameters &params) const {
    std::string key = params[0];

    BItem *found = getPath(key);
    if (found == nullptr) {
        return (Php::Value)nullptr;
    } else if (found->getType().stringValue() == "BDict") {
        BDict *found1 = new BDict(found);
        return Php::Object(found->getType(), found1);
    } else if (found->getType().stringValue() == "BList") {
        BList *found1 = new BList(found);
        return Php::Object(found->getType(), found1);
    } else if (found->getType().stringValue() == "BStr") {
        BStr *found1 = new BStr(found);
        return Php::Object(found->getType(), found1);
    } else if (found->getType().stringValue() == "BInt") {
        BInt *found1 = new BInt(found);
        return Php::Object(found->getType(), found1);
    }
    return (Php::Value)nullptr;
}

BItem* BDict::getPath(const std::string &key) const {
    std::string path = trimKey(key);
    std::string field = splitKey(path);

    if (path == "") {
        auto search = BData.find(field);
        if(search != BData.end()) {
            BItem *found = search->second;
            if (found->getType().stringValue() == "BDict") {
                BDict *found1 = new BDict(found);
                return found1;
            } else if (found->getType().stringValue() == "BList") {
                BList *found1 = new BList(found);
                return found1;
            } else if (found->getType().stringValue() == "BStr") {
                BStr *found1 = new BStr(found);
                return found1;
            } else if (found->getType().stringValue() == "BInt") {
                BInt *found1 = new BInt(found);
                return found1;
            } else throw Php::Exception("Error handling BDict");
        } else {
            return nullptr;
        }
    }

    auto search = BData.find(field);
    if(search != BData.end()) {
        BItem *found = search->second;
        if (found->getType().stringValue() == "BDict") {
            BDict *found1 = new BDict(found);
            return found1->getPath(path);
        } else if (found->getType().stringValue() == "BList") {
            BList *found1 = new BList(found);
            return found1->getPath(path);
        } else if (found->getType().stringValue() == "BStr") {
            return nullptr;
        } else if (found->getType().stringValue() == "BInt") {
            return nullptr;
        } else throw Php::Exception("Error handling BDict");
    } else {
        return nullptr;
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
    auto search = BData.find(key);
    if (search != BData.end()) {
        BData.erase(key);
        return true;
    } else {
        return false;
    }
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

/**
 * BList implements
 */
Php::Value BList::get(Php::Parameters &params) const {
    std::string key = params[0];

    BItem *found = getPath(key);
    if (found == nullptr) {
        return (Php::Value)nullptr;
    } else if (found->getType().stringValue() == "BDict") {
        BDict *found1 = new BDict(found);
        return Php::Object(found->getType(), found1);
    } else if (found->getType().stringValue() == "BList") {
        BList *found1 = new BList(found);
        return Php::Object(found->getType(), found1);
    } else if (found->getType().stringValue() == "BStr") {
        BStr *found1 = new BStr(found);
        return Php::Object(found->getType(), found1);
    } else if (found->getType().stringValue() == "BInt") {
        BInt *found1 = new BInt(found);
        return Php::Object(found->getType(), found1);
    }
    return (Php::Value)nullptr;
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
        if (found->getType().stringValue() == "BDict") {
            BDict *found1 = new BDict(found);
            return found1;
        } else if (found->getType().stringValue() == "BList") {
            BList *found1 = new BList(found);
            return found1;
        } else if (found->getType().stringValue() == "BStr") {
            BStr *found1 = new BStr(found);
            return found1;
        } else if (found->getType().stringValue() == "BInt") {
            BInt *found1 = new BInt(found);
            return found1;
        } else throw Php::Exception("Error handling BList");
    }

    BItem *found = BData[ifield];
    if (found->getType().stringValue() == "BDict") {
        BDict *found1 = new BDict(found);
        return found1;
    } else if (found->getType().stringValue() == "BList") {
        BList *found1 = new BList(found);
        return found1;
    } else if (found->getType().stringValue() == "BStr") {
        return nullptr;
    } else if (found->getType().stringValue() == "BInt") {
        return nullptr;
    } else throw Php::Exception("Error handling BList");
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
        // make a copy explicitly
        T *item = new T(*BItem);
        if (ifield < BData.size()) {
            BData[ifield] = item;
        } else if (ifield == BData.size()) {
            BData.push_back(item);
        } else {
            throw Php::Exception("the key is too large for the current BList");
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
    size_t key = (int64_t)params[0];
    if (key < 0 || key >= BData.size())
        return false;
    BData.erase(BData.begin() + key);
    return true;
}

void BList::add(Php::Parameters &params) {
    Php::Value item = params[0];
    if (!(item.instanceOf("BDict") || item.instanceOf("BList") ||
            item.instanceOf("BStr") || item.instanceOf("BInt"))) {
        throw Php::Exception("Error adding to BList");
    }
    BItem *cppItem = (BItem*)item.implementation();
    if (cppItem->getType().stringValue() == "BDict") {
        BDict *cppItem1 = (BDict*)item.implementation();
        BDict *cppItemCpy = new BDict(*cppItem1);
        BData.push_back(cppItemCpy);
    } else if (cppItem->getType().stringValue() == "BList") {
        BList *cppItem1 =  (BList*)item.implementation();
        BList *cppItemCpy = new BList(*cppItem1);
        BData.push_back(cppItemCpy);
    } else if (cppItem->getType().stringValue() == "BStr") {
        BStr *cppItem1 = (BStr*)item.implementation();
        BStr *cppItemCpy = new BStr(*cppItem1);
        BData.push_back(cppItemCpy);
    } else if (cppItem->getType().stringValue() == "BInt") {
        BInt *cppItem1 = (BInt*)item.implementation();
        BInt *cppItemCpy = new BInt(*cppItem1);
        BData.push_back(cppItemCpy);
    }
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

/**
 *  tell the compiler that the get_module is a pure C function
 */
extern "C" {
    
    /**
     *  Function that is called by PHP right after the PHP process
     *  has started, and that returns an address of an internal PHP
     *  strucure with all the details and features of your extension
     *
     *  @return void*   a pointer to an address that is understood by PHP
     */
    PHPCPP_EXPORT void *get_module() 
    {
        // static(!) Php::Extension object that should stay in memory
        // for the entire duration of the process (that's why it's static)
        static Php::Extension myExtension("bencode", "1.0");

        Php::Class<BItem> _BItem("BItem");
        _BItem.method("getType", &BItem::getType);
        _BItem.method("parse", &BItem::parse, {
                Php::ByVal("ben", Php::Type::String, true)
                });
        _BItem.method("load", &BItem::load, {
                Php::ByVal("file", Php::Type::String, true)
                });

        Php::Class<BDict> _BDict("BDict");
        _BDict.extends(_BItem);
        _BDict.method("getType", &BDict::getType);
        _BDict.method("set", &BDict::set, {
                Php::ByVal("key", Php::Type::String, true),
                Php::ByVal("value", Php::Type::Null, true)
                });
        _BDict.method("get", &BDict::get, {
                Php::ByVal("key", Php::Type::String, true)
                });
        _BDict.method("del", &BDict::del, {
                Php::ByVal("key", Php::Type::String, true)
                });
        _BDict.method("getKeys", &BDict::getKeys);
        _BDict.method("length", &BDict::length);
        _BDict.method("size", &BDict::size);
        _BDict.method("__toString", &BDict::__toString);
        _BDict.method("__construct", &BDict::__construct);
        _BDict.method("__destruct", &BDict::__destruct);

        Php::Class<BList> _BList("BList");
        _BList.extends(_BItem);
        _BList.method("getType", &BList::getType);
        _BList.method("set", &BList::set, {
                Php::ByVal("key", Php::Type::String, true),
                Php::ByVal("value", Php::Type::Null, true)
                });
        _BList.method("get", &BList::get, {
                Php::ByVal("key", Php::Type::String, true)
                });
        _BList.method("del", &BList::del, {
                Php::ByVal("key", Php::Type::Numeric, true)
                });
        _BList.method("add", &BList::add, {
                Php::ByVal("value", Php::Type::Null, true)
                });
        _BList.method("length", &BList::length);
        _BList.method("size", &BList::size);
        _BList.method("__toString", &BList::__toString);
        _BList.method("__construct", &BList::__construct);
        _BList.method("__destruct", &BList::__destruct);

        Php::Class<BStr> _BStr("BStr");
        _BStr.extends(_BItem);
        _BStr.method("getType", &BStr::getType);
        _BStr.method("get", &BStr::get);
        _BStr.method("set", &BStr::set, {
                Php::ByVal("value", Php::Type::String, true)
                });
        _BStr.method("length", &BStr::length);
        _BStr.method("__toString", &BStr::__toString);
        _BStr.method("__construct", &BStr::__construct);

        Php::Class<BInt> _BInt("BInt");
        _BInt.extends(_BItem);
        _BInt.method("getType", &BInt::getType);
        _BInt.method("get", &BInt::get);
        _BInt.method("set", &BInt::set, {
                Php::ByVal("value", Php::Type::String, true)
                });
        _BInt.method("length", &BInt::length);
        _BInt.method("__toString", &BInt::__toString);
        _BInt.method("__construct", &BInt::__construct);

        myExtension.add(std::move(_BItem));
        myExtension.add(std::move(_BDict));
        myExtension.add(std::move(_BList));
        myExtension.add(std::move(_BStr));
        myExtension.add(std::move(_BInt));
        
        // return the extension
        return myExtension;
    }
}

