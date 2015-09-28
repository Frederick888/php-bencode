#include "BItem.h"
#include "BStr.h"
#include "BInt.h"
#include "BDict.h"
#include "BList.h"

/**
 *  tell the compiler that the get_module is a pure C function
 */
extern "C" {
    
    /**
     *  Function that is called by PHP right after the PHP process
     *  has started, and that returns an address of an internal PHP
     *  strucure with all the details and features of your extension
     *
     *  @return void*   a pointer to an address that is understood by PHP
     */
    PHPCPP_EXPORT void *get_module() 
    {
        // static(!) Php::Extension object that should stay in memory
        // for the entire duration of the process (that's why it's static)
        static Php::Extension myExtension("bencode", "1.0");

        Php::Class<BItem> _BItem("BItem");
        _BItem.method("getType", &BItem::getType);
        _BItem.method("parse", &BItem::parse, {
                Php::ByVal("ben", Php::Type::String, true)
                });
        _BItem.method("load", &BItem::load, {
                Php::ByVal("file", Php::Type::String, true)
                });
        _BItem.method("save", &BItem::save, {
                Php::ByVal("file", Php::Type::String, true)
                });
        _BItem.method("search", &BItem::search, {
                Php::ByVal("needle", Php::Type::String, true),
                Php::ByVal("mode", Php::Type::String, true)
                });
        _BItem.method("get", &BItem::get, {
                Php::ByVal("key", Php::Type::String, true)
                });

        Php::Class<BDict> _BDict("BDict");
        _BDict.extends(_BItem);
        _BDict.method("getType", &BDict::getType);
        _BDict.method("set", &BDict::set, {
                Php::ByVal("key", Php::Type::String, true),
                Php::ByVal("value", Php::Type::Null, true)
                });
        _BDict.method("has", &BDict::has, {
                Php::ByVal("key", Php::Type::String, true)
                });
        _BDict.method("del", &BDict::del, {
                Php::ByVal("key", Php::Type::String, true)
                });
        _BDict.method("getKeys", &BDict::getKeys);
        _BDict.method("length", &BDict::length);
        _BDict.method("size", &BDict::size);
        _BDict.method("toArray", &BDict::toArray);
        _BDict.method("toMetaArray", &BDict::toMetaArray);
        _BDict.method("__toString", &BDict::__toString);
        _BDict.method("__construct", &BDict::__construct);
        _BDict.method("__destruct", &BDict::__destruct);

        Php::Class<BList> _BList("BList");
        _BList.extends(_BItem);
        _BList.method("getType", &BList::getType);
        _BList.method("set", &BList::set, {
                Php::ByVal("key", Php::Type::String, true),
                Php::ByVal("value", Php::Type::Null, true)
                });
        _BList.method("has", &BList::has, {
                Php::ByVal("key", Php::Type::String, true)
                });
        _BList.method("del", &BList::del, {
                Php::ByVal("key", Php::Type::String, true)
                });
        _BList.method("length", &BList::length);
        _BList.method("size", &BList::size);
        _BList.method("toArray", &BList::toArray);
        _BList.method("toMetaArray", &BList::toMetaArray);
        _BList.method("__toString", &BList::__toString);
        _BList.method("__construct", &BList::__construct);
        _BList.method("__destruct", &BList::__destruct);

        Php::Class<BStr> _BStr("BStr");
        _BStr.extends(_BItem);
        _BStr.method("getType", &BStr::getType);
        _BStr.method("get", &BStr::get);
        _BStr.method("set", &BStr::set, {
                Php::ByVal("value", Php::Type::String, true)
                });
        _BStr.method("length", &BStr::length);
        _BStr.method("toArray", &BStr::toArray);
        _BStr.method("toMetaArray", &BStr::toMetaArray);
        _BStr.method("__toString", &BStr::__toString);
        _BStr.method("__construct", &BStr::__construct, {
                Php::ByVal("value", Php::Type::String, false)
                });

        Php::Class<BInt> _BInt("BInt");
        _BInt.extends(_BItem);
        _BInt.method("getType", &BInt::getType);
        _BInt.method("get", &BInt::get);
        _BInt.method("set", &BInt::set, {
                Php::ByVal("value", Php::Type::String, true)
                });
        _BInt.method("length", &BInt::length);
        _BInt.method("toArray", &BInt::toArray);
        _BInt.method("toMetaArray", &BInt::toMetaArray);
        _BInt.method("__toString", &BInt::__toString);
        _BInt.method("__construct", &BInt::__construct, {
                Php::ByVal("value", Php::Type::Numeric, false)
                });

        myExtension.add(std::move(_BItem));
        myExtension.add(std::move(_BDict));
        myExtension.add(std::move(_BList));
        myExtension.add(std::move(_BStr));
        myExtension.add(std::move(_BInt));
        
        // return the extension
        return myExtension;
    }
}

