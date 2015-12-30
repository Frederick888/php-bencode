#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "php_bencode.h"
#include "binit.h"

/**** BITEM *****/
PHP_METHOD(bitem, __construct)
{
    bitem *bnode = NULL;
/*
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &maxGear) == FAILURE) {
        RETURN_NULL();
    }
*/
    bnode = new bitem();
    bitem_object *intern = Z_BITEM_OBJ_P(getThis());
    intern->bitem_data = bnode;
}
PHP_METHOD(bitem, get_type)
{
    std::string result;
    bitem_object *intern = Z_BITEM_OBJ_P(getThis());
    result = intern->bitem_data->get_type();
    RETURN_STRING(result.c_str());
}
PHP_METHOD(bitem, parse)
{
//    char *ben;
//    size_t ben_len;
//    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &ben, &ben_len) == FAILURE) {
//        RETURN_NULL();
//    }
//    std::string ben_str(ben);
//    zval *result = bitem::parse(ben_str);
//    RETURN_ARR(Z_ARRVAL_P(result));
    std::string ben = "d4:key15:item1e";
    RETURN_ZVAL(bitem::parse(ben), 1, 1);
}
static zend_function_entry bitem_methods[] = {
    PHP_ME(bitem, __construct,          NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(bitem, get_type,             NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bitem, parse,                NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    {NULL, NULL, NULL}
};

/**** BDICT *****/
PHP_METHOD(bdict, __construct)
{
    bdict *bnode = NULL;
    bnode = new bdict();
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    intern->bdict_data = bnode;
}
PHP_METHOD(bdict, get_type)
{
    std::string result;
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    result = intern->bdict_data->get_type();
    RETURN_STRING(result.c_str());
}
PHP_METHOD(bdict, get)
{
    char *key;
    size_t key_len = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &key_len) == FAILURE) {
            RETURN_NULL();
    }
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    std::string _key(key);
    RETURN_ZVAL(intern->bdict_data->get(_key), 1, 0);
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
    std::string _key(key);
    intern->bdict_data->set(_key, zv);
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
    std::string _key(key);
    RETURN_BOOL(intern->bdict_data->has(_key));
}
PHP_METHOD(bdict, del)
{
    char *key;
    size_t key_len = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &key_len) == FAILURE) {
            RETURN_NULL();
    }
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    std::string _key(key);
    RETURN_BOOL(intern->bdict_data->del(_key));
}
PHP_METHOD(bdict, to_array)
{
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    RETURN_ZVAL(intern->bdict_data->to_array(), 1, 1);
}
static zend_function_entry bdict_methods[] = {
    PHP_ME(bdict, __construct,          NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(bdict, get_type,             NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bdict, get,                  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bdict, set,                  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bdict, has,                  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bdict, del,                  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bdict, to_array,             NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

/**** BSTR *****/
PHP_METHOD(bstr, __construct)
{
    char *ben;
    size_t ben_len = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &ben, &ben_len) == FAILURE) {
            RETURN_NULL();
    }
    bstr *bnode = NULL;
    if (ben_len > 0) {
        std::string tmp(ben);
        bnode = new bstr(tmp);
    } else {
        bnode = new bstr();
    }
    bstr_object *intern = Z_BSTR_OBJ_P(getThis());
    intern->bstr_data = bnode;
}
PHP_METHOD(bstr, get_type)
{
    std::string result;
    bstr_object *intern = Z_BSTR_OBJ_P(getThis());
    result = intern->bstr_data->get_type();
    RETURN_STRING(result.c_str());
}
PHP_METHOD(bstr, get)
{
    bstr_object *intern = Z_BSTR_OBJ_P(getThis());
    std::string result = intern->bstr_data->get();
    RETURN_STRING(result.c_str());
}
PHP_METHOD(bstr, set)
{
    char *value;
    size_t value_len = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &value, &value_len) == FAILURE) {
        RETURN_FALSE;
    }
    std::string _value(value);
    bstr_object *intern = Z_BSTR_OBJ_P(getThis());
    intern->bstr_data->set(_value);
    RETURN_TRUE;
}
PHP_METHOD(bstr, length)
{
    bstr_object *intern = Z_BSTR_OBJ_P(getThis());
    size_t result = intern->bstr_data->length();
    RETURN_LONG(result);
}
PHP_METHOD(bstr, parse)
{
    char *ben;
    size_t ben_len = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &ben, &ben_len) == FAILURE) {
        RETURN_NULL();
    }
    if (!ben_len > 0) RETURN_NULL();
    std::string tmp(ben);
    size_t pt = 0;
    RETURN_ZVAL(bstr::parse(tmp, pt), 1, 1);
}
PHP_METHOD(bstr, to_array)
{
    bstr_object *intern = Z_BSTR_OBJ_P(getThis());
    RETURN_ZVAL(intern->bstr_data->to_array(), 1, 1);
}
PHP_METHOD(bstr, to_meta_array)
{
    bstr_object *intern = Z_BSTR_OBJ_P(getThis());
    RETURN_ZVAL(intern->bstr_data->to_meta_array(), 1, 1);
}
static zend_function_entry bstr_methods[] = {
    PHP_ME(bstr, __construct,           NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(bstr, get_type,              NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bstr, get,                   NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bstr, set,                   NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bstr, length,                NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bstr, parse,                 NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(bstr, to_array,              NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bstr, to_meta_array,         NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

/**** PHP ****/
PHP_MINIT_FUNCTION(bencode)
{
    BI_MINIT(bitem)
    BI_MINIT(bdict)
    BI_MINIT(bstr)
    return SUCCESS;
}

static zend_function_entry bencode_functions[] = {
    PHP_FE(bencode_world, NULL)
    {NULL, NULL, NULL}
};

zend_module_entry bencode_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_BENCODE_EXTNAME,
    bencode_functions,          //FUNCTIONS
    PHP_MINIT(bencode),         //PHP_MINIT
    NULL,                       //PHP_MSHUTDOWN(bencode),
    NULL,                       //PHP_RINIT(bencode),
    NULL,                       //PHP_RSHUTDOWN
    NULL,                       //PHP_MINFO
#if ZEND_MODULE_API_NO >= 20010901
    PHP_BENCODE_EXTVER,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_BENCODE
extern "C" {
ZEND_GET_MODULE(bencode)
}
#endif

PHP_FUNCTION(bencode_world)
{
//    zval _zv;
//    zval *zv = &_zv;
//    object_init_ex(zv, zend_container::bitem_ce);
//    bitem_object *intern = zend_container::bitem_fetch_object(Z_OBJ_P(zv));
//    intern->bitem_data = new bitem();
//    RETURN_ZVAL(zv, 1, 1);
//
//    zval _outer; zval *outer = &_outer; array_init(outer);
//    zval _inner; zval *inner = &_inner; array_init(inner);
//    std::string _key = "hello";
//    char *key = (char *)emalloc(_key.length());
//    strcpy(key, _key.c_str());
//    add_assoc_zval(outer, key, inner);
//    efree(key);
//    RETURN_ZVAL(outer, 0, 1);
    php_printf("SUCCESS: %d\n", SUCCESS);
    php_printf("FAILURE: %d\n", FAILURE);
}
