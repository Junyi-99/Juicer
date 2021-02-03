//
// Created by Junyi Hou on 2/2/21.
//

#ifndef OJ_CPP_H
#define OJ_CPP_H

#include "base.h"

namespace JuicerLang {
    class GNU_cpp_compiler : public Base {
    private:
        GNU_cpp_compiler() = default;

    public:
        GNU_cpp_compiler(GNU_cpp_compiler const &) = delete;

        void operator=(GNU_cpp_compiler const &) = delete;

        static GNU_cpp_compiler &getInstance() {
            static GNU_cpp_compiler instance;
            instance.lang_name = "cpp";
            return instance;
        }

        int compile(const string &source_code) override;

        int run(const string &path, const string &args, const string &env,
                const string &input, string &output) override;

        int diff(const string &input, const string &output) override;

        int setRules() override;
    };
}


#endif //OJ_CPP_H
