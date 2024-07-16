// FileMatrix.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "FileMatrix.h"

namespace utility
{
    std::string to_string(const std::wstring& wstr)
    {
        int count = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.length(), NULL, 0, NULL, NULL);
        std::string str(count, 0);
        WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &str[0], count, NULL, NULL);
        return str;
    }

    std::wstring to_wstring(const std::string& str)
    {
        int count = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), NULL, 0);
        std::wstring wstr(count, 0);
        MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), &wstr[0], count);
        return wstr;
    }
}

namespace fmatrix {

    template<typename T>
    T dbg_return(const std::string& origin, const std::string& subMethod, DWORD error)
    {
        std::string tmp = std::format("{} @ {} failed with error code: {}.", origin, subMethod, std::to_string(error));
        std::wstring dbgMsg = utility::to_wstring(tmp);
        OutputDebugString(dbgMsg.c_str());
        return T();
    }


    namespace monitor
    {
        std::vector<process_info> find_process(const std::filesystem::path& filePath)
    {
        if (filePath.empty() || !std::filesystem::exists(filePath))
        {
            OutputDebugString(L"fmatrix::monitor::find_process got wrong entry. Check file path.");
            return {};
        }

        DWORD dwSession{};
        WCHAR szSessionKey[CCH_RM_SESSION_KEY + 1]{};
        DWORD dwError = RmStartSession(&dwSession, 0, szSessionKey);
        if (dwError != ERROR_SUCCESS)
        {
            return dbg_return<std::vector<process_info>>("::find_process", "RmStartSession", dwError);
        }

        PCWSTR pszFile = filePath.c_str();
        dwError = RmRegisterResources(dwSession, 1, &pszFile, 0, NULL, 0, NULL);
        if (dwError != ERROR_SUCCESS)
        {
            return dbg_return<std::vector<process_info>>("::find_process", "RmRegisterResources", dwError);
        }

        DWORD dwReason{};
        UINT nProcInfoNeeded{};
        UINT nProcInfo{};
        RmGetList(dwSession, &nProcInfoNeeded, &nProcInfo, NULL, &dwReason);
        if (nProcInfoNeeded == 0)
        {
            OutputDebugString(L"::find_process finished with no result");
            return {};
        }

        std::vector<RM_PROCESS_INFO> rgpi(nProcInfoNeeded);
        nProcInfo = nProcInfoNeeded;
        dwError = RmGetList(dwSession, &nProcInfoNeeded, &nProcInfo, rgpi.data(), &dwReason);
        if (dwError != ERROR_SUCCESS)
        {
            return dbg_return<std::vector<process_info>>("::find_process", "RmGetList", dwError);
        }
        else if (nProcInfo == 0)
        {
            return {};
        }

        std::vector<process_info> result;
        for (UINT i = 0; i < nProcInfo; i++) {
            result.emplace_back(process_info(
                rgpi[i].strAppName,
                rgpi[i].Process.dwProcessId));
        }
        RmEndSession(dwSession);
        return result;
    }
    }
}