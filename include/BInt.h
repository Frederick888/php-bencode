#ifndef HEADER_BINT
#define HEADER_BINT

#include <phpcpp.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

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

        BInt(const BItem *that);

        virtual ~BInt() {}

        /**
         * Regular functions
         */
        Php::Value getType() const;

        Php::Value get() const;

        void set(Php::Parameters &params);

        Php::Value length() const;

        static BInt* parseI(const std::string &ben, size_t &pt);

        Php::Value toMetaArray() const;

        /**
         * Magic methods
         */
        Php::Value __toString() const;

        virtual void __construct(Php::Parameters &params);

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

#endif
