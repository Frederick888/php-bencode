#include <string>
#include "bitem.h"
#include "bdict.h"
#include "blist.h"
#include "bstr.h"
#include "bint.h"
#include "zend_container.h"

bdict::bdict() : bitem() {
    ALLOC_HASHTABLE(_data);
    zend_hash_init(_data, 0, NULL, ZVAL_PTR_DTOR, 0);
}

bdict::bdict(const bdict *that) : bitem() {
    ALLOC_HASHTABLE(_data);
    zend_hash_init(_data, 0, NULL, ZVAL_PTR_DTOR, 0);
    //zend_hash_copy(_data, that->_data, (copy_ctor_func_t) ZVAL_COPY_CTOR);
    for(zend_hash_internal_pointer_reset(that->_data);
            zend_hash_has_more_elements(that->_data) == SUCCESS;
            zend_hash_move_forward(that->_data)) {
        zval tmp;
        ZVAL_OBJ(&tmp, zend_container::bnode_object_clone(zend_hash_get_current_data(that->_data)));
        zend_string *_str_index;
        zend_ulong _num_index;
        zend_hash_get_current_key(that->_data, &_str_index, &_num_index);
        zend_hash_add(_data, _str_index, &tmp);
    }
}

bdict::~bdict() {
    zend_hash_destroy(_data);
    FREE_HASHTABLE(_data);
}

std::string bdict::get_type() const {
    return "bdict";
}

zval * bdict::get(const std::string &key) const {
    if (!zend_hash_str_exists(_data, key.c_str(), key.length())) {
        zval _zv;
        zval *zv = &_zv;
        ZVAL_BOOL(zv, 0);
        return zv;
    }
    return zend_hash_str_find(_data, key.c_str(), key.length());
}

bool bdict::has(const std::string &key) const {
    if (zend_hash_str_exists(_data, key.c_str(), key.length())) {
        return true;
    } else {
        return false;
    }
}

void bdict::set(const std::string &key, zval *value) {
    //zval tmp;
    //tmp = *value;
    //zval_copy_ctor(&tmp);
    std::string class_name = zend_container::bnode_object_get_class_name(value);
    zend_object *clone_object = NULL;
    if (class_name == "bdict" || class_name == "blist" ||
            class_name == "bstr" || class_name == "bint") {
        clone_object = zend_container::bnode_object_clone(value);
    } else {
        return;
    }
    zval tmp;
    ZVAL_OBJ(&tmp, clone_object);
    if (zend_hash_str_exists(_data, key.c_str(), key.length())) {
        zend_hash_str_update(_data, key.c_str(), key.length(), &tmp);
    } else {
        zend_hash_str_add(_data, key.c_str(), key.length(), &tmp);
    }
}

bool bdict::del(const std::string &key) {
    if (zend_hash_str_del(_data, key.c_str(), key.length()) == SUCCESS) {
        return true;
    } else {
        return false;
    }
}

size_t bdict::length() const {
    return (encode().length() / sizeof(char));
}

size_t bdict::count() const {
    return zend_array_count(_data);
}

zval * bdict::parse(const std::string &ben, size_t &pt) {
    if (ben[pt] != 'd')
        zend_throw_exception(
                zend_container::bdict_ce,
                "Error parsing bdict",
                1);
    zval _zv;
    zval *zv = &_zv;
    object_init_ex(zv, zend_container::bdict_ce);
    bdict_object *intern = zend_container::bdict_fetch_object(Z_OBJ_P(zv));
    intern->bdict_data = new bdict();
    ++pt;

    while (ben[pt] != 'e') {
        size_t start = pt;
        while (ben[pt] >= '0' && ben[pt] <= '9') ++pt;
        std::string key_len = ben.substr(start, pt - start);
        ++pt;
        std::string key = ben.substr(pt, std::stoull(key_len));
        pt += std::stoull(key_len);
        if (ben[pt] == 'd') {
            zval bnode = *bdict::parse(ben, pt);
            zend_hash_str_add(intern->bdict_data->_data, key.c_str(), key.length(), &bnode);
        } else if (ben[pt] == 'l') {
            zval bnode = *blist::parse(ben, pt);
            zend_hash_str_add(intern->bdict_data->_data, key.c_str(), key.length(), &bnode);
        } else if (ben[pt] >= '0' && ben[pt] <= '9') {
            zval bnode = *bstr::parse(ben, pt);
            zend_hash_str_add(intern->bdict_data->_data, key.c_str(), key.length(), &bnode);
        } else if (ben[pt] == 'i') {
            zval bnode = *bint::parse(ben, pt);
            zend_hash_str_add(intern->bdict_data->_data, key.c_str(), key.length(), &bnode);
        } else {
            zend_throw_exception(
                    zend_container::bdict_ce,
                    "Error parsing bdict",
                    1);
        }
    }
    ++pt;
    return zv;
}

