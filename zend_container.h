#ifndef BENCODE_HEADER_ZEND_CONTAINER
#define BENCODE_HEADER_ZEND_CONTAINER

extern "C" {
#include "php.h"
}

#include "bitem.h"
#include "bdict.h"
#include "bstr.h"

#define ZEND_CONTAINER_PRE(bclass)      \
    typedef struct {                    \
        bclass *bclass##_data;          \
        zend_object std;                \
    } bclass##_object;

ZEND_CONTAINER_PRE(bitem)
ZEND_CONTAINER_PRE(bdict)
ZEND_CONTAINER_PRE(bstr)

#define ZEND_CONTAINER_INIT(bclass)                                             \
    static zend_object_handlers bclass##_object_handlers;                       \
    static zend_class_entry *bclass##_ce;                                       \
    static bclass##_object * bclass##_fetch_object(zend_object *obj);           \
    static void bclass##_free_storage(zend_object *object TSRMLS_DC);           \
    static zend_object * bclass##_object_clone(zval *object TSRMLS_DC);         \
    static zend_object * bclass##_object_new(zend_class_entry *ce TSRMLS_DC);

class zend_container {
public:
    ZEND_CONTAINER_INIT(bitem)
    ZEND_CONTAINER_INIT(bdict)
    ZEND_CONTAINER_INIT(bstr)
};

#endif
