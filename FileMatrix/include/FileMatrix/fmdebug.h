#pragma once

#include <source_location>

class Logger
{
public:
	Logger(std::source_location& srcLocation) noexcept :m_SL(srcLocation) {};

	~Logger() noexcept = default;

private:
	std::source_location m_SL;
};