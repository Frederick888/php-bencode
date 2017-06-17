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
        zval *tmp = new zval();
        ZVAL_OBJ(tmp, zend_container::bnode_object_clone(zend_hash_get_current_data(that->_data)));
        zend_string *_str_index;
        zend_ulong _num_index;
        zend_hash_get_current_key(that->_data, &_str_index, &_num_index);
        zend_hash_index_add(_data, _num_index, tmp);
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
        return bitem::get_zval_bool(false);
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
    if (class_name == "bdict" || class_name == "blist" ||
            class_name == "bstr" || class_name == "bint") {
        clone_object = zend_container::bnode_object_clone(value);
    } else {
        return;
    }
    zval *tmp = new zval();
    ZVAL_OBJ(tmp, clone_object);
    zend_hash_next_index_insert(_data, tmp);
}

void blist::set(const size_t &key, zval *value) {
    std::string class_name = zend_container::bnode_object_get_class_name(value);
    zend_object *clone_object = NULL;
    if (class_name == "bdict" || class_name == "blist" ||
            class_name == "bstr" || class_name == "bint") {
        clone_object = zend_container::bnode_object_clone(value);
    } else {
        return;
    }
    zval *tmp = new zval();
    ZVAL_OBJ(tmp, clone_object);
    if (zend_hash_index_exists(_data, key)) {
        zend_hash_index_update(_data, key, tmp);
    } else {
        zend_hash_next_index_insert(_data, tmp);
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
                zval *copy_next = new zval();
                ZVAL_OBJ(copy_next, zend_container::bnode_object_clone(zend_hash_index_find(_data, i + 1)));
                zend_hash_index_update(_data, i, copy_next);
            }
            zend_hash_index_del(_data, constant_count);
        }
        return true;
    } else {
        return false;
    }
}

zval * blist::get_path(const std::string &key, size_t &pt) const {
    std::string current_key = bitem::get_current_key(key, pt);
    if (!bitem::is_ull(current_key)) {
        return bitem::get_zval_bool(false);
    }
    size_t current_key_long = std::stoull(current_key);

    if (!zend_hash_index_exists(_data, current_key_long)) {
        return bitem::get_zval_bool(false);
    }
    if (pt >= key.length()) {
        return zend_hash_index_find(_data, current_key_long);
    } else {
        zval *subnode = zend_hash_index_find(_data, current_key_long);
        std::string class_name = zend_container::bnode_object_get_class_name(subnode);
        if (class_name == "bdict") {
            return zend_container::bdict_fetch_object(Z_OBJ_P(subnode))->bdict_data->get_path(key, pt);
        } else if (class_name == "blist") {
            return zend_container::blist_fetch_object(Z_OBJ_P(subnode))->blist_data->get_path(key, pt);
        } else {
            return bitem::get_zval_bool(false);
        }
    }
}

void blist::set_path(const std::string &key, size_t &pt, zval *value) {
    std::string class_name = zend_container::bnode_object_get_class_name(value);
    if (!(class_name == "bdict" || class_name == "blist" ||
            class_name == "bstr" || class_name == "bint")) {
        bitem::throw_general_exception("Unsupported node given");
        return;
    }
    std::string current_key = bitem::get_current_key(key, pt);
    if (!bitem::is_ull(current_key)) {
        bitem::throw_general_exception("Invalid key for blist, only positive integer is allowed");
        return;
    }
    size_t current_key_long = std::stoull(current_key);
    if (zend_hash_index_exists(_data, current_key_long)) {
        zval *subnode = zend_hash_index_find(_data, current_key_long);
        std::string sub_class_name = zend_container::bnode_object_get_class_name(subnode);
        if (sub_class_name == "bstr" || sub_class_name == "bint") {
            if (pt >= key.length()) {
                zend_object *clone_object = zend_container::bnode_object_clone(value);
                zval *tmp = new zval();
                ZVAL_OBJ(tmp, clone_object);
                zend_hash_index_update(_data, current_key_long, tmp);
            } else {
                bitem::throw_general_exception("bstr and bint cannot have sub-nodes");
                return;
            }
        } else if (sub_class_name == "bdict") {
            zend_container::bdict_fetch_object(Z_OBJ_P(subnode))->bdict_data->set_path(key, pt, value);
        } else if (sub_class_name == "blist") {
            zend_container::blist_fetch_object(Z_OBJ_P(subnode))->blist_data->set_path(key, pt, value);
        }
    } else {
        if (pt >= key.length()) {
            zend_object *clone_object = zend_container::bnode_object_clone(value);
            zval *tmp = new zval();
            ZVAL_OBJ(tmp, clone_object);
            zend_hash_next_index_insert(_data, tmp);
        } else {
            size_t backup_pt = pt;
            std::string next_key = bitem::get_current_key(key, pt);
            pt = backup_pt;
            if (bitem::is_ull(next_key)) {
                zend_object *zo = zend_container::blist_object_new(zend_container::blist_ce);
                zend_container::blist_fetch_object(zo)->blist_data = new blist();
                zend_container::blist_fetch_object(zo)->blist_data->set_path(key, pt, value);
                zval *zv = new zval();
                ZVAL_OBJ(zv, zo);
                zend_hash_next_index_insert(_data, zv);
            } else {
                zend_object *zo = zend_container::bdict_object_new(zend_container::bdict_ce);
                zend_container::bdict_fetch_object(zo)->bdict_data = new bdict();
                zend_container::bdict_fetch_object(zo)->bdict_data->set_path(key, pt, value);
                zval *zv = new zval();
                ZVAL_OBJ(zv, zo);
                zend_hash_next_index_insert(_data, zv);
            }
        }
    }
}

