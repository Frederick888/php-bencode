#ifndef BENCODE_HEADER_ZEND_CONTAINER
#define BENCODE_HEADER_ZEND_CONTAINER

extern "C" {
#include "php.h"
}

#include "bitem.h"
#include "bdict.h"
#include "blist.h"
#include "bstr.h"
#include "bint.h"

#define ZEND_CONTAINER_PRE(bclass)      \
    typedef struct {                    \
        bclass *bnode_data;             \
        zend_object std;                \
    } bclass##_object;

ZEND_CONTAINER_PRE(bitem)
ZEND_CONTAINER_PRE(bdict)
ZEND_CONTAINER_PRE(blist)
ZEND_CONTAINER_PRE(bstr)
ZEND_CONTAINER_PRE(bint)

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
    ZEND_CONTAINER_INIT(blist)
    ZEND_CONTAINER_INIT(bstr)
    ZEND_CONTAINER_INIT(bint)
    static std::string bnode_object_get_class_name(zval *object) {
        zend_bool ini_ns = zend_ini_long((char *)"bencode.namespace", 17, 0);
        std::string class_name(ZSTR_VAL(Z_OBJ_P(object)->ce->name));
        if (ini_ns) {
            return class_name.substr(8);
        } else {
            return class_name;
        }
    }
    static zend_object * bnode_object_clone(zval *object) {
        zval *new_object = new zval();
        ZVAL_OBJ(new_object, Z_OBJ_P(object)->handlers->clone_obj(object));
        return Z_OBJ_P(new_object);
    }
    static inline bitem * bnode_fetch_object_data(zend_object *obj) {
        return (bitem *)(*((uintptr_t *)((char *)obj - sizeof(uintptr_t))));
    }
};

#endif
