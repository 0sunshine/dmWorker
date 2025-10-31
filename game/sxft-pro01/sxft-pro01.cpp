// sxft-pro01.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "dm/obj.h"
#include "sxft-pro01.h"

#include <fstream>
#include <string>
#include <format>

CWinApp theApp;

const std::string strPasswdFile = "../../passwd.txt";

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
        delete dm;
        return -1;
    }

    std::cout << std::format("注册成功") << std::endl;


    std::cin.get();
    std::cin.get();
}
