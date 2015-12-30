#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "php_bencode.h"
#include "bdict.h"

#define Z_BDICT_OBJ_P(zv) php_bdict_object_fetch_object(Z_OBJ_P(zv))

zend_object_handlers bdict_object_handlers;

typedef struct _bdict_object {
    BDict *bdict_data;
    zend_object std;
} bdict_object;

zend_class_entry *bdict_ce;

static void bdict_free_storage(zend_object *object TSRMLS_DC)
{
    //bdict_object *intern = (bdict_object *)object;
    //zend_object_std_dtor(&intern->std TSRMLS_CC);
    zend_object_std_dtor(object TSRMLS_CC);
    //delete intern->bdict_data;
}

zend_object * bdict_object_new(zend_class_entry *ce TSRMLS_DC)
{
    bdict_object *intern = (bdict_object *)ecalloc(1,
            sizeof(bdict_object) +
            zend_object_properties_size(ce));

    zend_object_std_init(&intern->std, ce TSRMLS_CC);
    object_properties_init(&intern->std, ce);
    intern->std.handlers = &bdict_object_handlers;

    return &intern->std;
}

static inline bdict_object * php_bdict_object_fetch_object(zend_object *obj)
{
    return (bdict_object *)((char *)obj - XtOffsetOf(bdict_object, std));
}

PHP_METHOD(BDict, __construct)
{
    long maxGear;
    BDict *bdict = NULL;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &maxGear) == FAILURE) {
        RETURN_NULL();
    }

    bdict = new BDict(maxGear);
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    intern->bdict_data = bdict;
}
PHP_METHOD(BDict, shift)
{
    long gear;
    BDict *bdict = NULL;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &gear) == FAILURE) {
        RETURN_NULL();
    }

    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    bdict = intern->bdict_data;
    if (bdict != NULL) {
        bdict->shift(gear);
    }
}
PHP_METHOD(BDict, accelerate)
{
    BDict *bdict = NULL;
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    bdict = intern->bdict_data;
    if (bdict != NULL) {
        bdict->accelerate();
    }
}
PHP_METHOD(BDict, brake)
{
    BDict *bdict = NULL;
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    bdict = intern->bdict_data;
    if (bdict != NULL) {
        bdict->brake();
    }
}
PHP_METHOD(BDict, getCurrentSpeed)
{
    BDict *bdict = NULL;
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    bdict = intern->bdict_data;
    if (bdict != NULL) {
        RETURN_LONG(bdict->getCurrentSpeed());
    }
    RETURN_NULL();
}
PHP_METHOD(BDict, getCurrentGear)
{
    BDict *bdict = NULL;
    bdict_object *intern = Z_BDICT_OBJ_P(getThis());
    bdict = intern->bdict_data;
    if (bdict != NULL) {
        RETURN_LONG(bdict->getCurrentGear());
    }
    RETURN_NULL();
}

static zend_function_entry bdict_methods[] = {
    PHP_ME(BDict, __construct,          NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(BDict, shift,                NULL, ZEND_ACC_PUBLIC)
    PHP_ME(BDict, accelerate,           NULL, ZEND_ACC_PUBLIC)
    PHP_ME(BDict, brake,                NULL, ZEND_ACC_PUBLIC)
    PHP_ME(BDict, getCurrentSpeed,      NULL, ZEND_ACC_PUBLIC)
    PHP_ME(BDict, getCurrentGear,       NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

PHP_MINIT_FUNCTION(bencode)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "BDict", bdict_methods);
    bdict_ce = zend_register_internal_class(&ce TSRMLS_CC);
    bdict_ce->create_object = bdict_object_new;

    memcpy(&bdict_object_handlers,
            zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    bdict_object_handlers.offset = XtOffsetOf(bdict_object, std);
    bdict_object_handlers.free_obj = bdict_free_storage;

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
    RETURN_TRUE;
}

