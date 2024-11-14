#pragma once
#include "FileMatrix/fmtypes.h"
#include <vector>
#include <filesystem>

namespace FMatrix
{
    namespace monitor {
        std::vector<process_info> find_process(const std::filesystem::path& filePath);
    }
}
