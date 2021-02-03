//
// Created by Junyi Hou on 2/3/21.
//

#ifndef JUICER_HELPER_H
#define JUICER_HELPER_H

#include <string>
#include <fstream>

class JuicerHelper {
public:
    static std::string read_file(const std::string &file_path) {
        std::ifstream file(file_path, std::ios::binary);
        std::string str((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
        return str;
    }

    static void write_file(const std::string &file_path, const std::string &content) {
        std::ofstream file(file_path, std::ios::binary);
        file << content;
        file.close();
    }
};


#endif //JUICER_HELPER_H
