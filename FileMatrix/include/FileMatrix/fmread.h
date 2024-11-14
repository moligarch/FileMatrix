#pragma once
#include <filesystem>

#include "FileMatrix/fmtypes.h"

namespace FMatrix
{
	class Reader final
	{
	public:
		Reader(const std::filesystem::path& file_path);
		~Reader();

		// Default constructor
		Reader() = delete;
		// Copy constructor
		Reader(const Reader&) = delete;
		// Copy assignment operator
		Reader& operator=(const Reader&) = delete;
		// Move constructor
		Reader(Reader&&) noexcept = default;
		// Move assignment operator
		Reader& operator=(Reader&&) noexcept = default;

		buffer_t Read();
		bool GetHash(Hash::Algorithm algorithm, std::string& output);
		std::streamsize ReadChunk(buffer_t& buffer, size_t chunk_size);

	private:
		Type::FMifstream file_;
	};
	namespace MT
	{
		class Reader
		{
		public:
			Reader(const std::filesystem::path& file_path, size_t thread_count);
			~Reader();

			// Default constructor
			Reader() = delete;
			// Copy constructor
			Reader(const Reader&) = delete;
			// Copy assignment operator
			Reader& operator=(const Reader&) = delete;
			// Move constructor
			Reader(Reader&&) noexcept = default;
			// Move assignment operator
			Reader& operator=(Reader&&) noexcept = default;
			
			buffer_t Read(std::error_code& ec);
			//std::streamsize ReadChunk(buffer_t& buffer, size_t chunk_size);

		private:
			Type::FMifstream file_;
			size_t thread_count_;
		};
	}
}