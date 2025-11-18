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
#include "DuoLuoTe.h"
#include "ShopBuy.h"

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


dmsoft* g_dm = nullptr;


int subDM()
{
    // 初始化COM(sta)
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

    std::cout << std::format("start sub dm") << std::endl;

    auto dm = new dmsoft;
    long dm_ret = 0;

    //dm_ret = dm->LoadAi("ai.module");
    //if (dm_ret != 1)
    //{
    //    std::cout << std::format("LoadAi failed!!!") << std::endl;
    //    return -1;
    //}


    //dm_ret = dm.BindWindowEx(hwnd, "gdi", "windows3", "dx.keypad.input.lock.api|dx.keypad.state.api|dx.keypad.api", "", 0)

    CString strPids = dm->EnumProcess("client.dat");
    std::cout << std::format("{}", strPids.GetBuffer()) << std::endl;

    std::vector<long> vecPids;

    {
        int iStartPos = 0;
        CString strTmp;
        do
        {
            strTmp = strPids.Tokenize(",", iStartPos);
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
        long hWnd = Str2Num<long>(windows);
        CString titles = dm->GetWindowTitle(hWnd);

        std::cout << std::format("windows title: {}", titles.GetString()) << std::endl;

        const char* wantTitle = "02";

        if (titles.Find(wantTitle) < 0)
        {
            std::cout << std::format("wanna title: {}, skip this", wantTitle) << std::endl;
            continue;
        }



        dm_ret = dm->BindWindowEx(hWnd, "gdi", "normal", "normal", "", 0);
        if (dm_ret <= 0)
        {
            std::cout << std::format("BindWindowEx error, ret: {}", dm_ret) << std::endl;
            break;
        }

        std::cout << std::format("BindWindowEx ok, {}", hWnd) << std::endl;

        //dm_ret = dm->Capture(0, 0, 2000, 2000, "xxx.bmp");

        //if (dm_ret <= 0)
        //{
        //    std::cout << std::format("Capture failed") << std::endl;
        //    break;
        //}

        //std::cout << std::format("Capture ok") << std::endl;

        //WishScript wishScript(dm, hWnd);

        //wishScript.doit();

        //while (true)
        //{
        //    int i = 5;

        //    dm->SetWindowState(hWnd, 1);

        //    while (i--)
        //    {
        //        dm->KeyPressChar("2");
        //        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        //    }

        //    std::this_thread::sleep_for(std::chrono::milliseconds(11000));
        //}

        //DuoLuoTe duoLuoTe(dm, hWnd);
        //duoLuoTe.doit();

        ShopBuy shopBuy(dm, hWnd);
        shopBuy.doit();

        break;
    }

    return 0;
}



// 钩子回调函数
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        KBDLLHOOKSTRUCT* pKey = (KBDLLHOOKSTRUCT*)lParam;
        if (pKey->vkCode == VK_F12) {
            if (wParam == WM_KEYDOWN) {
                printf("检测到 F2 按下\n");
                exit(-1);
            }
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main()
{
    srand(102400);

    LoadConfig();

    // 安装全局键盘钩子
    HHOOK hHook = SetWindowsHookExW(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandleW(NULL), 0);
    if (!hHook) {
        printf("钩子安装失败: %d\n", GetLastError());
        return -1;
    }

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

    g_dm = new dmsoft;
    // 注册
    long dm_ret = g_dm->Reg(strRegCode.c_str(), strAdditionCode.c_str());
    if (dm_ret != 1)
    {
        std::cout << std::format("注册失败: {}, ver: {}", dm_ret, g_dm->Ver().GetString()) << std::endl;
        return -1;
    }

    std::cout << std::format("注册成功") << std::endl;


    dm_ret = g_dm->LoadAi("ai.module");
    if (dm_ret != 1)
    {
        std::cout << std::format("LoadAi failed!!!") << std::endl;
        return -1;
    }
    std::cout << std::format("LoadAi ok") << std::endl;


    std::thread thr(subDM);

    // 消息循环
    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    // 卸载钩子（程序退出时）
    UnhookWindowsHookEx(hHook);
}
