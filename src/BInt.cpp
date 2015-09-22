#include "BItem.h"
#include "BStr.h"
#include "BInt.h"
#include "BDict.h"
#include "BList.h"

/**
 * BInt implements
 */
BInt::BInt(const BItem *that) {
    if (that->getType().stringValue() != "BInt")
        throw Php::Exception("Error converting BItem to BInt, the original type is " + that->getType());
    _value = that->get().numericValue();
}

Php::Value BInt::getType() const {
    return "BInt";
}

Php::Value BInt::get() const {
    return _value;
}

void BInt::set(Php::Parameters &params) {
    int64_t value = params[0];
    _value = value;
}

Php::Value BInt::length() const {
    return (int64_t)(__toString().stringValue().length());
}

BInt* BInt::parseI(const std::string &ben, size_t &pt) {
    if (ben[pt] != 'i')
        throw Php::Exception("Error parsing BInt");
    ++pt;
    std::string strint = "";
    while (ben[pt] != 'e') {
        strint += ben[pt];
        ++pt;
    }
    ++pt;
    BInt *retval = new BInt(std::stoll(strint));
    return retval;
}

Php::Value BInt::toArray() const {
    return _value;
}

Php::Value BInt::toMetaArray() const {
    Php::Value retval;
    retval["_type"] = "BInt";
    retval["_length"] = length();
    retval["_data"] = _value;
    return retval;
}

Php::Value BInt::__toString() const {
    return "i" + numtos(_value) + "e";
}

void BInt::__construct(Php::Parameters &params) {
    int64_t value = params[0];
    _value = value;
}
