#include <string>
#include <cstring>
#include "bitem.h"
#include "bdict.h"
#include "blist.h"
#include "bstr.h"
#include "bint.h"
#include "zend_container.h"

std::string bint::get_type() const {
    return "bint";
}

long bint::get() const {
    return _value;
}

void bint::set(const long &value) {
    _value = value;
}

size_t bint::length() const {
    std::string ben = encode();
    return ben.length();
}

zval * bint::parse(const std::string &ben, size_t &pt) {
    if (ben[pt] != 'i')
        return bitem::throw_general_exception("Error parsing bint");
    ++pt;
    std::string strint = "";
    while (ben[pt] != 'e') strint += ben[pt++];
    long result = std::stoll(strint);
    ++pt;

    zval _zv;
    zval *zv = &_zv;
    zend_object *zo = zend_container::bint_object_new(zend_container::bint_ce);
    ZVAL_OBJ(zv, zo);
    bint_object *intern = zend_container::bint_fetch_object(Z_OBJ_P(zv));
    intern->bint_data = new bint(result);
    return zv;
}

std::string bint::encode() const {
    return "i" + std::to_string(_value) + "e";
}

zval * bint::to_array(const bool include_meta) const {
    zval _zv;
    zval *zv = &_zv;
    if (include_meta) {
        array_init(zv);
        char *_type = estrdup("_type");
        char *_type_data = estrdup("bint");
        char *_length = estrdup("_length");
        char *_data = estrdup("_data");
        add_assoc_string(zv, _type, _type_data);
        add_assoc_long(zv, _length, length());
        add_assoc_long(zv, _data, _value);
        efree(_type);
        efree(_type_data);
        efree(_length);
        efree(_data);
    } else {
        ZVAL_LONG(zv, _value);
    }
    return zv;
}
