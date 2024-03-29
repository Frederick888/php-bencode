#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "binit.h"
#include "php_bencode.h"

#define CALL_AND_HANDLE(expr)                                                                                                \
    try {                                                                                                                    \
        zval zv = expr;                                                                                                      \
        RETURN_ZVAL(&zv, 1, 1);                                                                                              \
    } catch (const std::invalid_argument &ia) {                                                                              \
        zend_throw_exception(NULL, ("Invalid argument: " + std::string(ia.what())).c_str(), BENCODE_ERROR_INVALID_ARGUMENT); \
        RETURN_NULL();                                                                                                       \
    } catch (const std::out_of_range &oor) {                                                                                 \
        zend_throw_exception(NULL, ("Out of Range error: " + std::string(oor.what())).c_str(), BENCODE_ERROR_OUT_OF_RANGE);  \
        RETURN_NULL();                                                                                                       \
    } catch (const std::exception &e) {                                                                                      \
        zend_throw_exception(NULL, ("Undefined error: " + std::string(e.what())).c_str(), BENCODE_ERROR_UNDEFINED);          \
        RETURN_NULL();                                                                                                       \
    } catch (...) {                                                                                                          \
        zend_throw_exception(NULL, "Unknown error", BENCODE_ERROR_UNKNOWN);                                                  \
        RETURN_NULL();                                                                                                       \
    }

