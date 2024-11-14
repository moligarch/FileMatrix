#include "pch.h"
#include "FileMatrix/fmread.h"

void MakeCapital(std::string& str)
{
    for (auto& chr : str)
    {
        chr = std::toupper(chr);
    }
}

namespace FMatrix
{
    Reader::Reader(const std::filesystem::path& file_path) : file_(file_path)
    {
        file_.exceptions(file_.goodbit);
    }

    Reader::~Reader()
    {
        if (file_.is_open()) {
            file_.close();
        }
    }


    buffer_t Reader::Read()
    {
        if (!file_.good()) {
            return {};
        }

        buffer_t result(Type::FMifstream::_Iter(file_), {});
        return result;
    }
    bool Reader::GetHash(Hash::Algorithm algorithm, std::string& output)
    {
        output = hashpp::get::getFileHash(algorithm, file_.get_path().string()).getString();
        MakeCapital(output);
        if (output.empty())
        {
            return false;
        }
        
        return true;
    }
    std::streamsize Reader::ReadChunk(buffer_t& buffer, size_t chunk_size)
    {

        if (!file_.good()) {
            return -1;
        }
        file_.read(buffer.data(), chunk_size);
        return file_.gcount();
    }

    namespace MT
    {
        Reader::Reader(const std::filesystem::path& file_path, size_t thread_count) :
            file_(file_path), thread_count_(thread_count)
        {
            file_.exceptions(file_.goodbit);
        }
        Reader::~Reader()
    {
        if (file_.is_open()) {
            file_.close();
        }
    }
        buffer_t Reader::Read(std::error_code& ec)
    {
        // Check if the file exists and get its size
        if (!std::filesystem::exists(file_.get_path(), ec)) {
            return {};
        }
        const auto file_size = std::filesystem::file_size(file_.get_path(), ec);

        if (!file_size)
        {
            return {};
        }

        // Calculate chunk size for each thread
        const size_t chunk_size = file_size / thread_count_;
        std::vector<std::byte> result(file_size); // Preallocate memory for the entire file
        ;
        // Function to read a chunk of the file
        auto readfc = [&](size_t start, size_t end) -> void {
            file_.seekg(start);
            const auto data_span = std::span<std::byte>(result).subspan(start, end - start);
            file_.read(data_span.data(), data_span.size());
            };

        // Launch threads to read chunks of the file
        std::vector<std::future<void>> futures;
        for (size_t i = 0; i < thread_count_; ++i) {
            size_t start = i * chunk_size;
            size_t end = (i == thread_count_ - 1) ? file_size : start + chunk_size; // Last thread reads until the end
            futures.emplace_back(std::async(std::launch::async, readfc, start, end));
        }

        // Wait for all threads to finish
        try
        {
            for (auto& future : futures) {
                future.get();
            }
        }
        catch (const std::ios::failure& e) {
            ec = e.code();
            return {};
        }


        return result;
    }
    }
}