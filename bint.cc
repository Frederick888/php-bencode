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
    size_t start = ++pt;
    while (ben[pt] != 'e') ++pt;
    long result = std::stoll(ben.substr(start, pt - start));
    ++pt;

    zval *zv = new zval();
    zend_object *zo = zend_container::bint_object_new(zend_container::bint_ce);
    ZVAL_OBJ(zv, zo);
    bint_object *intern = zend_container::bint_fetch_object(Z_OBJ_P(zv));
    intern->bnode_data = new bint(result);
    return zv;
}

std::string bint::encode() const {
    return "i" + std::to_string(_value) + "e";
}

zval * bint::to_array(const bool include_meta) const {
    zval *zv = new zval();
    if (include_meta) {
        array_init(zv);
        add_assoc_string(zv, (char *)"_type", (char *)"bint");
        add_assoc_long(zv, (char *)"_length", length());
        add_assoc_long(zv, (char *)"_data", _value);
    } else {
        ZVAL_LONG(zv, _value);
    }
    return zv;
}

zval * bint::search(const std::string &needle, const long &mode, const std::string path) const {
    zval *zv = new zval();
    array_init(zv);

    if (mode == 1 && bitem::is_ll(needle) && _value == std::stoll(needle))
        add_next_index_stringl(zv, path.c_str(), path.length());

    return zv;
}
