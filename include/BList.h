#ifndef HEADER_BLIST
#define HEADER_BLIST

#include <phpcpp.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

class BList : public BItem {
    private:
        BList *me_list;

    public:
        std::vector<BItem*> BData;

        /**
         * C++ constructor and destructor
         */
        BList() : BItem() {}

        BList(const BList &that) :
            BItem(), BData( that.BData ) { me_list = this; }

        BList(const BList *that) :
            BItem(), BData( that->BData ) { me_list = this; }

        BList(const BItem *that) :
            BItem(), BData( that->getDataL() ) { me_list = this; }

        virtual ~BList() {}

        /**
         * Regular functions
         */
        Php::Value getType() const;

        BList* me() const { return me_list; }

        BList* clone() const;

        std::vector<BItem*> getDataL() const;

        BItem* getPath(const std::string &key) const;

        Php::Value has(Php::Parameters &params) const;

        bool chas(const std::string &key) const;

        void set(Php::Parameters &params);

        template<typename T>
        void setPath(const std::string &key, T *BItem);

        Php::Value del(Php::Parameters &params);

        Php::Value length() const;

        Php::Value size() const;

        static BList* parseL(const std::string &ben, size_t &pt);

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
