#ifndef BENCODE_HEADER_BITEM
#define BENCODE_HEADER_BITEM

extern "C" {
#include "php.h"
}

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class bitem
{
protected:
    static zval *throw_general_exception(const std::string message);
    static zval *get_zval_bool(const bool value);
    static std::string get_current_key(const std::string &path, size_t &pt);
    static std::string escape_key(const std::string &key);
    static bool is_ull(const std::string &s);
    static bool is_ll(const std::string &s);
    static void zend_hash_append_strings(HashTable *target, HashTable *source);

public:
    bitem() {}
    bitem(const bitem *that) {}
    ~bitem() {}

    virtual std::string get_type() const;
    static zval *parse(const std::string &ben);
    virtual std::string encode() const = 0;
    static zval *load(const std::string &file_path);
    void save(const std::string &file_path) const;
    virtual zval *to_array(const bool include_meta) const = 0;
    // mode 0: search in keys; mode 1: search in values
    virtual zval *search(const std::string &needle, const long &mode, const std::string path) const = 0;
};

#endif
