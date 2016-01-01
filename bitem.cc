#include <string>
#include "bitem.h"
#include "bdict.h"
#include "blist.h"
#include "bstr.h"
#include "bint.h"
#include "zend_container.h"

std::string bitem::get_type() const {
    return "bitem";
}

zval * bitem::parse(const std::string &ben) {
}
