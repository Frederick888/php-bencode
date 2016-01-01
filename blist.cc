#include <string>
#include "bitem.h"
#include "bdict.h"
#include "blist.h"
#include "bstr.h"
#include "bint.h"
#include "zend_container.h"

blist::blist() : bitem() {
    ALLOC_HASHTABLE(_data);
    zend_hash_init(_data, 0, NULL, ZVAL_PTR_DTOR, 0);
}

blist::blist(const blist *that) : bitem() {
    ALLOC_HASHTABLE(_data);
    zend_hash_init(_data, 0, NULL, ZVAL_PTR_DTOR, 0);
    //zend_hash_copy(_data, that->_data, (copy_ctor_func_t) ZVAL_COPY_CTOR);
    for(zend_hash_internal_pointer_reset(that->_data);
            zend_hash_has_more_elements(that->_data) == SUCCESS;
            zend_hash_move_forward(that->_data)) {
        zval temp;
        ZVAL_OBJ(&temp, zend_container::bnode_object_clone(zend_hash_get_current_data(that->_data)));
        zend_string *_str_index;
        zend_ulong _num_index;
        zend_hash_get_current_key(that->_data, &_str_index, &_num_index);
        zend_hash_index_add(_data, _num_index, &temp);
    }
}

blist::~blist() {
    zend_hash_destroy(_data);
    FREE_HASHTABLE(_data);
}

std::string blist::get_type() const {
    return "blist";
}

zval * blist::get(const size_t &key) const {
    if (!zend_hash_index_exists(_data, key)) {
        zval _zv;
        zval *zv = &_zv;
        ZVAL_BOOL(zv, 0);
        return zv;
    }
    return zend_hash_index_find(_data, key);
}

bool blist::has(const size_t &key) const {
    if (zend_hash_index_exists(_data, key)) {
        return true;
    } else {
        return false;
    }
}

void blist::add(zval *value) {
    std::string class_name = zend_container::bnode_object_get_class_name(value);
    zend_object *clone_object = NULL;
    if (class_name == "blist" || class_name == "blist" ||
            class_name == "bstr" || class_name == "bint") {
        clone_object = zend_container::bnode_object_clone(value);
    } else {
        return;
    }
    zval temp;
    ZVAL_OBJ(&temp, clone_object);
    zend_hash_next_index_insert(_data, &temp);
}

void blist::set(const size_t &key, zval *value) {
    std::string class_name = zend_container::bnode_object_get_class_name(value);
    zend_object *clone_object = NULL;
    if (class_name == "blist" || class_name == "blist" ||
            class_name == "bstr" || class_name == "bint") {
        clone_object = zend_container::bnode_object_clone(value);
    } else {
        return;
    }
    zval temp;
    ZVAL_OBJ(&temp, clone_object);
    if (zend_hash_index_exists(_data, key)) {
        zend_hash_index_update(_data, key, &temp);
    } else {
        zend_hash_next_index_insert(_data, &temp);
    }
}

bool blist::del(const size_t &key) {
    if (zend_hash_index_del(_data, key) == SUCCESS) {
        if (key < count()) {
            size_t constant_count = count();
            for (size_t i = key; i < constant_count; ++i) {
                if (zend_hash_index_exists(_data, i)) {
                    zend_hash_index_del(_data, i);
                }
                zval copy_next;
                ZVAL_OBJ(&copy_next, zend_container::bnode_object_clone(zend_hash_index_find(_data, i + 1)));
                zend_hash_index_update(_data, i, &copy_next);
            }
            zend_hash_index_del(_data, constant_count);
        }
        return true;
    } else {
        return false;
    }
}

size_t blist::length() const {
    return (encode().length() / sizeof(char));
}

size_t blist::count() const {
    return zend_array_count(_data);
}

zval * blist::parse(const std::string &ben, size_t &pt) {
    if (ben[pt] != 'l')
        zend_throw_exception(
                zend_container::blist_ce,
                "Error parsing blist",
                1);
    ++pt;
    zval _zv;
    zval *zv = &_zv;
    object_init_ex(zv, zend_container::blist_ce);
    blist_object *intern = zend_container::blist_fetch_object(Z_OBJ_P(zv));
    intern->blist_data = new blist();

    while (ben[pt] != 'e') {
        if (ben[pt] == 'd') {
            zval bnode = *bdict::parse(ben, pt);
            zend_hash_next_index_insert(intern->blist_data->_data, &bnode);
        } else if (ben[pt] == 'l') {
            zval bnode = *blist::parse(ben, pt);
            zend_hash_next_index_insert(intern->blist_data->_data, &bnode);
        } else if (ben[pt] >= '0' && ben[pt] <= '9') {
            zval bnode = *bstr::parse(ben, pt);
            zend_hash_next_index_insert(intern->blist_data->_data, &bnode);
        } else if (ben[pt] == 'i') {
            zval bnode = *bint::parse(ben, pt);
            zend_hash_next_index_insert(intern->blist_data->_data, &bnode);
        } else {
            zend_throw_exception(
                    zend_container::blist_ce,
                    "Error parsing blist",
                    1);
        }
    }
    ++pt;
    return zv;
}

std::string blist::encode() const {
    std::string result = "l";
    for(zend_hash_internal_pointer_reset(_data);
            zend_hash_has_more_elements(_data) == SUCCESS;
            zend_hash_move_forward(_data)) {
        zval *value = zend_hash_get_current_data(_data);
        std::string class_name = zend_container::bnode_object_get_class_name(value);

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

zval * blist::to_array(const bool include_meta) const {
    zval _zv;
    zval *zv = &_zv;
    array_init(zv);

    for(zend_hash_internal_pointer_reset(_data);
            zend_hash_has_more_elements(_data) == SUCCESS;
            zend_hash_move_forward(_data)) {
        zval *value = zend_hash_get_current_data(_data);
        std::string class_name = zend_container::bnode_object_get_class_name(value);
        if (class_name == "bdict") {
            bdict_object *bnode = zend_container::bdict_fetch_object(Z_OBJ_P(value));
            zval subarray = *bnode->bdict_data->to_array(include_meta);
            add_next_index_zval(zv, &subarray);
        } else if (class_name == "blist") {
            blist_object *bnode = zend_container::blist_fetch_object(Z_OBJ_P(value));
            zval subarray = *bnode->blist_data->to_array(include_meta);
            add_next_index_zval(zv, &subarray);
        } else if (class_name == "bstr") {
            bstr_object *bnode = zend_container::bstr_fetch_object(Z_OBJ_P(value));
            zval subarray = *bnode->bstr_data->to_array(include_meta);
            add_next_index_zval(zv, &subarray);
        } else if (class_name == "bint") {
            bint_object *bnode = zend_container::bint_fetch_object(Z_OBJ_P(value));
            zval subarray = *bnode->bint_data->to_array(include_meta);
            add_next_index_zval(zv, &subarray);
        }
    }

    if (include_meta) {
        char *_type = estrdup("_type");
        char *_type_data = estrdup("blist");
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