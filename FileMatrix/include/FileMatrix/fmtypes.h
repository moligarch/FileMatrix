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
        public:
            // Constructor that takes a file path and opens the file
            explicit FMifstream(std::filesystem::path file_path, std::ios_base::openmode mode = std::ios::in | std::ios::binary)
                : file_path_(file_path), std::basic_ifstream<std::byte>(std::move(file_path), mode) {
            }

            // Destructor
            ~FMifstream() {
                if (this->is_open()) {
                    this->close();
                }
            }

            // Method to get the file path
            const std::filesystem::path& get_path() const noexcept
            {
                return file_path_;
            }

        private:
            std::filesystem::path& file_path_;  // Store the file path
        };

        class FMofstream : public std::basic_ofstream<std::byte> {
        public:
            // Constructor that takes a file path and opens the file
            explicit FMofstream(std::string file_path, std::ios_base::openmode mode = std::ios::out | std::ios::binary)
                : file_path_(file_path), std::basic_ofstream<std::byte>(std::move(file_path), mode) {
            }

            // Destructor
            ~FMofstream() {
                if (this->is_open()) {
                    this->close();
                }
            }

            // Method to get the file path
            const std::string& get_path() const noexcept 
            {
                return file_path_;
            }

        private:
            std::string file_path_;  // Store the file path
        };

    };

    using buffer_t = std::vector<std::byte>;
    using process_info = type::process_info_;
}