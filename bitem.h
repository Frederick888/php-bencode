#ifndef BENCODE_HEADER_BITEM
#define BENCODE_HEADER_BITEM

extern "C" {
#include "php.h"
}

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

class bitem {
    protected:
        static zval * throw_general_exception(const std::string message);
        static zval * get_zval_bool(const bool value);

    public:
        bitem() {}
        bitem(const bitem *that) {}
        ~bitem() {}

        virtual std::string get_type() const;
        static zval * parse(const std::string &ben);
        virtual std::string encode() const = 0;
        static zval * load(const std::string &file_path);
        void save(const std::string &file_path) const;
};

#endif