std::string bdict::encode() const {
    std::string result = "d";
    for(zend_hash_internal_pointer_reset(_data);
            zend_hash_has_more_elements(_data) == SUCCESS;
            zend_hash_move_forward(_data)) {
        zend_string *str_index;
        zend_ulong num_index;
        zend_hash_get_current_key(_data, &str_index, &num_index);
        zval *value = zend_hash_get_current_data(_data);
        std::string class_name = zend_container::bnode_object_get_class_name(value);
        std::string _str_index(ZSTR_VAL(str_index));

        result += bitem::numtos(_str_index.length()) + ":" + _str_index;
        if (class_name == "bdict") {
            result += (zend_container::bdict_fetch_object(Z_OBJ_P(value)))->bdict_data->encode();
        } else if (class_name == "blist") {
            result += (zend_container::blist_fetch_object(Z_OBJ_P(value)))->blist_data->encode();
        } else if (class_name == "bstr") {
            result += (zend_container::bstr_fetch_object(Z_OBJ_P(value)))->bstr_data->encode();
        } else if (class_name == "bint") {
            result += (zend_container::bint_fetch_object(Z_OBJ_P(value)))->bint_data->encode();
        }
    }
    return result + "e";
}

zval * bdict::to_array(const bool include_meta) const {
    zval _zv;
    zval *zv = &_zv;
    array_init(zv);

    for(zend_hash_internal_pointer_reset(_data);
            zend_hash_has_more_elements(_data) == SUCCESS;
            zend_hash_move_forward(_data)) {
        zend_string *str_index;
        zend_ulong num_index;
        zend_hash_get_current_key(_data, &str_index, &num_index);
        zval *value = zend_hash_get_current_data(_data);
        std::string class_name = zend_container::bnode_object_get_class_name(value);
        char *_str_index = (char *)emalloc(ZSTR_LEN(str_index));
        strcpy(_str_index, ZSTR_VAL(str_index));
        if (class_name == "bdict") {
            bdict_object *bnode = zend_container::bdict_fetch_object(Z_OBJ_P(value));
            zval subarray = *bnode->bdict_data->to_array(include_meta);
            add_assoc_zval(zv, _str_index, &subarray);
        } else if (class_name == "blist") {
            blist_object *bnode = zend_container::blist_fetch_object(Z_OBJ_P(value));
            zval subarray = *bnode->blist_data->to_array(include_meta);
            add_assoc_zval(zv, _str_index, &subarray);
        } else if (class_name == "bstr") {
            bstr_object *bnode = zend_container::bstr_fetch_object(Z_OBJ_P(value));
            zval subarray = *bnode->bstr_data->to_array(include_meta);
            add_assoc_zval(zv, _str_index, &subarray);
        } else if (class_name == "bint") {
            bint_object *bnode = zend_container::bint_fetch_object(Z_OBJ_P(value));
            zval subarray = *bnode->bint_data->to_array(include_meta);
            add_assoc_zval(zv, _str_index, &subarray);
        }
        efree(_str_index);
    }

    if (include_meta) {
        char *_type = estrdup("_type");
        char *_type_data = estrdup("bdict");
        char *_length = estrdup("_length");
        char *_data = estrdup("_data");
        zval _zv_outer;
        zval *zv_outer = &_zv_outer;
        array_init(zv_outer);
        add_assoc_string(zv_outer, _type, _type_data);
        add_assoc_long(zv_outer, _length, length());
        add_assoc_zval(zv_outer, _data, zv);
        efree(_type);
        efree(_type_data);
        efree(_length);
        efree(_data);
        return zv_outer;
    }
    return zv;
}
