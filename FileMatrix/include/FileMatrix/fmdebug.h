#pragma once

#include <source_location>

class Logger
{
public:
	Logger(std::source_location& srcLocation);
	~Logger();

private:
	std::source_location m_SL;

};