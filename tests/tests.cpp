#include "pch.h"
#include "CppUnitTest.h"
#include "FileMatrix.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Monitor
{
	TEST_CLASS(FindProces)
	{
	public:
        TEST_METHOD(Pass)
        {
            std::wstring filePath = std::filesystem::current_path().wstring() + L"\\test.txt";
            std::wofstream writer(filePath, std::ios::out | std::ios::binary);
            if (writer.is_open()) {
                DWORD pid = GetCurrentProcessId();
                auto res = FMatrix::monitor::find_process(filePath);
                FMatrix::process_info m{};
                
                Assert::AreEqual(pid, res[0].pid);
                writer.close();
                std::filesystem::remove(filePath);
            }
            else {
                Assert::Fail();
            }
        }
        TEST_METHOD(WrongFilePath)
        {
            std::wstring filePath = std::filesystem::current_path().wstring() + L"\\test.txt";
            if (!std::filesystem::exists(filePath)) {
                auto res = FMatrix::monitor::find_process(filePath);
                Assert::AreEqual(res.size(), size_t(0));
            }
            else {
                std::filesystem::remove(filePath);
                auto res = FMatrix::monitor::find_process(filePath);
                Assert::AreEqual(res.size(), size_t(0));
            }
        }
        TEST_METHOD(FileNotUsed)
        {
            std::wstring filePath = std::filesystem::current_path().wstring() + L"\\test.txt";
            std::wofstream writer(filePath, std::ios::out | std::ios::binary);
            if (writer.is_open()) {
                writer << "For Test.";
                writer.close();
                auto res = FMatrix::monitor::find_process(filePath);
                Assert::IsFalse(res.size());
                std::filesystem::remove(filePath);
            }
            else {
                Assert::Fail();
            }
        }
	};
}

namespace MultiThreadRead {
    TEST_CLASS(ReadFile) {
        TEST_METHOD(MultipleThread) {
            //StartUp
            std::filesystem::path fPath = std::filesystem::current_path() / "test.txt";
            std::ofstream writer(fPath, std::ios::out);
            std::string sample{ "HELLO" };

            for (const auto& ch : sample) {
                for (int i{}; i<100 ; i++)
                    writer << ch;
                writer << "\n";
            } // 510 character (10 char of \r \n)

            writer.close();
            std::error_code ec;
            FMatrix::MT::Reader reader{ fPath, 5};
            auto res = reader.Read(ec);
            Assert::IsTrue(ec.value() == 0);
            Assert::IsTrue(res.size() == 510);

            //TearDown
            std::filesystem::remove(fPath, ec);
        }
    };
}
