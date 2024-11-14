#pragma once
#include <string>
#include <vector>
#include <cstddef>
#include <fstream>

#include <hashplusplus/include/hashpp.h>

namespace FMatrix {

    namespace Type
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
            explicit FMifstream(const std::filesystem::path& file_path, std::ios_base::openmode mode = std::ios::in | std::ios::binary)
                : file_path_(file_path), std::basic_ifstream<std::byte>(file_path, mode) {
            }

            // Destructor
            ~FMifstream() {
                if (this->is_open()) {
                    this->close();
                }
            }

            // Copy constructor
            FMifstream(const FMifstream&) = default;
            // Move constructor
            FMifstream(FMifstream&&) noexcept = default;
            // Copy assignment operator
            FMifstream& operator=(const FMifstream&) = default;
            // Move assignment operator
            FMifstream& operator=(FMifstream&&) noexcept = default;
            // Method to get the file path
            const std::filesystem::path& get_path() const noexcept
            {
                return file_path_;
            }

        private:
            const std::filesystem::path& file_path_;  // Store the file path
        };

        class FMofstream : public std::basic_ofstream<std::byte> {
        public:
            // Constructor that takes a file path and opens the file
            explicit FMofstream(const std::string& file_path, std::ios_base::openmode mode = std::ios::out | std::ios::binary)
                : file_path_(file_path), std::basic_ofstream<std::byte>(file_path, mode) {
            }

            // Destructor
            ~FMofstream() {
                if (this->is_open()) {
                    this->close();
                }
            }

            // Copy constructor
            FMofstream(const FMofstream&) = default;
            // Move constructor
            FMofstream(FMofstream&&) noexcept = default;
            // Copy assignment operator
            FMofstream& operator=(const FMofstream&) = default;
            // Move assignment operator
            FMofstream& operator=(FMofstream&&) noexcept = default;

            // Method to get the file path
            const std::string& get_path() const noexcept 
            {
                return file_path_;
            }

        private:
            const std::string file_path_;  // Store the file path
        };

    };

    namespace Hash
    {
        using Algorithm = hashpp::ALGORITHMS;
    }
    using buffer_t = std::vector<std::byte>;
    using process_info = Type::process_info_;
}