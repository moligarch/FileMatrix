// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include <span>
#include <mutex>
#include <memory>
#include <thread>
#include <future>
#include <format>
#include <string>
#include <vector>
#include <fstream>
#include <optional>
#include <iostream>
#include <filesystem>
#include <system_error>
#include <source_location>

#include "Windows.h"
#include "RestartManager.h"
#endif //PCH_H
