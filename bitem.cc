#include <string>
#include "bitem.h"
#include "bdict.h"
#include "blist.h"
#include "bstr.h"
#include "bint.h"
#include "zend_container.h"

zval * bitem::throw_general_exception(const std::string message) {
    zend_throw_exception(NULL, message.c_str(), 1);
    zval _zv;
    zval *zv = &_zv;
    ZVAL_BOOL(zv, 0);
    return zv;
}

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
        return bitem::throw_general_exception("Error parsing bitem");
    }
}

zval * bitem::load(const std::string &file_path) {
    std::ifstream ben_file(file_path);
    if (!ben_file.is_open()) {
        return bitem::throw_general_exception("Error opening file");
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
        bitem::throw_general_exception("Error opening file");
        return;
    }
    ben_file << encode();
    ben_file.close();
}
