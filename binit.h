#ifndef BENCODE_HEADER_INIT
#define BENCODE_HEADER_INIT

#include "zend_container.h"

#define BI_PRELUDE(bclass)                                              \
    zend_object_handlers zend_container::bclass##_object_handlers;      \
    zend_class_entry *zend_container::bclass##_ce;                      \
    bclass##_object * zend_container::bclass##_fetch_object(zend_object *obj)           \
    {                                                                                   \
        return (bclass##_object *)((char *)obj - XtOffsetOf(bclass##_object, std));     \
    }                                                                                   \
    void zend_container::bclass##_free_storage(zend_object *object TSRMLS_DC)           \
    {                                                                                   \
        bclass##_object *intern = zend_container::bclass##_fetch_object(object);        \
        delete intern->bclass##_data;                                                   \
        zend_object_std_dtor(&intern->std TSRMLS_CC);                                   \
    }                                                                                   \
    zend_object * zend_container::bclass##_object_new(zend_class_entry *ce TSRMLS_DC)   \
    {                                                                           \
        bclass##_object *intern = (bclass##_object *)ecalloc(1,                 \
                sizeof(bclass##_object) +                                       \
                zend_object_properties_size(ce));                               \
        zend_object_std_init(&intern->std, ce TSRMLS_CC);                       \
        object_properties_init(&intern->std, ce);                               \
        intern->std.handlers = &zend_container::bclass##_object_handlers;       \
        return &intern->std;                                                    \
    }                                                                                                           \
    zend_object * zend_container::bclass##_object_clone(zval *object TSRMLS_DC)                                 \
    {                                                                                                           \
        bclass##_object *old_object = zend_container::bclass##_fetch_object(Z_OBJ_P(object));                   \
        zend_object *new_zend_object = zend_container::bclass##_object_new(zend_container::bclass##_ce);        \
        bclass##_object *new_object = zend_container::bclass##_fetch_object(new_zend_object);                   \
        bclass *new_object_data = new bclass(old_object->bclass##_data);                                        \
        new_object->bclass##_data = new_object_data;                                                            \
        return &new_object->std;                                                                                \
    }

#define BI_MINIT(bclass) do {                                                                                   \
    zend_class_entry ce;                                                                                        \
    if (ini_ns) INIT_CLASS_ENTRY(ce, "bencode\\"#bclass, bclass##_methods)                                      \
    else INIT_CLASS_ENTRY(ce, #bclass, bclass##_methods);                                                       \
    zend_container::bclass##_ce = zend_register_internal_class_ex(&ce, zend_container::bitem_ce TSRMLS_CC);     \
    zend_container::bclass##_ce->create_object = zend_container::bclass##_object_new;                           \
    memcpy(&zend_container::bclass##_object_handlers,                                                           \
            zend_get_std_object_handlers(), sizeof(zend_object_handlers));                                      \
    zend_container::bclass##_object_handlers.offset = XtOffsetOf(bclass##_object, std);                         \
    zend_container::bclass##_object_handlers.clone_obj = zend_container::bclass##_object_clone;                 \
    zend_container::bclass##_object_handlers.free_obj = zend_container::bclass##_free_storage; } while (0);

#define Z_BITEM_OBJ_P(zv) zend_container::bitem_fetch_object(Z_OBJ_P(zv))
zend_object_handlers zend_container::bitem_object_handlers;
zend_class_entry *zend_container::bitem_ce;
#define Z_BDICT_OBJ_P(zv) zend_container::bdict_fetch_object(Z_OBJ_P(zv))
BI_PRELUDE(bdict)
#define Z_BLIST_OBJ_P(zv) zend_container::blist_fetch_object(Z_OBJ_P(zv))
BI_PRELUDE(blist)
#define Z_BSTR_OBJ_P(zv) zend_container::bstr_fetch_object(Z_OBJ_P(zv))
BI_PRELUDE(bstr)
#define Z_BINT_OBJ_P(zv) zend_container::bint_fetch_object(Z_OBJ_P(zv))
BI_PRELUDE(bint)

#endif
