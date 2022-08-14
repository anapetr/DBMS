
#ifndef DBMS_FILEUTILS_H
#define DBMS_FILEUTILS_H

#pragma once

#include <string>
#include <fstream>
#include <filesystem>

class FileUtils {
public:
    static void readString(std::ifstream &in, std::string &dest) {
        size_t size = 0;
        char *str = nullptr;
        in.read((char *) &size, sizeof(size));
        str = new char[size + 1];
        in.read(str, size);
        str[size] = '\0';
        dest = str;
        delete[] str;
    }

    static void writeString(std::ofstream &out, std::string dest) {
        size_t size = dest.size();
        out.write((char *) &size, sizeof(size));
        out.write((char *) dest.c_str(), size);
    }

    static void createFolder(const std::string &pathToFolder) {
        std::__fs::filesystem::create_directories(pathToFolder);
    }
};

#endif //DBMS_FILEUTILS_H
