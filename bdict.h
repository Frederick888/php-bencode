#ifndef BENCODE_HEADER_BDICT
#define BENCODE_HEADER_BDICT

extern "C" {
#include "php.h"
#include "zend_exceptions.h"
}

#include <string>
#include "bitem.h"

class bdict : public bitem {
    public:
        HashTable *_data;

        bdict();
        bdict(const bdict *that);
        ~bdict();

        std::string get_type() const;
        zval * get(const std::string &key) const;
        bool has(const std::string &key) const;
        void set(const std::string &key, zval *value);
        bool del(const std::string &key);
        zval * get_path(const std::string &key, size_t &pt) const;
        size_t length() const;
        size_t count() const;

        static zval * parse(const std::string &ben, size_t &pt);
        std::string encode() const;
        zval * to_array(const bool include_meta) const;
};

#endif
