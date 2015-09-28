#ifndef HEADER_BDICT
#define HEADER_BDICT

#include <phpcpp.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

class BDict : public BItem {
    private:
        BDict *me_dict;

    public:
        std::map<std::string, BItem*> BData;

        /**
         * C++ constructor and destructor
         */
        BDict() : BItem() {}

        BDict(const BDict &that) :
            BItem(), BData( that.BData ) { me_dict = this; }

        BDict(const BDict *that) :
            BItem(), BData( that->BData ) { me_dict = this; }

        BDict(const BItem *that) :
            BItem(), BData( that->getDataD() ) { me_dict = this; }

        virtual ~BDict() {}

        /**
         * Regular functions
         */
        Php::Value getType() const;

        BDict* me() { return me_dict; }

        std::map<std::string, BItem*> getDataD() const;

        BItem* getPath(const std::string &key) const;

        Php::Value has(Php::Parameters &params) const;

        bool chas(const std::string &key) const;

        void set(Php::Parameters &params);

        template<typename T>
        void setPath(const std::string &key, T *BItem);

        Php::Value del(Php::Parameters &params);

        Php::Value getKeys() const;

        Php::Value length() const;

        Php::Value size() const;

        static BDict* parseD(const std::string &ben, size_t &pt);

        Php::Value toArray() const;

        Php::Value toMetaArray() const;

        void csearch(const std::string &needle, const char &mode,
                    std::vector<std::string> &pathStack, std::vector<std::string> &result) const;
        /**
         * Magic methods
         */
        virtual void __construct();

        virtual void __destruct();

        Php::Value __toString() const;
};

#endif
