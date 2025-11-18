#pragma once

#include "dm/obj.h"
#include "util.h"

#include <optional>

class ShopBuy
{
public:
    ShopBuy(dmsoft* dm, long wnd);

    void doit();
    void BuyOnce(int goodX, int goodY);
    void ExchangeZhuangShengShiTou();
    void GetYB();
    //std::optional<Point> FindBmp(const char* path);

private:
    dmsoft* _dm;
    long _wnd;

    const char* _shopBuyBmpPath = "./icon/shop_buy.bmp";
    const char* _shopBuyOKBmpPath = "./icon/shop_buy_ok.bmp";
    //const char* _WishStartBtnBmpPath = "./icon/wish_start_btn.bmp";
    //const char* _WishPoolBmpPath = "./icon/wish_pool.bmp";
};
