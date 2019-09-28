#ifndef BENCODE_HEADER_BSTR
#define BENCODE_HEADER_BSTR

extern "C" {
#include "php.h"
#include "zend_exceptions.h"
}

#include "bitem.h"
#include <string>

class bstr : public bitem
{
public:
    std::string _value;

    bstr()
        : bitem() {}
    bstr(std::string value)
        : bitem(), _value(value) {}
    bstr(const bstr *that)
        : bitem(), _value(that->_value) {}
    ~bstr() {}

    std::string get_type() const;
    std::string get() const;
    void set(const std::string &value);
    size_t length() const;

    static zval *parse(const std::string &ben, size_t &pt);
    std::string encode() const;
    zval *to_array(const bool include_meta) const;
    zval *search(const std::string &needle, const long &mode, const std::string path) const;
};

#endif
