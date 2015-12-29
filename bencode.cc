#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "php.h"
#include "php_bencode.h"

PHP_MINIT_FUNCTION(bencode)
{
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