/**** BITEM *****/
PHP_METHOD(bitem, __construct)
{
    // block instantiating bitem
    // TODO: find a programmatic way to do this
    RETURN_NULL();
}
PHP_METHOD(bitem, parse)
{
    char *ben;
    size_t ben_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &ben, &ben_len) == FAILURE) {
        RETURN_NULL();
    }
    std::string ben_str(ben, ben_len);
    CALL_AND_HANDLE(bitem::parse(ben_str));
}
PHP_METHOD(bitem, load)
{
    char *file_path;
    size_t file_path_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &file_path, &file_path_len) == FAILURE) {
        RETURN_NULL();
    }
    std::string file_path_str(file_path, file_path_len);
    CALL_AND_HANDLE(bitem::load(file_path_str));
}
PHP_METHOD(bitem, save)
{
    char *file_path;
    size_t file_path_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &file_path, &file_path_len) == FAILURE) {
        RETURN_NULL();
    }
    std::string file_path_str(file_path, file_path_len);
    std::string class_name = zend_container::bnode_object_get_class_name(getThis());
    if (class_name == "bdict") {
        bdict_object *intern = Z_BDICT_OBJ_P(getThis());
        intern->bnode_data->save(file_path_str);
        RETURN_TRUE;
    } else if (class_name == "blist") {
        blist_object *intern = Z_BLIST_OBJ_P(getThis());
        intern->bnode_data->save(file_path_str);
        RETURN_TRUE;
    } else if (class_name == "bstr") {
        bstr_object *intern = Z_BSTR_OBJ_P(getThis());
        intern->bnode_data->save(file_path_str);
        RETURN_TRUE;
    } else if (class_name == "bint") {
        bint_object *intern = Z_BINT_OBJ_P(getThis());
        intern->bnode_data->save(file_path_str);
        RETURN_TRUE;
    }
    RETURN_FALSE;
}
static zend_function_entry bitem_methods[] = {
    /* clang-format off */
    PHP_ME(bitem, __construct,          arginfo_void, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(bitem, parse,                arginfo_void, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(bitem, load,                 arginfo_void, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(bitem, save,                 arginfo_void, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
    /* clang-format on */
};

/**** BDICT *****/
PHP_METHOD(bdict, __construct)
{
    bdict *bnode = NULL;
    bnode = new bdict();
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    intern->bnode_data = bnode;
}
PHP_METHOD(bdict, get_type)
{
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    std::string result = intern->bnode_data->get_type();
    RETURN_STRINGL(result.c_str(), result.length());
}
PHP_METHOD(bdict, get)
{
    char *key;
    size_t key_len = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &key_len) == FAILURE) {
        RETURN_NULL();
    }
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    std::string _key(key, key_len);
    zval zv = intern->bnode_data->get(_key);
    RETURN_ZVAL(&zv, 1, 1);
}
PHP_METHOD(bdict, get_path)
{
    char *key;
    size_t key_len = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &key_len) == FAILURE) {
        RETURN_NULL();
    }
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    std::string _key(key, key_len);
    size_t pt = 0;
    zval zv = intern->bnode_data->get_path(_key, pt);
    RETURN_ZVAL(&zv, 1, 1);
}
PHP_METHOD(bdict, get_copy)
{
    char *key;
    size_t key_len = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &key_len) == FAILURE) {
        RETURN_NULL();
    }
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    std::string _key(key, key_len);
    zval zv = intern->bnode_data->get(_key);
    RETURN_OBJ(zend_container::bnode_object_clone(VAL_OR_OBJ2(zv)));
}
PHP_METHOD(bdict, get_path_copy)
{
    char *key;
    size_t key_len = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &key_len) == FAILURE) {
        RETURN_NULL();
    }
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    std::string _key(key, key_len);
    size_t pt = 0;
    zval zv = intern->bnode_data->get_path(_key, pt);
    RETURN_OBJ(zend_container::bnode_object_clone(VAL_OR_OBJ2(zv)));
}
PHP_METHOD(bdict, set)
{
    char *key;
    size_t key_len = 0;
    zval *zv;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "so", &key, &key_len, &zv) == FAILURE) {
        RETURN_FALSE;
    }
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    std::string _key(key, key_len);
    intern->bnode_data->set(_key, zv);
    RETURN_TRUE;
}
PHP_METHOD(bdict, set_path)
{
    char *key;
    size_t key_len = 0;
    zval *zv;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "so", &key, &key_len, &zv) == FAILURE) {
        RETURN_FALSE;
    }
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    size_t pt = 0;
    std::string _key(key, key_len);
    intern->bnode_data->set_path(_key, pt, zv);
    RETURN_TRUE;
}
PHP_METHOD(bdict, has)
{
    char *key;
    size_t key_len = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &key_len) == FAILURE) {
        RETURN_NULL();
    }
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    std::string _key(key, key_len);
    RETURN_BOOL(intern->bnode_data->has(_key));
}
PHP_METHOD(bdict, del)
{
    char *key;
    size_t key_len = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &key_len) == FAILURE) {
        RETURN_NULL();
    }
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    std::string _key(key, key_len);
    RETURN_BOOL(intern->bnode_data->del(_key));
}
PHP_METHOD(bdict, del_path)
{
    char *key;
    size_t key_len = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &key_len) == FAILURE) {
        RETURN_NULL();
    }
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    std::string _key(key, key_len);
    size_t pt = 0;
    RETURN_BOOL(intern->bnode_data->del_path(_key, pt));
}
PHP_METHOD(bdict, length)
{
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    size_t result = intern->bnode_data->length();
    RETURN_LONG(result);
}
PHP_METHOD(bdict, count)
{
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    size_t result = intern->bnode_data->count();
    RETURN_LONG(result);
}
PHP_METHOD(bdict, parse)
{
    char *ben;
    size_t ben_len = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &ben, &ben_len) == FAILURE) {
        RETURN_NULL();
    }
    if (!ben_len)
        RETURN_NULL();
    std::string tmp(ben, ben_len);
    size_t pt = 0;
    CALL_AND_HANDLE(bdict::parse(tmp, pt));
}
PHP_METHOD(bdict, encode)
{
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    std::string result = intern->bnode_data->encode();
    RETURN_STRINGL(result.c_str(), result.length());
}
PHP_METHOD(bdict, search)
{
    char *needle;
    size_t needle_len = 0;
    long mode;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sl", &needle, &needle_len, &mode) == FAILURE) {
        RETURN_NULL();
    }
    std::string tmp(needle, needle_len);
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    CALL_AND_HANDLE(intern->bnode_data->search(tmp, mode, ""));
}
PHP_METHOD(bdict, to_array)
{
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    zval zv = intern->bnode_data->to_array(false);
    RETURN_ZVAL(&zv, 1, 1);
}
PHP_METHOD(bdict, to_meta_array)
{
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    zval zv = intern->bnode_data->to_array(true);
    RETURN_ZVAL(&zv, 1, 1);
}
PHP_METHOD(bdict, __toString)
{
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    std::string result = intern->bnode_data->encode();
    RETURN_STRINGL(result.c_str(), result.length());
}
static zend_function_entry bdict_methods[] = {
    /* clang-format off */
    PHP_ME(bdict, __construct,          arginfo_void, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(bdict, get_type,             arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(bdict, get,                  arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(bdict, get_path,             arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(bdict, get_copy,             arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(bdict, get_path_copy,        arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(bdict, set,                  arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(bdict, set_path,             arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(bdict, has,                  arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(bdict, del,                  arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(bdict, del_path,             arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(bdict, length,               arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(bdict, count,                arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(bdict, parse,                arginfo_void, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(bdict, encode,               arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(bitem, save,                 arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(bdict, search,               arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(bdict, to_array,             arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(bdict, to_meta_array,        arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(bdict, __toString,           arginfo_void, ZEND_ACC_PUBLIC | ZEND_ACC_HAS_RETURN_TYPE)
    {NULL, NULL, NULL}
    /* clang-format on */
};

/**** BLIST *****/
PHP_METHOD(blist, __construct)
{
    blist *bnode = NULL;
    bnode = new blist();
    blist_object *intern = Z_BLIST_OBJ_P(getThis());
    intern->bnode_data = bnode;
}
PHP_METHOD(blist, get_type)
{
    blist_object *intern = Z_BLIST_OBJ_P(getThis());
    std::string result = intern->bnode_data->get_type();
    RETURN_STRINGL(result.c_str(), result.length());
}
PHP_METHOD(blist, get)
{
    long key;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &key) == FAILURE) {
        RETURN_NULL();
    }
    if (key < 0) {
        RETURN_NULL();
    }
    blist_object *intern = Z_BLIST_OBJ_P(getThis());
    zval zv = intern->bnode_data->get(key);
    RETURN_ZVAL(&zv, 1, 1);
}
PHP_METHOD(blist, get_path)
{
    char *key;
    size_t key_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &key_len) == FAILURE) {
        RETURN_NULL();
    }
    blist_object *intern = Z_BLIST_OBJ_P(getThis());
    std::string _key(key, key_len);
    size_t pt = 0;
    zval zv = intern->bnode_data->get_path(_key, pt);
    RETURN_ZVAL(&zv, 1, 1);
}
PHP_METHOD(blist, get_copy)
{
    long key;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &key) == FAILURE) {
        RETURN_NULL();
    }
    if (key < 0) {
        RETURN_NULL();
    }
    blist_object *intern = Z_BLIST_OBJ_P(getThis());
    zval zv = intern->bnode_data->get(key);
    RETURN_OBJ(zend_container::bnode_object_clone(VAL_OR_OBJ2(zv)));
}
PHP_METHOD(blist, get_path_copy)
{
    char *key;
    size_t key_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &key_len) == FAILURE) {
        RETURN_NULL();
    }
    blist_object *intern = Z_BLIST_OBJ_P(getThis());
    std::string _key(key, key_len);
    size_t pt = 0;
    zval zv = intern->bnode_data->get_path(_key, pt);
    RETURN_OBJ(zend_container::bnode_object_clone(VAL_OR_OBJ2(zv)));
}
PHP_METHOD(blist, add)
{
    zval *zv;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o", &zv) == FAILURE) {
        RETURN_FALSE;
    }
    blist_object *intern = Z_BLIST_OBJ_P(getThis());
    intern->bnode_data->add(zv);
    RETURN_TRUE;
}
PHP_METHOD(blist, set_path)
{
    char *key;
    long key_len;
    zval *zv;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lo", &key, &key_len, &zv) == FAILURE) {
        RETURN_FALSE;
    }
    blist_object *intern = Z_BLIST_OBJ_P(getThis());
    std::string _key(key, key_len);
    size_t pt = 0;
    intern->bnode_data->set_path(_key, pt, zv);
    RETURN_TRUE;
}
PHP_METHOD(blist, set)
{
    long key;
    zval *zv;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lo", &key, &zv) == FAILURE) {
        RETURN_FALSE;
    }
    if (key < 0) {
        RETURN_FALSE;
    }
    blist_object *intern = Z_BLIST_OBJ_P(getThis());
    intern->bnode_data->set(key, zv);
    RETURN_TRUE;
}
PHP_METHOD(blist, has)
{
    long key;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &key) == FAILURE) {
        RETURN_NULL();
    }
    if (key < 0) {
        RETURN_FALSE;
    }
    blist_object *intern = Z_BLIST_OBJ_P(getThis());
    RETURN_BOOL(intern->bnode_data->has(key));
}
PHP_METHOD(blist, del)
{
    long key;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &key) == FAILURE) {
        RETURN_NULL();
    }
    blist_object *intern = Z_BLIST_OBJ_P(getThis());
    RETURN_BOOL(intern->bnode_data->del(key));
}
PHP_METHOD(blist, del_path)
{
    char *key;
    size_t key_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &key_len) == FAILURE) {
        RETURN_NULL();
    }
    blist_object *intern = Z_BLIST_OBJ_P(getThis());
    std::string _key(key, key_len);
    size_t pt = 0;
    RETURN_BOOL(intern->bnode_data->del_path(_key, pt));
}
PHP_METHOD(blist, length)
{
    blist_object *intern = Z_BLIST_OBJ_P(getThis());
    size_t result = intern->bnode_data->length();
    RETURN_LONG(result);
}
PHP_METHOD(blist, count)
{
    blist_object *intern = Z_BLIST_OBJ_P(getThis());
    size_t result = intern->bnode_data->count();
    RETURN_LONG(result);
}
PHP_METHOD(blist, parse)
{
    char *ben;
    size_t ben_len = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &ben, &ben_len) == FAILURE) {
        RETURN_NULL();
    }
    if (!ben_len)
        RETURN_NULL();
    std::string tmp(ben, ben_len);
    size_t pt = 0;
    CALL_AND_HANDLE(blist::parse(tmp, pt));
}
PHP_METHOD(blist, encode)
{
    blist_object *intern = Z_BLIST_OBJ_P(getThis());
    std::string result = intern->bnode_data->encode();
    RETURN_STRINGL(result.c_str(), result.length());
}
PHP_METHOD(blist, search)
{
    char *needle;
    size_t needle_len = 0;
    long mode;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sl", &needle, &needle_len, &mode) == FAILURE) {
        RETURN_NULL();
    }
    std::string tmp(needle, needle_len);
    blist_object *intern = Z_BLIST_OBJ_P(getThis());
    CALL_AND_HANDLE(intern->bnode_data->search(tmp, mode, ""));
}
PHP_METHOD(blist, to_array)
{
    blist_object *intern = Z_BLIST_OBJ_P(getThis());
    zval zv = intern->bnode_data->to_array(false);
    RETURN_ZVAL(&zv, 1, 1);
}
PHP_METHOD(blist, to_meta_array)
{
    blist_object *intern = Z_BLIST_OBJ_P(getThis());
    zval zv = intern->bnode_data->to_array(true);
    RETURN_ZVAL(&zv, 1, 1);
}
PHP_METHOD(blist, __toString)
{
    blist_object *intern = Z_BLIST_OBJ_P(getThis());
    std::string result = intern->bnode_data->encode();
    RETURN_STRINGL(result.c_str(), result.length());
}
static zend_function_entry blist_methods[] = {
    /* clang-format off */
    PHP_ME(blist, __construct,          arginfo_void, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(blist, get_type,             arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(blist, get,                  arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(blist, get_path,             arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(blist, get_copy,             arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(blist, get_path_copy,        arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(blist, add,                  arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(blist, set,                  arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(blist, set_path,             arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(blist, has,                  arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(blist, del,                  arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(blist, del_path,             arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(blist, length,               arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(blist, count,                arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(blist, parse,                arginfo_void, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(blist, encode,               arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(bitem, save,                 arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(blist, search,               arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(blist, to_array,             arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(blist, to_meta_array,        arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(blist, __toString,           arginfo_void, ZEND_ACC_PUBLIC | ZEND_ACC_HAS_RETURN_TYPE)
    {NULL, NULL, NULL}
    /* clang-format on */
};

/**** BSTR *****/
PHP_METHOD(bstr, __construct)
{
    char *value;
    size_t value_len = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &value, &value_len) == FAILURE) {
        RETURN_NULL();
    }
    bstr *bnode = NULL;
    if (value_len > 0) {
        std::string tmp(value, value_len);
        bnode = new bstr(tmp);
    } else {
        bnode = new bstr();
    }
    bstr_object *intern = Z_BSTR_OBJ_P(getThis());
    intern->bnode_data = bnode;
}
PHP_METHOD(bstr, get_type)
{
    bstr_object *intern = Z_BSTR_OBJ_P(getThis());
    std::string result = intern->bnode_data->get_type();
    RETURN_STRINGL(result.c_str(), result.length());
}
PHP_METHOD(bstr, get)
{
    bstr_object *intern = Z_BSTR_OBJ_P(getThis());
    std::string result = intern->bnode_data->get();
    RETURN_STRINGL(result.c_str(), result.length());
}
PHP_METHOD(bstr, set)
{
    char *value;
    size_t value_len = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    std::string _value(value, value_len);
    bstr_object *intern = Z_BSTR_OBJ_P(getThis());
    intern->bnode_data->set(_value);
    RETURN_TRUE;
}
PHP_METHOD(bstr, length)
{
    bstr_object *intern = Z_BSTR_OBJ_P(getThis());
    size_t result = intern->bnode_data->length();
    RETURN_LONG(result);
}
PHP_METHOD(bstr, parse)
{
    char *ben;
    size_t ben_len = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &ben, &ben_len) == FAILURE) {
        RETURN_NULL();
    }
    if (!ben_len)
        RETURN_NULL();
    std::string tmp(ben);
    size_t pt = 0;
    CALL_AND_HANDLE(bstr::parse(tmp, pt));
}
PHP_METHOD(bstr, encode)
{
    bstr_object *intern = Z_BSTR_OBJ_P(getThis());
    std::string result = intern->bnode_data->encode();
    RETURN_STRINGL(result.c_str(), result.length());
}
PHP_METHOD(bstr, search)
{
    RETURN_NULL();
}
PHP_METHOD(bstr, to_array)
{
    bstr_object *intern = Z_BSTR_OBJ_P(getThis());
    zval zv = intern->bnode_data->to_array(false);
    RETURN_ZVAL(&zv, 1, 1);
}
PHP_METHOD(bstr, to_meta_array)
{
    bstr_object *intern = Z_BSTR_OBJ_P(getThis());
    zval zv = intern->bnode_data->to_array(true);
    RETURN_ZVAL(&zv, 1, 1);
}
PHP_METHOD(bstr, __toString)
{
    bstr_object *intern = Z_BSTR_OBJ_P(getThis());
    std::string result = intern->bnode_data->encode();
    RETURN_STRINGL(result.c_str(), result.length());
}
static zend_function_entry bstr_methods[] = {
    /* clang-format off */
    PHP_ME(bstr, __construct,           arginfo_void, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(bstr, get_type,              arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(bstr, get,                   arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(bstr, set,                   arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(bstr, length,                arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(bstr, parse,                 arginfo_void, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(bstr, encode,                arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(bitem, save,                 arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(bstr, to_array,              arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(bstr, to_meta_array,         arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(bstr, __toString,            arginfo_void, ZEND_ACC_PUBLIC | ZEND_ACC_HAS_RETURN_TYPE)
    {NULL, NULL, NULL}
    /* clang-format on */
};

/**** BINT *****/
PHP_METHOD(bint, __construct)
{
    long value;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l", &value) == FAILURE) {
        RETURN_NULL();
    }
    bint *bnode = NULL;
    if (ZEND_NUM_ARGS()) {
        bnode = new bint(value);
    } else {
        bnode = new bint();
    }
    bint_object *intern = Z_BINT_OBJ_P(getThis());
    intern->bnode_data = bnode;
}
PHP_METHOD(bint, get_type)
{
    bint_object *intern = Z_BINT_OBJ_P(getThis());
    std::string result = intern->bnode_data->get_type();
    RETURN_STRINGL(result.c_str(), result.length());
}
PHP_METHOD(bint, get)
{
    bint_object *intern = Z_BINT_OBJ_P(getThis());
    long result = intern->bnode_data->get();
    RETURN_LONG(result);
}
PHP_METHOD(bint, set)
{
    long value;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &value) == FAILURE) {
        RETURN_FALSE;
    }
    bint_object *intern = Z_BINT_OBJ_P(getThis());
    intern->bnode_data->set(value);
    RETURN_TRUE;
}
PHP_METHOD(bint, length)
{
    bint_object *intern = Z_BINT_OBJ_P(getThis());
    size_t result = intern->bnode_data->length();
    RETURN_LONG(result);
}
PHP_METHOD(bint, parse)
{
    char *ben;
    size_t ben_len = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &ben, &ben_len) == FAILURE) {
        RETURN_NULL();
    }
    if (!ben_len)
        RETURN_NULL();
    std::string tmp(ben, ben_len);
    size_t pt = 0;
    CALL_AND_HANDLE(bint::parse(tmp, pt));
}
PHP_METHOD(bint, encode)
{
    bint_object *intern = Z_BINT_OBJ_P(getThis());
    std::string result = intern->bnode_data->encode();
    RETURN_STRINGL(result.c_str(), result.length());
}
PHP_METHOD(bint, search)
{
    RETURN_NULL();
}
PHP_METHOD(bint, to_array)
{
    bint_object *intern = Z_BINT_OBJ_P(getThis());
    zval zv = intern->bnode_data->to_array(false);
    RETURN_ZVAL(&zv, 1, 1);
}
PHP_METHOD(bint, to_meta_array)
{
    bint_object *intern = Z_BINT_OBJ_P(getThis());
    zval zv = intern->bnode_data->to_array(true);
    RETURN_ZVAL(&zv, 1, 1);
}
PHP_METHOD(bint, __toString)
{
    bint_object *intern = Z_BINT_OBJ_P(getThis());
    std::string result = intern->bnode_data->encode();
    RETURN_STRINGL(result.c_str(), result.length());
}
static zend_function_entry bint_methods[] = {
    /* clang-format off */
    PHP_ME(bint, __construct,           arginfo_void, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(bint, get_type,              arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(bint, get,                   arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(bint, set,                   arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(bint, length,                arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(bint, parse,                 arginfo_void, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(bint, encode,                arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(bitem, save,                 arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(bint, to_array,              arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(bint, to_meta_array,         arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(bint, __toString,            arginfo_void, ZEND_ACC_PUBLIC | ZEND_ACC_HAS_RETURN_TYPE)
    {NULL, NULL, NULL}
    /* clang-format on */
};

/**** PHP ****/
PHP_INI_BEGIN()
PHP_INI_ENTRY("bencode.namespace", "0", PHP_INI_SYSTEM, NULL)
PHP_INI_END()

PHP_MINIT_FUNCTION(bencode)
{
    REGISTER_INI_ENTRIES();

    char *ini_ns_key = estrdup("bencode.namespace");
    zend_bool ini_ns = zend_ini_long(ini_ns_key, strlen(ini_ns_key), 0);
    efree(ini_ns_key);
    do {
        zend_class_entry ce;
        if (ini_ns)
            INIT_CLASS_ENTRY(ce, "bencode\\bitem", bitem_methods)
        else
            INIT_CLASS_ENTRY(ce, "bitem", bitem_methods);
        ce.ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;
        ce.ce_flags |= ZEND_ACC_IMPLICIT_ABSTRACT_CLASS;
        zend_container::bitem_ce = zend_register_internal_class(&ce TSRMLS_CC);
        memcpy(&zend_container::bitem_object_handlers,
               zend_get_std_object_handlers(), sizeof(zend_object_handlers));
        zend_container::bitem_object_handlers.offset = XtOffsetOf(bitem_object, std);
    } while (0);
    BI_MINIT(bdict)
    BI_MINIT(blist)
    BI_MINIT(bstr)
    BI_MINIT(bint)
    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(bencode)
{
    UNREGISTER_INI_ENTRIES();
    return SUCCESS;
}

zend_module_entry bencode_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_BENCODE_EXTNAME,
    NULL,                   //FUNCTIONS
    PHP_MINIT(bencode),     //PHP_MINIT
    PHP_MSHUTDOWN(bencode), //PHP_MSHUTDOWN(bencode),
    NULL,                   //PHP_RINIT(bencode),
    NULL,                   //PHP_RSHUTDOWN
    NULL,                   //PHP_MINFO
#if ZEND_MODULE_API_NO >= 20010901
    PHP_BENCODE_EXTVER,
#endif
    STANDARD_MODULE_PROPERTIES};

#ifdef COMPILE_DL_BENCODE
extern "C" {
ZEND_GET_MODULE(bencode)
}
#endif
