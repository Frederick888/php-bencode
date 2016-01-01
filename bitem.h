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
        template<typename T>
        static std::string numtos(const T &value) {
            std::string result;
            std::stringstream ss;
            ss << value;
            ss >> result;
            return result;
        }

    public:
        bitem() {}
        bitem(const bitem *that) {}
        ~bitem() {}

        virtual std::string get_type() const;
        static zval * parse(const std::string &ben);
        virtual std::string encode() const = 0;
};

#endif
