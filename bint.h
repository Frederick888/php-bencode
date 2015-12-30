#ifndef BENCODE_HEADER_BITEM
#define BENCODE_HEADER_BITEM

extern "C" {
#include "php.h"
}

#include "bitem.h"

class bint : public bitem {
    public:
        long _value;
};

#endif
