#include "pch.h"
#include "CppUnitTest.h"
#include "FileMatrix\FileMatrix.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace tests
{
	TEST_CLASS(Monitor)
	{
	public:
		
        TEST_METHOD(FindProcess)
        {
            std::wstring filePath;
            WCHAR buffer[MAX_PATH] = { 0 }; // Allocate buffer
            GetCurrentDirectory(MAX_PATH, buffer); // Get current directory
            filePath=std::wstring(buffer) + L"\\test.txt";
            std::wofstream writer(filePath, std::ios::out | std::ios::binary);
            if (writer.is_open()) {
                DWORD pid = GetCurrentProcessId();
                auto res = fmatrix::monitor::find_process(filePath);
                Assert::AreEqual(pid, res[0].pid);
                writer.close();
                std::filesystem::remove(filePath);
            }
            else {
                Assert::Fail();
            }
        }
	};
}
