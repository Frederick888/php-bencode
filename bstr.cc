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
    if (!isdigit(ben[pt]))
        return bitem::throw_general_exception("Error parsing bstr");
    const size_t start = pt;
    while (isdigit(ben[pt])) ++pt;
    size_t len = std::stoull(ben.substr(start, pt - start));
    ++pt;

    zval *zv = new zval();
    zend_object *zo = zend_container::bstr_object_new(zend_container::bstr_ce);
    ZVAL_OBJ(zv, zo);
    bstr_object *intern = zend_container::bstr_fetch_object(Z_OBJ_P(zv));
    intern->bnode_data = new bstr(ben.substr(pt, len));
    pt += len;
    return zv;
}

std::string bstr::encode() const {
    return std::to_string(_value.length()) + ":" + _value;
}

zval * bstr::to_array(const bool include_meta) const {
    zval *zv = new zval();
    char *_data = (char *)emalloc(_value.length() + 1);
    memcpy(_data, _value.c_str(), _value.length());
    if (include_meta) {
        array_init(zv);
        if (_value.length() == 0) return zv;
        add_assoc_string(zv, (char *) "_type", (char *) "bstr");
        add_assoc_long(zv, (char *) "_length", length());
        add_assoc_stringl(zv, (char *) "_data", _data, _value.length());
    } else {
        ZVAL_STRINGL(zv, _data, _value.length());
    }
    efree(_data);
    return zv;
}

zval * bstr::search(const std::string &needle, const long &mode, const std::string path) const {
    zval *zv = new zval();
    array_init(zv);

    if (mode == 0 && _value.find(needle) != std::string::npos)
        add_next_index_stringl(zv, path.c_str(), path.length());

    return zv;
}
