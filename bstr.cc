#include <string>
#include <cstring>
#include "bitem.h"
#include "bdict.h"
#include "blist.h"
#include "bstr.h"
#include "bint.h"
#include "zend_container.h"

std::string bstr::get_type() const {
    return "bstr";
}

std::string bstr::get() const {
    return _value;
}

void bstr::set(const std::string &value) {
    _value = value;
}

size_t bstr::length() const {
    return (encode().length() / sizeof(char));
}

zval * bstr::parse(const std::string &ben, size_t &pt) {
    if (ben[pt] < '0' || ben[pt] > '9')
        return bitem::throw_general_exception("Error parsing bstr");
    const size_t start = pt;
    while (ben[pt] >= '0' && ben[pt] <= '9') ++pt;
    std::string len = ben.substr(start, pt - start);
    ++pt;

    zval _zv;
    zval *zv = &_zv;
    object_init_ex(zv, zend_container::bstr_ce);
    bstr_object *intern = zend_container::bstr_fetch_object(Z_OBJ_P(zv));
    intern->bstr_data = new bstr(ben.substr(pt, std::stoull(len)));
    pt += std::stoull(len);
    return zv;
}

std::string bstr::encode() const {
    return std::to_string(_value.length()) + ":" + _value;
}

zval * bstr::to_array(const bool include_meta) const {
    zval _zv;
    zval *zv = &_zv;
    if (include_meta) {
        array_init(zv);
        if (_value.length() == 0) return zv;
        char *_type = estrdup("_type");
        char *_type_data = estrdup("bstr");
        char *_length = estrdup("_length");
        char *_data = estrdup("_data");
        char *_data_data = zend_container::get_cstring(_value);
        add_assoc_string(zv, _type, _type_data);
        add_assoc_long(zv, _length, length());
        add_assoc_string(zv, _data, _data_data);
        efree(_type);
        efree(_type_data);
        efree(_length);
        efree(_data);
        efree(_data_data);
    } else {
        char *tmp = zend_container::get_cstring(_value);
        ZVAL_STRING(zv, tmp);
        efree(tmp);
    }
    return zv;
}
