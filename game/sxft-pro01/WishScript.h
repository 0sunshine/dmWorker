#pragma once

#include "dm/obj.h"
#include "util.h"

#include <optional>



class WishScript
{
public:
    WishScript(dmsoft* dm, long wnd);

    void doit();

    std::optional<Point> FindBmp(const char* path);

private:
    dmsoft* _dm;
    long _wnd;

    const char* _shopBmpPath = "./icon/shop.bmp";
    const char* _WishBtnBmpPath = "./icon/wish_btn.bmp";
    const char* _WishStartBtnBmpPath = "./icon/wish_start_btn.bmp";
    const char* _WishPoolBmpPath = "./icon/wish_pool.bmp";
};

