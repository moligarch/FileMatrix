#pragma once
#ifndef FILE_MATRIX
#define FILE_MATRIX
#include "pch.h"

struct process_info {
    std::wstring name;
    unsigned long pid;

    process_info(const std::wstring& n, unsigned long p) : name(n), pid(p) {}
    process_info() : pid(0) {}

    operator bool() const { return !name.empty() && pid != 0; }
};

namespace utility {
    std::string to_string(const std::wstring& wstr);
    std::wstring to_wstring(const std::string& str);
}


namespace fmatrix {
    namespace monitor {
        std::vector<process_info> find_process(const std::filesystem::path& filePath);
    }
}

#endif // !FILE_MATRIX
