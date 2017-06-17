#include <string>
#include "bitem.h"
#include "bdict.h"
#include "blist.h"
#include "bstr.h"
#include "bint.h"
#include "zend_container.h"

zval * bitem::throw_general_exception(const std::string message) {
    zend_throw_exception(NULL, message.c_str(), 1);
    return bitem::get_zval_bool(false);
}

zval * bitem::get_zval_bool(const bool value) {
    zval *zv = new zval();
    ZVAL_BOOL(zv, value);
    return zv;
}

std::string bitem::get_current_key(const std::string &path, size_t &pt) {
    size_t start = pt;
    while (!(path[pt] == '/' && path[pt - 1] != '\\') && pt < path.length()) ++pt;
    std::string current_key = path.substr(start, pt - start);
    ++pt;
    size_t escape = current_key.find("\\/");
    while (escape < current_key.length()) {
        current_key.replace(escape, 2, "/");
        escape = current_key.find("\\/");
    }
    return current_key;
}

std::string bitem::escape_key(const std::string &key) {
    std::string result = "";
    size_t pt = 0;
    size_t to_esc = std::min(key.find('/', pt), key.find('\\', pt));
    while (to_esc < key.length()) {
        result += key.substr(pt, to_esc - pt);
        if (key[to_esc] == '/')
            result += "\\/";
        else if (key[to_esc] == '\\')
            result += "\\\\";
        pt = to_esc + 1;
        to_esc = std::min(key.find('/', pt), key.find('\\', pt));
    }
    return result + key.substr(pt);
}

bool bitem::is_ull(const std::string &s) {
    if(s.empty() || !isdigit(s[0])) return false;
    char *p;
    strtoull(s.c_str(), &p, 10);
    return (*p == 0);
}

bool bitem::is_ll(const std::string &s) {
    if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

    char *p;
    strtoll(s.c_str(), &p, 10);

    return (*p == 0);
}

void bitem::zend_hash_append_strings(HashTable *target, HashTable *source) {
    for(zend_hash_internal_pointer_reset(source);
            zend_hash_has_more_elements(source) == SUCCESS;
            zend_hash_move_forward(source)) {
        zend_string *_str_index;
        zend_ulong num_index;
        zend_hash_get_current_key(source, &_str_index, &num_index);
        zval *value = zend_hash_get_current_data(source);
        zend_hash_next_index_insert(target, value);
    }
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
    } else if (isdigit(ben[0])) {
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
    zval *result = parse(ben);
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

zval * bitem::search(const std::string &needle, const long &mode, const std::string path) const {
    return nullptr;
}
