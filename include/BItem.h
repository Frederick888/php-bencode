#ifndef HEADER_BITEM
#define HEADER_BITEM

#include <phpcpp.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <limits>

class BItem : public Php::Base {
    private:
        BItem *me_item;

    protected:
        template<typename T>
        std::string numtos(const T &value) const {
            std::string retval;
            std::stringstream sstream;
            sstream << value;
            sstream >> retval;
            return retval;
        }

        std::string trimKey(std::string key) const;

        std::string escapeKey(std::string key) const;

        std::string parseKey(std::string key) const;

        std::string splitKey(std::string &key) const;

        bool isSizet(const std::string &intstr) const;

    public:
        /**
         * C++ constructor and destructor
         */
        BItem() { me_item = this; }
        BItem(const BItem &that) { me_item = this; }
        virtual ~BItem() {}

        /**
         * Regular functions
         */
        virtual Php::Value getType() const;

        virtual BItem* me() const { return me_item; }

        virtual BItem* clone() const = 0;

        virtual Php::Value get(Php::Parameters &params) const;

        virtual BItem* getPath(const std::string &key) const;

        virtual std::map<std::string, BItem*> getDataD() const;

        virtual std::vector<BItem*> getDataL() const;

        virtual std::string getDataS() const;

        virtual int64_t getDataI() const;

        static Php::Value parse(Php::Parameters &params);

        static Php::Value load(Php::Parameters &params);

        static Php::Value parseStr(const std::string &ben);

        void save(Php::Parameters &params) const;

        virtual Php::Value toArray() const = 0;

        virtual Php::Value toMetaArray() const = 0;

        Php::Value search(Php::Parameters &params) const;

        virtual void csearch(const std::string &needle, const char &mode,
                            std::vector<std::string> &pathStack, std::vector<std::string> &result) const = 0;

        /**
         * Magic methods
         */
        virtual Php::Value __toString() const = 0;
};

#endif
