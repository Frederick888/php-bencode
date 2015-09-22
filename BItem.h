class BItem : public Php::Base {
    protected:
        template<typename T>
        std::string numtos(const T &value) const;

        std::string trimKey(const std::string &key) const;

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

        /**
         * Magic methods
         */
        virtual Php::Value __toString() const;
};
