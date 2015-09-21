#include <phpcpp.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

class BItem : public Php::Base {
    protected:
        template<typename T>
        std::string numtos(const T &value) const {
            std::string retval;
            std::stringstream sstream;
            sstream << value;
            sstream >> retval;
            return retval;
        }

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
            return Php::Value();
        }

        virtual void set(std::string v) {
            return;
        }

        virtual void set(int64_t v) {
            return;
        }

        virtual void set(std::string vk, BItem *vv) {
            return;
        }

        virtual std::unordered_map<std::string, BItem*> getData() const {
            std::unordered_map<std::string, BItem*> dummy;
            return dummy;
        }

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

        /**
         * Magic methods
         */
        Php::Value __toString() {
            return numtos(_value.length()) + ":" + _value;
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

        /**
         * Magic methods
         */
        Php::Value __toString() {
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
            BItem(), BData( that->getData() ) {}

        virtual ~BList() {}

        /**
         * Regular functions
         */
        Php::Value getType() const {
            return "BList";
        }

        std::unordered_map<std::string, BItem*> getData() const {
            return BData;
        }

        Php::Value get(Php::Parameters &params) const {
            size_t key = params[0];
            if (key < 0 || key >= BData.size()) {
                throw Php::Exception("Out of range");
            }

            BItem *found = BData[key];
            if (found->getType().stringValue() == "BDict") {
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

        void set(Php::Parameters &params) {
            size_t key = params[0];
            if (key < 0) {
                throw Php::Exception("Index of BList should be a positive value");
            }
            if (key >= BData.size()) {
                throw Php::Exception("Out of range, do you mean BList->add() ?");
            }
            Php::Value item = params[1];
            if (!(item.instanceOf("BDict") || item.instanceOf("BList") ||
                    item.instanceOf("BStr") || item.instanceOf("BInt"))) {
                throw Php::Exception("Error adding to BList");
            }
            BItem *cppItem = (BItem*)item.implementation();
            if (cppItem->getType().stringValue() == "BDict") {
                BDict *cppItem1 = (BDict*)item.implementation();
                BDict *cppItemCpy = new BDict(*cppItem1);
                BData[key] = cppItemCpy;
            } else if (cppItem->getType().stringValue() == "BList") {
                BList *cppItem1 =  (BList*)item.implementation();
                BList *cppItemCpy = new BList(*cppItem1);
                BData[key] = cppItemCpy;
            } else if (cppItem->getType().stringValue() == "BStr") {
                BStr *cppItem1 = (BStr*)item.implementation();
                BStr *cppItemCpy = new BStr(*cppItem1);
                BData[key] = cppItemCpy;
            } else if (cppItem->getType().stringValue() == "BInt") {
                BInt *cppItem1 = (BInt*)item.implementation();
                BInt *cppItemCpy = new BInt(*cppItem1);
                BData[key] = cppItemCpy;
            }
        }

        Php::Value length() {
            return (int64_t)BData.size();
        }

        /**
         * Magic methods
         */
        virtual void __construct() {
            BData.clear();
        }

        virtual void __destruct() {
            BData.clear();
        }

        Php::Value __toString() {
            return "list testing";
        }
};

class BDict : public BItem {
    public:
        std::unordered_map<std::string, BItem*> BData;

        /**
         * C++ constructor and destructor
         */
        BDict() : BItem() {}

        BDict(const BDict &that) :
            BItem(), BData( that.BData ) {}

        BDict(const BDict *that) :
            BItem(), BData( that->BData ) {}

        BDict(const BItem *that) :
            BItem(), BData( that->getData() ) {}

        virtual ~BDict() {}

        /**
         * Regular functions
         */
        Php::Value getType() const {
            return "BDict";
        }

        std::unordered_map<std::string, BItem*> getData() const {
            return BData;
        }

        Php::Value get(Php::Parameters &params) const {
            std::string key = params[0];

            auto search = BData.find(key);
            if(search != BData.end()) {
                BItem *found = search->second;
                if (found->getType().stringValue() == "BDict") {
                    BDict *found1 = new BDict(found);
                    return Php::Object(found->getType(), found1);
                } else if (found->getType().stringValue() == "BList") {
                } else if (found->getType().stringValue() == "BStr") {
                    BStr *found1 = new BStr(found);
                    return Php::Object(found->getType(), found1);
                } else if (found->getType().stringValue() == "BInt") {
                    BInt *found1 = new BInt(found);
                    return Php::Object(found->getType(), found1);
                }
            }
            return (Php::Value)nullptr;
        }

        void set(Php::Parameters &params) {
            std::string key = params[0];
            Php::Value item = params[1];
            if (!(item.instanceOf("BDict") || item.instanceOf("BList") ||
                    item.instanceOf("BStr") || item.instanceOf("BInt"))) {
                throw Php::Exception("Error adding to BDict");
            }
            BData.erase(key);
            BItem *cppItem = (BItem*)item.implementation();
            if (cppItem->getType().stringValue() == "BDict") {
                BDict *cppItem1 = (BDict*)item.implementation();
                BDict *cppItemCpy = new BDict(*cppItem1);
                BData.insert({key, cppItemCpy});
            } else if (cppItem->getType().stringValue() == "BList") {
            } else if (cppItem->getType().stringValue() == "BStr") {
                BStr *cppItem1 = (BStr*)item.implementation();
                BStr *cppItemCpy = new BStr(*cppItem1);
                BData.insert({key, cppItemCpy});
            } else if (cppItem->getType().stringValue() == "BInt") {
                BInt *cppItem1 = (BInt*)item.implementation();
                BInt *cppItemCpy = new BInt(*cppItem1);
                BData.insert({key, cppItemCpy});
            }
        }

        Php::Value length() {
            return (int64_t)BData.size();
        }

        /**
         * Magic methods
         */
        virtual void __construct() {
            BData.clear();
        }

        virtual void __destruct() {
            BData.clear();
        }

        Php::Value __toString() {
            return "dict testing";
        }
};

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
        _BItem.method("getType", &BItem::getType, {});

        Php::Class<BDict> _BDict("BDict");
        _BDict.extends(_BItem);
        _BDict.method("getType", &BDict::getType, {});
        _BDict.method("set", &BDict::set, {
                Php::ByVal("key", Php::Type::String, true),
                Php::ByVal("value", Php::Type::Null, true)
                });
        _BDict.method("get", &BDict::get, {
                Php::ByVal("key", Php::Type::String, true)
                });
        _BDict.method("length", &BDict::length, {});
        _BDict.method("__construct", &BDict::__construct);
        _BDict.method("__destruct", &BDict::__destruct);

        Php::Class<BStr> _BStr("BStr");
        _BStr.extends(_BItem);
        _BStr.method("getType", &BStr::getType, {});
        _BStr.method("get", &BStr::get, {});
        _BStr.method("set", &BStr::set, {
                Php::ByVal("value", Php::Type::String, true)
                });
        _BStr.method("__construct", &BStr::__construct);

        Php::Class<BInt> _BInt("BInt");
        _BInt.extends(_BItem);
        _BInt.method("getType", &BInt::getType, {});
        _BInt.method("get", &BInt::get, {});
        _BInt.method("set", &BInt::set, {
                Php::ByVal("value", Php::Type::String, true)
                });
        _BInt.method("__construct", &BInt::__construct);

        myExtension.add(std::move(_BItem));
        myExtension.add(std::move(_BDict));
        myExtension.add(std::move(_BStr));
        myExtension.add(std::move(_BInt));
        
        // return the extension
        return myExtension;
    }
}

