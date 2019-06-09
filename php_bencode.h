#ifndef PHP_BENCODE_H
#define PHP_BENCODE_H

#define PHP_BENCODE_EXTNAME     "bencode"
#define PHP_BENCODE_EXTVER      "1.2.4"

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

#define CALL_AND_HANDLE(expr)                                                                                   \
    try {                                                                                                       \
        return (expr);                                                                                          \
    } catch (const std::invalid_argument& ia) {                                                                 \
        zend_throw_exception(NULL, std::string("Invalid argument: " + std::string(ia.what())).c_str(), 2);      \
        RETURN_NULL();                                                                                          \
    } catch (const std::out_of_range& oor) {                                                                    \
        zend_throw_exception(NULL, std::string("Out of Range error: " + std::string(oor.what())).c_str(), 3);   \
        RETURN_NULL();                                                                                          \
    } catch (const std::exception& e) {                                                                         \
        zend_throw_exception(NULL, std::string("Undefined error: " + std::string(e.what())).c_str(), 4);        \
        RETURN_NULL();                                                                                          \
    } catch (...) {                                                                                             \
        zend_throw_exception(NULL, "Unkown error", 5);                                                          \
        RETURN_NULL();                                                                                          \
    }                                                                                                           \


#endif
