#include "pch.h"
#include "FileMatrix/fmmonitor.h"
#include "FileMatrix/fmtypes.h"


namespace utility
{
    std::string to_string(const std::wstring& wstr)
    {
        const int count = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), int(wstr.length()), NULL, 0, NULL, NULL);
        std::string str(count, 0);
        WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &str.at(0), count, NULL, NULL);
        return str;
    }

    std::wstring to_wstring(const std::string& str)
    {
        const int count = MultiByteToWideChar(CP_ACP, 0, str.c_str(), int(str.length()), NULL, 0);
        std::wstring wstr(count, 0);
        MultiByteToWideChar(CP_ACP, 0, str.c_str(), int(str.length()), &wstr.at(0), count);
        return wstr;
    }

    template<typename T>
    T dbg_return(const std::string& error, const T* res = nullptr)
    {
        std::string tmp = std::format("Function ({}) @ Line({}) Failed. Description: {}.", __func__, __LINE__, error);
        std::wstring dbgMsg = utility::to_wstring(tmp);
        OutputDebugString(dbgMsg.c_str());
        if (res == nullptr) {
            return T();
        }
        return *res;
    }
}

#ifdef _DEBUG
#define RETURN(T,msg,obj) return utility::dbg_return<T>(msg,obj)
#else
#define RETURN(T,msg,obj) return *obj

#endif // _DEBUG

namespace FMatrix {
    namespace monitor
    {
        using process_info_list = std::vector<process_info>;
        process_info_list find_process(const std::filesystem::path& filePath)
        {
            OutputDebugString(L"====== FileMatrix ** find_process Called. ======");
            if (filePath.empty() || !std::filesystem::exists(filePath))
            {
                RETURN(std::vector<process_info>, "Path does not exist.", nullptr);
            }

            DWORD dwSession{};
            auto szSessionKey = std::make_unique < WCHAR[] >(CCH_RM_SESSION_KEY + 1);
            DWORD dwError = RmStartSession(&dwSession, 0, szSessionKey.get());
            if (dwError != ERROR_SUCCESS)
            {
                RETURN(process_info_list, "Error: " + std::to_string(dwError), nullptr);
            }

            PCWSTR pszFile = filePath.c_str();
            dwError = RmRegisterResources(dwSession, 1, &pszFile, 0, NULL, 0, NULL);
            if (dwError != ERROR_SUCCESS)
            {
                RETURN(process_info_list, "Error: " + std::to_string(dwError), nullptr);
            }

            DWORD dwReason{};
            UINT nProcInfoNeeded{};
            UINT nProcInfo{};
            RmGetList(dwSession, &nProcInfoNeeded, &nProcInfo, NULL, &dwReason);
            if (nProcInfoNeeded == 0)
            {
                RETURN(process_info_list, "Finished With No Result.", nullptr);
            }

            std::vector<RM_PROCESS_INFO> rgpi(nProcInfoNeeded);
            nProcInfo = nProcInfoNeeded;
            dwError = RmGetList(dwSession, &nProcInfoNeeded, &nProcInfo, rgpi.data(), &dwReason);
            if (dwError != ERROR_SUCCESS)
            {
                RETURN(process_info_list, "Error: " + std::to_string(dwError), nullptr);
            }
            else if (nProcInfo == 0)
            {
                RETURN(process_info_list, "Finished With No Result.(2nd)", nullptr);
            }

            std::vector<process_info> result;
            for (UINT i = 0; i < nProcInfo; i++) {
                result.emplace_back(process_info(
                    rgpi.at(i).strAppName,
                    rgpi.at(i).Process.dwProcessId));
            }
            RmEndSession(dwSession);
            RETURN(process_info_list, "Finished With No Result.", &result);
        }
    }
}