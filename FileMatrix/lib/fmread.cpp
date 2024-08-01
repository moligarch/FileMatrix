#include "pch.h"
#include "FileMatrix/fmread.h"





namespace fmatrix
{
    namespace read
    {
    } //namespace read

    namespace mtread //multi-thread read
    {
        std::optional<std::vector<std::byte>> read_file(
            /*__In__*/      const std::filesystem::path& file_path,
            /*__In__*/      size_t thread_count,
            /*__In_Out__*/  std::error_code& ec)
        {
            // Check if the file exists and get its size
            if (!std::filesystem::exists(file_path, ec)) {
                return std::nullopt;
            }
            auto fileSize = std::filesystem::file_size(file_path);

            // Calculate chunk size for each thread
            size_t chunkSize = fileSize / thread_count;
            std::vector<std::byte> result(fileSize); // Preallocate memory for the entire file

            // Function to read a chunk of the file
            auto readChunk = [&](size_t start, size_t end) -> void {
                std::ifstream file(file_path, std::ios::binary);
                file.seekg(start);
                file.read(reinterpret_cast<char*>(result.data() + start), end - start);
                };

            // Launch threads to read chunks of the file
            std::vector<std::future<void>> futures;
            for (size_t i = 0; i < thread_count; ++i) {
                size_t start = i * chunkSize;
                size_t end = (i == thread_count - 1) ? fileSize : start + chunkSize; // Last thread reads until the end
                futures.emplace_back(std::async(std::launch::async, readChunk, start, end));
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
                return std::nullopt;
            }
            

            return result;
        }
    } //namespace mtread
}