//
// Created by Junyi Hou on 2/2/21.
//

#ifndef OJ_BASE_H
#define OJ_BASE_H

#include <string>
#include <algorithm>
using std::string;

namespace JuicerLang {
    class Base {
    public:
        /* 编译指定程序 */
        virtual int compile(string source_code, string args) = 0;

        /* 在沙盒中运行程序 */
        virtual int run(string path, string args, string input, string &output) = 0;

        /* 比较 input 和 output 是否相同 */
        virtual int diff(string input, string output) = 0;

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
    };
}

#endif //OJ_BASE_H
