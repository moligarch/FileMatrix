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
		Reader(const std::filesystem::path& file_path);
		~Reader();

		buffer_t Read();
		std::streamsize ReadChunk(buffer_t& buffer, size_t chunk_size);

	private:
		type::FMifstream file_;
	};
	namespace MT
	{
		class Reader
		{
		public:
			Reader() = delete;
			Reader(const std::filesystem::path& file_path, size_t thread_count);
			~Reader();

			buffer_t Read(std::error_code& ec);
			//std::streamsize ReadChunk(buffer_t& buffer, size_t chunk_size);

		private:
			type::FMifstream file_;
			size_t thread_count_;
		};
	}
}