#ifndef PHP_BENCODE_H
#define PHP_BENCODE_H

/* clang-format off */
#define PHP_BENCODE_EXTNAME     "bencode"
#define PHP_BENCODE_EXTVER      "1.2.5"
/* clang-format on */

extern "C" {
#include "php.h"
#include "php_ini.h"
#ifdef ZTS
#include "TSRM.h"
#endif
}

PHP_MINIT_FUNCTION(bencode);
PHP_MSHUTDOWN_FUNCTION(bencode);

extern zend_module_entry bencode_module_entry;
#define phpext_bencode_ptr &bencode_module_entry

#endif
