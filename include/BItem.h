#ifndef HEADER_BITEM
#define HEADER_BITEM

#include <phpcpp.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

class BItem : public Php::Base {
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
        BItem() {}
        BItem(const BItem &that) {}
        virtual ~BItem() {}

        /**
         * Regular functions
         */
        virtual Php::Value getType() const;

        virtual Php::Value get() const;

        virtual std::map<std::string, BItem*> getDataD() const;

        virtual std::vector<BItem*> getDataL() const;

        static Php::Value parse(Php::Parameters &params);

        static Php::Value load(Php::Parameters &params);

        void save(Php::Parameters &params) const;

        Php::Value search(Php::Parameters &params) const;

        virtual void csearch(const std::string &needle, const char &mode,
                            std::vector<std::string> &pathStack, std::vector<std::string> &result) const;

        /**
         * Magic methods
         */
        virtual Php::Value __toString() const;
};

#endif
