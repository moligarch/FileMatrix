#pragma once
#include <filesystem>
#include <vector>
#include <cstddef>

#include "FileMatrix/fmtypes.h"

namespace FMatrix
{
    namespace read
    {
    } //namespace read

    namespace mtread //multi-thread read
    {
        std::optional<std::vector<std::byte>> read_file(
        /*__In__*/      const std::filesystem::path& file_path,
        /*__In__*/      size_t thread_count,
        /*__In_Out__*/  std::error_code& ec);

    } //namespace mtread
}