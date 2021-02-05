//
// Created by Junyi Hou on 2/2/21.
//

#ifndef OJ_BASE_H
#define OJ_BASE_H

#include <string>
#include <algorithm>
#include "../sandbox.h"
#include "../helper.h"

using std::string;

namespace JuicerLang {

    class Base {
    public:

        virtual /* 编译指定程序 */
        int compile() { throw NotImplementedException(); }

        /* 在沙盒中运行程序，output 的文件路径 会被自动记录在私有成员里*/
        virtual int run(const uint8_t &case_id) { throw NotImplementedException(); }

        /* 比较 input 和 output 是否相同 */
        virtual int diff(const uint8_t &case_id) { throw NotImplementedException(); }

        void
        config(const string &code_source, const uint32_t &compile_time_limit, const uint32_t &run_time_limit,
               const uint32_t &stack_limit, const uint32_t &memory_limit, const uint32_t &output_limit,
               const vector<string> &in_cases, const vector<string> &out_cases) {
            this->source_code = code_source;
            this->limit_stack = stack_limit;
            this->limit_output = output_limit;
            this->limit_memory = memory_limit;
            this->limit_run_time = run_time_limit;
            this->limit_compile_time = compile_time_limit;
            this->cases_in = in_cases;
            this->cases_out = out_cases;
        }

        /* 获取语言名 */
        string getLang() {
            return this->lang_name;
        };

    protected:
        string source_code = "none";
        uint32_t limit_compile_time = 0; // ms, default 3000ms
        uint32_t limit_run_time = 0;     // ms, default 1000ms
        uint32_t limit_stack = 0;        // KBytes, default 262,144 KBytes (256MB)
        uint32_t limit_memory = 0;       // KBytes, default 262,144 KBytes (256MB)
        uint32_t limit_output = 0;       // KBytes, default 1024 KBytes (10KB)
        vector<string> cases_in;         // 文件路径
        vector<string> cases_out;        // 文件路径
        string output_file = "output.txt"; /* 重定向 stdout 到这个文件 */
        string compile_log = "compile_log.txt"; /* 重定向 stdout 到这个文件 */

    protected:
        string lang_name = "none";
        string binary_path = "none"; /* 编译后生成的二进制文件路径 */
    };
}

#endif //OJ_BASE_H
