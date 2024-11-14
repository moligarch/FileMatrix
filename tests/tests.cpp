#include "pch.h"
#include "CppUnitTest.h"
#include "FileMatrix.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

void MakeTestFile(const std::filesystem::path& fPath)
{
    std::ofstream writer(fPath, std::ios::out);
    std::string sample{ "HELLO" };

    for (const auto& ch : sample) {
        for (int i{}; i < 100; i++)
            writer << ch;
        writer << "\n";
    } // 510 character (10 char of \r \n)

    writer.close();
}


namespace Monitor
{
	TEST_CLASS(FindProces)
	{
	public:
        TEST_METHOD(Pass)
        {
            auto filePath = std::filesystem::current_path() / "test.txt";
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
            auto filePath = std::filesystem::current_path() / "test.txt";
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
            auto filePath = std::filesystem::current_path() / "test.txt";
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

namespace FMRead
{
    TEST_CLASS(SingleThread)
    {
        TEST_METHOD(Reader_Read) {
            //StartUp
            std::filesystem::path fPath = std::filesystem::current_path() / "test.txt";
            MakeTestFile(fPath);
            {
                FMatrix::Reader reader{ fPath };
                auto res = reader.Read();
                Assert::IsTrue(res.size() == 510);
            }
            //TearDown
            std::filesystem::remove(fPath);
        }

        TEST_METHOD(Reader_GetHash_SHA256)
        {
            // StartUp
            std::filesystem::path fPath = std::filesystem::current_path() / "test.txt";
            MakeTestFile(fPath);
            {
                FMatrix::Reader reader{ fPath };
                std::string hash;
                bool done = reader.GetHash(FMatrix::Hash::Algorithm::SHA2_256, hash);
                std::string expectedHash{ "330C082C82A840F6EC8C5D48E227E5F3815F44FBFE88D2A503B905E837F4F259" };
                Assert::IsTrue(done);
                Assert::IsTrue(hash == expectedHash);
            }
            //TearDown
            std::filesystem::remove(fPath);
        }
    };

    TEST_CLASS(MultiTread)
    {
        TEST_METHOD(MT_Reader_Read) {
            //StartUp
            std::filesystem::path fPath = std::filesystem::current_path() / "test.txt";
            MakeTestFile(fPath);
            {
                std::error_code ec;
                FMatrix::MT::Reader reader{ fPath, 5 };
                auto res = reader.Read(ec);
                Assert::IsTrue(ec.value() == 0);
                Assert::IsTrue(res.size() == 510);
            }
            //TearDown
            std::filesystem::remove(fPath);
        }
    };


}
