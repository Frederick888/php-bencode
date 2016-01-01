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
    size_t pt = 0;
    if (ben[0] == 'd') {
        return bdict::parse(ben, pt);
    } else if (ben[0] == 'l') {
        return blist::parse(ben, pt);
    } else if (ben[0] >= '0' && ben[0] <= '9') {
        return bstr::parse(ben, pt);
    } else if (ben[0] == 'i') {
        return bint::parse(ben, pt);
    } else {
        zend_throw_exception(
                zend_container::bitem_ce,
                "Error parsing bitem",
                1);
    }
}

zval * bitem::load(const std::string &file_path) {
    std::ifstream ben_file(file_path);
    if (!ben_file.is_open()) {
        zend_throw_exception(
                zend_container::bitem_ce,
                "Error opening file",
                1);
    }
    std::string ben((std::istreambuf_iterator<char>(ben_file)),
            (std::istreambuf_iterator<char>()));
    ben_file.close();
    zval _result = *parse(ben);
    zval *result = &_result;
    return result;
}

void bitem::save(const std::string &file_path) const {
    std::ofstream ben_file(file_path);
    if (!ben_file.is_open()) {
        zend_throw_exception(
                zend_container::bitem_ce,
                "Error opening file",
                1);
    }
    ben_file << encode();
    ben_file.close();
}