bool blist::del_path(const std::string &key, size_t &pt) {
    std::string current_key = bitem::get_current_key(key, pt);
    if (!bitem::is_ull(current_key)) {
        return false;
    }
    size_t current_key_long = std::stoull(current_key);

    if (!zend_hash_index_exists(_data, current_key_long)) {
        return false;
    }
    if (pt >= key.length()) {
        return del(current_key_long);
    } else {
        zval *subnode = zend_hash_index_find(_data, current_key_long);
        std::string class_name = zend_container::bnode_object_get_class_name(subnode);
        if (class_name == "bdict") {
            return zend_container::bdict_fetch_object(Z_OBJ_P(subnode))->bdict_data->del_path(key, pt);
        } else if (class_name == "blist") {
            return zend_container::blist_fetch_object(Z_OBJ_P(subnode))->blist_data->del_path(key, pt);
        } else {
            return false;
        }
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
        return bitem::throw_general_exception("Error parsing blist");
    ++pt;
    zval *zv = new zval();
    zend_object *zo = zend_container::blist_object_new(zend_container::blist_ce);
    ZVAL_OBJ(zv, zo);
    blist_object *intern = zend_container::blist_fetch_object(Z_OBJ_P(zv));
    intern->blist_data = new blist();

    while (ben[pt] != 'e') {
        if (ben[pt] == 'd') {
            zval *bnode = bdict::parse(ben, pt);
            zend_hash_next_index_insert(intern->blist_data->_data, bnode);
        } else if (ben[pt] == 'l') {
            zval *bnode = blist::parse(ben, pt);
            zend_hash_next_index_insert(intern->blist_data->_data, bnode);
        } else if (isdigit(ben[pt])) {
            zval *bnode = bstr::parse(ben, pt);
            zend_hash_next_index_insert(intern->blist_data->_data, bnode);
        } else if (ben[pt] == 'i') {
            zval *bnode = bint::parse(ben, pt);
            zend_hash_next_index_insert(intern->blist_data->_data, bnode);
        } else {
            return bitem::throw_general_exception("Error parsing blist");
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
    zval *zv = new zval();
    array_init(zv);

    for(zend_hash_internal_pointer_reset(_data);
            zend_hash_has_more_elements(_data) == SUCCESS;
            zend_hash_move_forward(_data)) {
        zval *value = zend_hash_get_current_data(_data);
        std::string class_name = zend_container::bnode_object_get_class_name(value);
        if (class_name == "bdict") {
            bdict_object *bnode = zend_container::bdict_fetch_object(Z_OBJ_P(value));
            zval *subarray = bnode->bdict_data->to_array(include_meta);
            add_next_index_zval(zv, subarray);
        } else if (class_name == "blist") {
            blist_object *bnode = zend_container::blist_fetch_object(Z_OBJ_P(value));
            zval *subarray = bnode->blist_data->to_array(include_meta);
            add_next_index_zval(zv, subarray);
        } else if (class_name == "bstr") {
            bstr_object *bnode = zend_container::bstr_fetch_object(Z_OBJ_P(value));
            zval *subarray = bnode->bstr_data->to_array(include_meta);
            add_next_index_zval(zv, subarray);
        } else if (class_name == "bint") {
            bint_object *bnode = zend_container::bint_fetch_object(Z_OBJ_P(value));
            zval *subarray = bnode->bint_data->to_array(include_meta);
            add_next_index_zval(zv, subarray);
        }
    }

    if (include_meta) {
        char *_type = estrdup("_type");
        char *_type_data = estrdup("blist");
        char *_length = estrdup("_length");
        char *_data = estrdup("_data");
        zval *zv_outer = new zval();
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

zval * blist::search(const std::string &needle, const long &mode, const std::string path) const {
    if (mode < 0 || mode > 1)
        bitem::throw_general_exception("Illegal search mode");
    bool modek = false, modev = false;
    if (mode == 0)
        modek = true;
    else
        modev = true;

    zval *zv = new zval();
    array_init(zv);

    for(zend_hash_internal_pointer_reset(_data);
            zend_hash_has_more_elements(_data) == SUCCESS;
            zend_hash_move_forward(_data)) {
        zend_string *_str_index;
        zend_ulong num_index;
        zend_hash_get_current_key(_data, &_str_index, &num_index);
        zval *value = zend_hash_get_current_data(_data);
        std::string class_name = zend_container::bnode_object_get_class_name(value);
        std::string current_path = (path == "" ? "" : path + "/") + std::to_string(num_index);

        if (class_name == "bdict") {
            zval *next_result = (zend_container::bdict_fetch_object(Z_OBJ_P(value)))->bdict_data->search(needle, mode, current_path);
            zend_hash_append_strings(Z_ARRVAL_P(zv), Z_ARRVAL_P(next_result));
        } else if (class_name == "blist") {
            zval *next_result = (zend_container::blist_fetch_object(Z_OBJ_P(value)))->blist_data->search(needle, mode, current_path);
            zend_hash_append_strings(Z_ARRVAL_P(zv), Z_ARRVAL_P(next_result));
        } else if (modev && class_name == "bstr") {
            if ((zend_container::bstr_fetch_object(Z_OBJ_P(value)))->bstr_data->_value.find(needle) != std::string::npos)
                add_next_index_stringl(zv, current_path.c_str(), current_path.length());
        } else if (modev && bitem::is_ll(needle) && class_name == "bint") {
            if ((zend_container::bint_fetch_object(Z_OBJ_P(value)))->bint_data->_value == std::stoll(needle))
                add_next_index_stringl(zv, current_path.c_str(), current_path.length());
        }
    }

    return zv;
}
