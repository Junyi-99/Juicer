//
// Created by Junyi Hou on 2/2/21.
//

#ifndef OJ_C_H
#define OJ_C_H

#include "base.h"

namespace JuicerLang {
    class GNU_c_compiler : public Base {
    private:
        GNU_c_compiler() = default;

    public:
        GNU_c_compiler(GNU_c_compiler const &) = delete;

        void operator=(GNU_c_compiler const &) = delete;

        static GNU_c_compiler &getInstance() {
            static GNU_c_compiler instance;
            instance.lang_name = "c";
            return instance;
        }

        int compile(const string &source_code) override;

        int
        run(const string &path, const string &args, const string &env, const string &input, string &output) override;

        int diff(const string &input, const string &output) override;

        int setRules() override;
    };
}


#endif //OJ_C_H
