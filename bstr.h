#ifndef BENCODE_HEADER_BSTR
#define BENCODE_HEADER_BSTR

extern "C" {
#include "php.h"
#include "zend_exceptions.h"
}

#include <string>
#include "bitem.h"

class bstr : public bitem {
    public:
        std::string _value;

        bstr() : bitem() {}
        bstr(std::string value) :
            bitem(), _value( value ) {}
        bstr(const bstr *that) :
            bitem(), _value( that->_value ) {}
        ~bstr() {}

        std::string get_type() const;
        std::string get() const;
        void set(const std::string &value);
        size_t length() const;

        static zval * parse(const std::string &ben, size_t &pt);
        zval * to_array() const;
        zval * to_meta_array() const;
};

#endif
