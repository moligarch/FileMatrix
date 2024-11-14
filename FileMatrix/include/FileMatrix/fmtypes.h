#pragma once
#include <string>
#include <vector>
#include <cstddef>
#include <fstream>

namespace FMatrix {

    namespace type
    {
        struct process_info_
        {
            std::wstring name;
            unsigned long pid;

            process_info_(const std::wstring& n, unsigned long p) : name(n), pid(p) {}
            process_info_() noexcept : pid(0) {};

            operator bool() const noexcept { return !name.empty() && pid != 0; }
        };

        class FMifstream : public std::basic_ifstream<std::byte> {
        };
    };

    using buffer_t = std::vector<std::byte>;
    using process_info = type::process_info_;
}