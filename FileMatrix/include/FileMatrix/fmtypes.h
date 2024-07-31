#pragma once
#include <string>

namespace fmatrix {

    namespace type
    {
        struct process_info
        {
            std::wstring name;
            unsigned long pid;

            process_info(const std::wstring& n, unsigned long p) : name(n), pid(p) {}
            process_info() : pid(0) {}

            operator bool() const { return !name.empty() && pid != 0; }
        };
    };

    using process_info = type::process_info;
}