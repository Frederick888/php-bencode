#ifndef BENCODE_HEADER_BINT
#define BENCODE_HEADER_BINT

extern "C" {
#include "php.h"
#include "zend_exceptions.h"
}

#include <string>
#include "bitem.h"

class bint : public bitem {
    public:
        long _value;

        bint() : bitem() {}
        bint(long value) :
            bitem(), _value( value ) {}
        bint(const bint *that) :
            bitem(), _value( that->_value ) {}
        ~bint() {}

        std::string get_type() const;
        long get() const;
        void set(const long &value);
        size_t length() const;

        static zval * parse(const std::string &ben, size_t &pt);
        std::string encode() const;
        zval * to_array(const bool include_meta) const;
};

#endif
