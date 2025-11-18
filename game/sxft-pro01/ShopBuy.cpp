#include "ShopBuy.h"

#include <thread>
#include <stdio.h>
#include <format>
#include <iostream>
#include <chrono>

ShopBuy::ShopBuy(dmsoft* dm, long wnd)
{
    _dm = dm;
    _wnd = wnd;
}

void ShopBuy::doit()
{
    _dm->SetWindowState(_wnd, 8);
    _dm->SetWindowState(_wnd, 1);

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    int count = 3000;
    while (count--)
    {
        //GetYB();

        //ExchangeZhuangShengShiTou();

        BuyOnce(820, 622); //紫色月光石精华位置 第四行第二个
        BuyOnce(596, 693);  //绿色月光石精华位置 第五行第一个

        //std::this_thread::sleep_for(std::chrono::milliseconds(50));

        std::cout << std::format("remain count : {}", count) << std::endl;
    }
}

void ShopBuy::BuyOnce(int goodX, int goodY)
{
    //(820, 622) 紫色月光石精华
    //(596, 693) 绿色月光石精华

    //int goodX = 596;
    //int goodY = 693;

    const int delayms = 5;

    _dm->MoveTo(goodX, goodY);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayms));

    _dm->MoveTo(goodX, goodY);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayms));

    _dm->LeftClick();
    std::this_thread::sleep_for(std::chrono::milliseconds(delayms));

    long x = -1, y = -1;
    while (x < 0 || y < 0)
    {
        _dm->AiFindPic(0, 0, 2000, 2000, _shopBuyBmpPath, 0.9, 0, &x, &y);
        std::this_thread::sleep_for(std::chrono::milliseconds(delayms));
        std::cout << std::format("not find  pos: {}, try again", _shopBuyBmpPath) << std::endl;
    }

    std::cout << std::format("find pos: {}, {}", x, y) << std::endl;

    {
        _dm->MoveTo(x + 92, y - 13);
        std::this_thread::sleep_for(std::chrono::milliseconds(delayms));

        _dm->MoveTo(x + 93, y - 13);
        std::this_thread::sleep_for(std::chrono::milliseconds(delayms));

        //_dm->MoveTo(x + 94, y - 13);
        //std::this_thread::sleep_for(std::chrono::milliseconds(delayms));
    }

    _dm->LeftClick();
    std::this_thread::sleep_for(std::chrono::milliseconds(delayms));

    _dm->KeyPressChar("2");
    std::this_thread::sleep_for(std::chrono::milliseconds(delayms));

    _dm->KeyPressChar("7");
    std::this_thread::sleep_for(std::chrono::milliseconds(delayms));

    {
        _dm->MoveTo(x + 50, y + 45);
        std::this_thread::sleep_for(std::chrono::milliseconds(delayms));

        _dm->MoveTo(x + 51, y + 45);
        std::this_thread::sleep_for(std::chrono::milliseconds(delayms));

        //_dm->MoveTo(x + 52, y + 45);
        //std::this_thread::sleep_for(std::chrono::milliseconds(delayms));
    }

    _dm->LeftClick();
    std::this_thread::sleep_for(std::chrono::milliseconds(delayms));

    x = -1, y = -1;
    while (x < 0 || y < 0)
    {
        _dm->MoveTo(0, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(delayms));

        _dm->AiFindPic(0, 0, 2000, 2000, _shopBuyOKBmpPath, 0.9, 0, &x, &y);
        std::this_thread::sleep_for(std::chrono::milliseconds(delayms));
        std::cout << std::format("not find  pos: {}, try again", _shopBuyOKBmpPath) << std::endl;
    }

    {
        _dm->MoveTo(x + 95, y + 82);
        std::this_thread::sleep_for(std::chrono::milliseconds(delayms));

        _dm->MoveTo(x + 96, y + 83);
        std::this_thread::sleep_for(std::chrono::milliseconds(delayms));

        //_dm->MoveTo(x + 95, y + 82);
        //std::this_thread::sleep_for(std::chrono::milliseconds(delayms));
    }

    _dm->LeftClick();
    std::this_thread::sleep_for(std::chrono::milliseconds(delayms));
}

void ShopBuy::ExchangeZhuangShengShiTou()
{
    {
        _dm->MoveTo(573, 492);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        _dm->MoveTo(572, 492);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        _dm->MoveTo(573, 492);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    _dm->LeftClick();
    std::this_thread::sleep_for(std::chrono::milliseconds(150));

    {
        _dm->WheelDown();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        _dm->WheelDown();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        _dm->WheelDown();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    {
        _dm->MoveTo(436, 597);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        _dm->MoveTo(436, 598);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        _dm->MoveTo(436, 597);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    _dm->LeftClick();
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
}

void ShopBuy::GetYB()
{
    {
        _dm->MoveTo(642, 535);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        _dm->MoveTo(641, 535);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        _dm->MoveTo(642, 535);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    _dm->LeftClick();
    std::this_thread::sleep_for(std::chrono::milliseconds(150));


    //

    {
        _dm->MoveTo(460, 525);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        _dm->MoveTo(461, 525);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        _dm->MoveTo(461, 525);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    _dm->LeftClick();
    std::this_thread::sleep_for(std::chrono::milliseconds(150));

    //

    {
        _dm->MoveTo(461, 508);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        _dm->MoveTo(462, 508);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        _dm->MoveTo(461, 508);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    _dm->LeftClick();
    std::this_thread::sleep_for(std::chrono::milliseconds(150));

    //
    _dm->KeyPressChar("esc");
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
}
