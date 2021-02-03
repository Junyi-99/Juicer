//
// Created by Junyi Hou on 2/2/21.
//

#ifndef OJ_BASE_H
#define OJ_BASE_H

#include <string>
#include <algorithm>
#include "../sandbox.h"

using std::string;

namespace JuicerLang {
    class NotImplementedException : public std::exception {
    public:
        explicit NotImplementedException(const char *error = "Function not yet implemented.") {
            errorMessage = error;
        }

        const char *what() const noexcept override {
            return errorMessage.c_str();
        }

    private:
        std::string errorMessage;
    };

    class Base {
    public:
        Base() {
            lang_name = "none";
            binary_path = "none";
        }

        virtual /* 编译指定程序 */
        int compile(string const &source_code) { throw NotImplementedException(); }

        virtual /* 在沙盒中运行程序 */
        int run(const string &input, string &output, uint32_t limit_time,
                uint32_t limit_stack, uint32_t limit_memory, uint32_t limit_output) { throw NotImplementedException(); }

        virtual /* 比较 input 和 output 是否相同 */
        int diff(string const &input, string const &output) { throw NotImplementedException(); }

        virtual /* 设置 seccomp 规则，每个语言都要自己设置一份 */
        int setRules() { throw NotImplementedException(); }

        /* 设置语言名（全小写） */
        void setLang(string name) {
            std::transform(name.begin(), name.end(), this->lang_name.begin(), ::tolower);
        }

        /* 获取语言名 */
        string getLang() {
            return this->lang_name;
        };

    protected:
        string lang_name;
        string binary_path;
    };
}

#endif //OJ_BASE_H
