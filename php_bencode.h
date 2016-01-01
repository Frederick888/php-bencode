#ifndef PHP_BENCODE_H
#define PHP_BENCODE_H

#define PHP_BENCODE_EXTNAME     "bencode"
#define PHP_BENCODE_EXTVER      "1.0"

extern "C" {
#include "php.h"
#ifdef ZTS
#include "TSRM.h"
#endif
}

PHP_FUNCTION(bencode_hello);

extern zend_module_entry bencode_module_entry;
#define phpext_bencode_ptr &bencode_module_entry

#endif
