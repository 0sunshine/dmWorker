#include "WishScript.h"

#include <thread>
#include <stdio.h>
#include <format>
#include <iostream>
#include <chrono>

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

    int count = 1000;

    _dm->SetWindowState(_wnd, 8);
    _dm->SetWindowState(_wnd, 1);

    while (count-- > 0)
    {
        std::cout << " WishScript::doit，count：" << count << std::endl;

        auto shopBmpPoint = FindBmp(_shopBmpPath);
        int retryCount = 0;

        while (!shopBmpPoint)
        {
            ++retryCount = 0;
            if (retryCount > 3)
            {
                _dm->KeyPressChar("esc");
                std::this_thread::sleep_for(std::chrono::milliseconds(50));

                _dm->KeyPressChar("esc");
                std::this_thread::sleep_for(std::chrono::milliseconds(50));

                _dm->KeyPressChar("esc");
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
            //可能在其他界面, 先esc返回
            _dm->KeyPressChar("esc");
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            shopBmpPoint = FindBmp(_shopBmpPath);
        }


        //按D弹出商城
        _dm->MoveTo(0, 0); //防止鼠标遮挡导致找不到图
        _dm->KeyPressChar("D");
        auto wishBtnBmpPoint = FindBmp(_WishBtnBmpPath);
        while (!wishBtnBmpPoint)
        {
            //重试
            std::cout << "没找到许愿按钮，重试" << std::endl;
            wishBtnBmpPoint = FindBmp(_WishBtnBmpPath);
            if (wishBtnBmpPoint)
            {
                break;
            }

            _dm->MoveTo(0, 0); //防止鼠标遮挡导致找不到图
            _dm->KeyPressChar("D");
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            wishBtnBmpPoint = FindBmp(_WishBtnBmpPath);
        }


        _dm->MoveTo(wishBtnBmpPoint->x + 40, wishBtnBmpPoint->y + 40);
        _dm->LeftClick();
        auto wishStartBtnBmpPoint = FindBmp(_WishStartBtnBmpPath);
        while (!wishStartBtnBmpPoint)
        {

            auto wishPoolBtnBmpPoint = FindBmp(_WishPoolBmpPath);
            if (wishPoolBtnBmpPoint) //说明在许愿界面
            {
                auto wishBtnBmpPoint = FindBmp(_WishBtnBmpPath);
                if (!wishBtnBmpPoint) //许愿按钮看不到
                {
                    std::cout << "已在许愿界面" << std::endl;
                    break; //让下一次循环去退出
                }
            }
            else //不在许愿池了
            {
                break; //走到下一步去
            }


            //重试
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            _dm->MoveTo(wishBtnBmpPoint->x + 40, wishBtnBmpPoint->y + 40);
            _dm->LeftClick();
            wishStartBtnBmpPoint = FindBmp(_WishStartBtnBmpPath);
        }

    }

    _dm->SetWindowState(_wnd, 9);

    std::cout << "leave WishScript::doit" << std::endl;
}

std::optional<Point> WishScript::FindBmp(const char* path)
{

    long x = 0, y = 0;
    int ret = _dm->AiFindPic(0, 0, 1920, 1080, path, 0.9, 0, &x, &y);
    if (ret < 0)
    {
        std::cout << std::format("AiFindPic error, bmp: {}", path) << std::endl;
        return  std::nullopt;
    }

    return Point{ x, y };
}


//bool WishScript::OpenShop()
//{
//    auto start_tp = std::chrono::steady_clock::now();
//    auto curr_tp = start_tp;
//    auto timeout = std::chrono::milliseconds(2000);
//
//    while (std::chrono::duration_cast<std::chrono::milliseconds>(start_tp - curr_tp) > timeout)
//    {
//        long x = 0, y = 0;
//        int ret = _dm->AiFindPic(0, 0, 1920, 1080, _shopBmpPath, 0.9, 0, &x, &y);
//        if (ret >= 0)
//        {
//            std::cout << std::format("AiFindPic ({}, {}), bmp: {}", x, y, _shopBmpPath) << std::endl;
//            return  true;
//        }
//
//        _dm->KeyPressChar("esc");
//        std::this_thread::sleep_for(std::chrono::milliseconds(50));
//    }
//
//    std::cout << std::format("AiFindPic error, bmp: {}", _shopBmpPath) << std::endl;
//    return false;
//}

