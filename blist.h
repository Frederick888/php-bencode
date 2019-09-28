#ifndef BENCODE_HEADER_BLIST
#define BENCODE_HEADER_BLIST

extern "C" {
#include "php.h"
#include "zend_exceptions.h"
}

#include "bitem.h"
#include <string>

class blist : public bitem
{
public:
    HashTable *_data;

    blist();
    blist(const blist *that);
    ~blist();

    std::string get_type() const;
    zval *get(const size_t &key) const;
    bool has(const size_t &key) const;
    void add(zval *value);
    void set(const size_t &key, zval *value);
    bool del(const size_t &key);
    zval *get_path(const std::string &key, size_t &pt) const;
    void set_path(const std::string &key, size_t &pt, zval *value);
    bool del_path(const std::string &key, size_t &pt);
    size_t length() const;
    size_t count() const;

    static zval *parse(const std::string &ben, size_t &pt);
    std::string encode() const;
    zval *to_array(const bool include_meta) const;
    zval *search(const std::string &needle, const long &mode, const std::string path) const;
};

#endif
