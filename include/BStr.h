#ifndef HEADER_BSTR
#define HEADER_BSTR

#include <phpcpp.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

class BStr : public BItem {
    private:
        BStr *me_str;

    public:
        std::string _value;

        /**
         * C++ constructor and destructor
         */
        BStr() : BItem() { me_str = this; }
        
        BStr(std::string value) :
            BItem(), _value( value ) { me_str = this; }

        BStr(const BStr &that) :
            BItem(), _value( that._value ) { me_str = this; }

        BStr(const BStr *that) :
            BItem(), _value( that->_value ) { me_str = this; }

        BStr(const BItem *that);

        virtual ~BStr() {}

        /**
         * Regular functions
         */
        Php::Value getType() const;

        BStr* me() { return me_str; }

        std::string getDataS() const;

        Php::Value get() const;

        void set(Php::Parameters &params);

        Php::Value length() const;

        static BStr* parseS(const std::string &ben, size_t &pt);

        Php::Value toArray() const;

        Php::Value toMetaArray() const;

        void csearch(const std::string &needle, const char &mode,
                    std::vector<std::string> &pathStack, std::vector<std::string> &result) const;

        /**
         * Magic methods
         */
        Php::Value __toString() const;

        virtual void __construct(Php::Parameters &params);

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

#endif
