#include "WishScript.h"

#include <thread>
#include <stdio.h>
#include <format>
#include <iostream>

WishScript::WishScript(dmsoft* dm, long wnd)
{
    _dm = dm;
    _wnd = wnd;
}

//1024 * 768
void WishScript::doit()
{
    std::cout << "enter WishScript::doit" << std::endl;

    //1 : 激活指定窗口
    //8 : 置顶指定窗口22
    //9 : 取消置顶指定窗口
    //_dm->SetWindowState(hWnd, 9);

    int count = 100;

    _dm->SetWindowState(_wnd, 8);

    while (count-- > 0)
    {
        _dm->SetWindowState(_wnd, 1);
        




        _dm->KeyPressChar("D");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        _dm->MoveTo(647, 449);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        _dm->LeftClick();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        _dm->KeyPressChar("esc");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    _dm->SetWindowState(_wnd, 9);

    std::cout << "leave WishScript::doit" << std::endl;
}

bool WishScript::OpenShop()
{
    return false;
}

bool WishScript::StartWish()
{
    return false;
}

bool WishScript::CloseShop()
{
    return false;
}
