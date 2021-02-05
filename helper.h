//
// Created by Junyi Hou on 2/3/21.
//

#ifndef JUICER_HELPER_H
#define JUICER_HELPER_H

#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <locale>

enum class ResultType {
    NO_SUCH_PROBLEM,
    SYSTEM_ERROR,
    COMPILE_ERROR,

    ACCEPTED,
    WRONG_ANSWER,
    PRESENTATION_ERROR,
    RUNTIME_ERROR,
    TIME_LIMIT_EXCEED,
    MEMORY_LIMIT_EXCEED,
    OUTPUT_LIMIT_EXCEED,
    NON_ZERO_EXIT_ERROR,
    SEGMENTATION_FAULT,
    FLOAT_POINT_EXCEPTION, // Divide by 0
    VALIDATE_ERROR,
};

class ResultException : public std::exception {
public:
    explicit ResultException(ResultType type, const std::string &details) {
        result_type = type;
        error_message = details;
    }

    ResultType what_type() {
        return result_type;
    }

    const char *what() const noexcept override {
        return error_message.c_str();
    }

private:
    ResultType result_type;
    std::string error_message;
};

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

namespace JuicerHelper {

    // trim from start (in place)
    static inline void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
    }

    // trim from end (in place)
    static inline void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    // trim from both ends (in place)
    static inline void trim(std::string &s) {
        ltrim(s);
        rtrim(s);
    }

    // trim from start (copying)
    static inline std::string ltrim_copy(std::string s) {
        ltrim(s);
        return s;
    }

    // trim from end (copying)
    static inline std::string rtrim_copy(std::string s) {
        rtrim(s);
        return s;
    }

    // trim from both ends (copying)
    static inline std::string trim_copy(std::string s) {
        trim(s);
        return s;
    }

    static std::string read_file(const std::string &file_path) {
        std::ifstream file(file_path, std::ios::binary);
        std::string str((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
        return str;
    }

    static void write_file(const std::string &file_path, const std::string &content) {
        std::ofstream file(file_path, std::ios::binary | std::ios::trunc);
        file << content;
        file.close();
    }

    static std::vector<std::string> split(std::string &str) {
        std::stringstream ss(str);
        std::vector<std::string> result;

        while (ss.good()) {
            std::string substr;
            getline(ss, substr, ',');
            result.push_back(substr);
        }
        return result;
    }

};


#endif //JUICER_HELPER_H
