#include <string>
#include "bitem.h"
#include "bdict.h"
#include "bstr.h"
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
        zval temp;
        ZVAL_OBJ(&temp, Z_OBJ_P(zend_hash_get_current_data(that->_data))->handlers->clone_obj(zend_hash_get_current_data(that->_data)));
        zend_string *_str_index;
        zend_ulong _num_index;
        zend_hash_get_current_key(that->_data, &_str_index, &_num_index);
        zend_hash_add(_data, _str_index, &temp);
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
    //zval temp;
    //temp = *value;
    //zval_copy_ctor(&temp);
    size_t _class_name_len = ZSTR_LEN(Z_OBJ_P(value)->ce->name);
    char *_class_name = (char *)emalloc(_class_name_len);
    strcpy(_class_name, ZSTR_VAL(Z_OBJ_P(value)->ce->name));
    std::string class_name(_class_name);
    efree(_class_name);
    zend_object *clone_object = NULL;
    if (class_name == "bdict" || class_name == "blist" ||
            class_name == "bstr" || class_name == "bint") {
        clone_object = Z_OBJ_P(value)->handlers->clone_obj(value);
    } else {
        return;
    }
    zval temp;
    ZVAL_OBJ(&temp, clone_object);
    if (zend_hash_str_exists(_data, key.c_str(), key.length())) {
        zend_hash_str_update(_data, key.c_str(), key.length(), &temp);
    } else {
        zend_hash_str_add(_data, key.c_str(), key.length(), &temp);
    }
}

bool bdict::del(const std::string &key) {
    if (zend_hash_str_del(_data, key.c_str(), key.length()) == SUCCESS) {
        return true;
    } else {
        return false;
    }
}

zval * bdict::parse(const std::string &ben, size_t &pt) {
    zval _zv;
    zval *zv = &_zv;
    array_init(zv);

    std::string key = "";
    ++pt;
    while (ben[pt] >= '0' && ben[pt] <= '9') {
        key += ben[pt++];
    }
    ++pt;
    size_t key_end = pt + std::stoul(key, nullptr, 0);
    key = "";
    for (size_t i = pt; i < key_end; i++) key += ben[i];
    pt = key_end;

    std::string value = "tttto";
    char *ckey = (char *)emalloc(key.length());
    strcpy(ckey, key.c_str());
    char *cvalue = (char *)emalloc(value.length());
    strcpy(cvalue, value.c_str());
    add_assoc_string(zv, ckey, cvalue);
    efree(ckey); efree(cvalue);

    return zv;
}

zval * bdict::to_array() const {
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
        char *class_name = (char *)emalloc(
                ZSTR_LEN(Z_OBJ_P(value)->ce->name));
        class_name = strcpy(class_name, ZSTR_VAL(Z_OBJ_P(value)->ce->name));
        std::string _class_name(class_name);
        char *_str_index = (char *)emalloc(ZSTR_LEN(str_index));
        strcpy(_str_index, ZSTR_VAL(str_index));
        if (_class_name == "bdict") {
            bdict_object *bnode = zend_container::bdict_fetch_object(Z_OBJ_P(value));
            zval subarray = *bnode->bdict_data->to_array();
            add_assoc_zval(zv, _str_index, &subarray);
        } else if (_class_name == "bstr") {
            bstr_object *bnode = zend_container::bstr_fetch_object(Z_OBJ_P(value));
            std::string subvalue = bnode->bstr_data->get();
            char *_subvalue = (char *)emalloc(subvalue.length());
            strcpy(_subvalue, subvalue.c_str());
            add_assoc_string(zv, _str_index, _subvalue);
            efree(_subvalue);
        }
        efree(class_name);
        efree(_str_index);
    }

    return zv;
}
