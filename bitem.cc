#include <string>
#include "bitem.h"
#include "bdict.h"
#include "bstr.h"
#include "zend_container.h"

std::string bitem::get_type() const {
    return "bitem";
}

zval * bitem::parse(const std::string &ben) {
    zval _zv;
    zval *zv = &_zv;
    object_init_ex(zv, zend_container::bitem_ce);
    bitem_object *intern = zend_container::bitem_fetch_object(Z_OBJ_P(zv));
    intern->bitem_data = new bitem();
    return zv;
}
