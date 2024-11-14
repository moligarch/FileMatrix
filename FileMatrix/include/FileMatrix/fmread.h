#pragma once
#include <filesystem>
#include <vector>
#include <cstddef>

#include "FileMatrix/fmtypes.h"

namespace FMatrix
{
	class Reader final
	{
	public:
		Reader() = delete;
		Reader(std::filesystem::path file_path);
		~Reader();

		buffer_t Read();
		std::streamsize ReadChunk(buffer_t& buffer, size_t chunk_size);

	private:
		type::FMifstream file_;
	};

	class MTReader
	{
	public:
		MTReader() = delete;
		MTReader(std::filesystem::path file_path, size_t thread_count);
		~MTReader();

		buffer_t Read(std::error_code& ec);
		//std::streamsize ReadChunk(buffer_t& buffer, size_t chunk_size);

	private:
		type::FMifstream file_;
		size_t thread_count_;
	};
}