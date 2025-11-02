// sxft-pro01.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "dm/obj.h"
#include "sxft-pro01.h"
#include "WishScript.h"

#include <type_traits>
#include <fstream>
#include <string>
#include <format>
#include <vector>
#include <sstream>
#include <thread>

CWinApp theApp;

const std::string strPasswdFile = "../../passwd.txt";

template <typename T>
T Str2Num(const CString& s)
{
    static_assert(std::is_integral_v<T>, "T must be num");

    std::istringstream ss(s.GetString());
    T v{};
    ss >> v;

    return v;
}


int main()
{
    // 初始化COM(sta)
    (void)CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

    // 设置本地字符集为gbk
    setlocale(LC_ALL, "chs");

    if (!AfxWinInit(::GetModuleHandle(nullptr), nullptr, ::GetCommandLine(), 0))
    {
        printf("AfxWinInit error\n");
        return -1;
    }

    std::ifstream fin(strPasswdFile);
    if (!fin)
    {
        std::cout << std::format("open file error: {}", strPasswdFile) << std::endl;
        return -1;
    }

    std::string strRegCode;
    std::getline(fin, strRegCode);

    std::string strAdditionCode;
    std::getline(fin, strAdditionCode);

    fin.close();

    std::cout << std::format("code:\n{}\n{}\n", strRegCode, strAdditionCode) << std::endl;

    dmsoft* dm = new dmsoft;
    // 注册
    long dm_ret = dm->Reg(strRegCode.c_str(), strAdditionCode.c_str());
    if (dm_ret != 1)
    {
        std::cout << std::format("注册失败: {}, ver: {}", dm_ret, dm->Ver().GetString()) << std::endl;
        return -1;
    }

    std::cout << std::format("注册成功") << std::endl;


    dm_ret = dm->LoadAi("ai.module");
    if (dm_ret != 1)
    {
        std::cout << std::format("LoadAi failed!!!") << std::endl;
        return -1;
    }


    //dm_ret = dm.BindWindowEx(hwnd, "gdi", "windows3", "dx.keypad.input.lock.api|dx.keypad.state.api|dx.keypad.api", "", 0)

    CString strPids = dm->EnumProcess("client.dat");
    std::cout << std::format("{}", strPids.GetBuffer()) << std::endl;

    std::vector<long> vecPids;

    {
        int iStartPos = 0;
        CString strTmp;
        do
        {
            strTmp = strPids.Tokenize("|", iStartPos);
            if (strTmp.IsEmpty())
            {
                break;
            }

            vecPids.push_back(Str2Num<long>(strTmp));

        } while (true);
    }


    for (auto pid : vecPids)
    {
        CString strInfo = dm->GetProcessInfo(pid);

        //dm->GetWindowClass()
        std::cout << std::format("pid: {}, info: {}", pid, strInfo.GetString()) << std::endl;


        CString windows = dm->EnumWindowByProcessId(pid, "", "", 16);
        std::cout << std::format("windows: {}", pid, windows.GetString()) << std::endl;

        
        long hWnd = Str2Num<long>(windows);
        dm_ret = dm->BindWindowEx(hWnd, "gdi", "normal", "normal", "", 0);
        if(dm_ret <= 0)
        {
            std::cout << std::format("BindWindowEx error, ret: {}", dm_ret) << std::endl;
            break;
        }

        std::cout << std::format("BindWindowEx ok, {}", hWnd) << std::endl;

        dm_ret = dm->Capture(0, 0, 2000, 2000, "xxx.bmp");

        if (dm_ret <= 0)
        {
            std::cout << std::format("Capture failed") << std::endl;
            break;
        }

        std::cout << std::format("Capture ok") << std::endl;

        long x = 0, y = 0;
        dm_ret = dm->FindStr(0, 0, 2000, 2000 , "一级", "ffffff-000000", 0.8, &x, &y);
        if (dm_ret < 0)
        {
            std::cout << std::format("FindStr failed") << std::endl;
            break;
        }


        std::cout << std::format("{},{}", x, y) << std::endl;

        
        //WishScript wishScript(dm, hWnd);

        //wishScript.doit();
    }




    std::cin.get();
    std::cin.get();
}
