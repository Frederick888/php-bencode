#include "BItem.h"
#include "BStr.h"
#include "BInt.h"
#include "BDict.h"
#include "BList.h"

/**
 * BStr implements
 */
BStr::BStr(const BItem *that) {
    if (that->getType().stringValue() != "BStr")
        throw Php::Exception("Error converting BItem to BStr, the original type is " + that->getType());
    _value = that->getDataS();
    me_str = this;
}

Php::Value BStr::getType() const {
    return "BStr";
}

std::string BStr::getDataS() const {
    return _value;
}

Php::Value BStr::get() const {
    return _value;
}

void BStr::set(Php::Parameters &params) {
    std::string value = params[0];
    _value = value;
}

Php::Value BStr::length() const {
    return (int64_t)(__toString().stringValue().length());
}

void BStr::csearch(const std::string &needle, const char &mode,
                    std::vector<std::string> &pathStack, std::vector<std::string> &result) const {
    if (mode != 's') return;
    if (_value.find(needle) < _value.length()) {
        std::string path;
        for (size_t i = 0; i < pathStack.size(); i++)
            path += pathStack[i];
        result.push_back(trimKey(path));
    }
}

Php::Value BStr::__toString() const {
    return numtos(_value.size()) + ":" + _value;
}

void BStr::__construct(Php::Parameters &params) {
    std::string value = params[0];
    _value = value;
}

BStr* BStr::parseS(const std::string &ben, size_t &pt) {
    if (ben[pt] < '0' || ben[pt] > '9')
        throw Php::Exception("Error parsing BStr");
    const size_t start = pt;
    while (ben[pt] >= '0' && ben[pt] <= '9') ++pt;
    std::string len = ben.substr(start, pt - start);
    ++pt;
    BStr *retval = new BStr(ben.substr(pt, std::stoull(len)));
    pt += std::stoull(len);
    return retval;
}

Php::Value BStr::toArray() const {
    return _value;
}

Php::Value BStr::toMetaArray() const {
    Php::Value retval;
    retval["_type"] = "BStr";
    retval["_length"] = length();
    retval["_data"] = _value;
    return retval;
}
