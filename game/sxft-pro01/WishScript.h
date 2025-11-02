#pragma once

#include "dm/obj.h"

class WishScript
{
public:
    WishScript(dmsoft* dm, long wnd);

    void doit();

    bool OpenShop();
    bool StartWish();
    bool CloseShop();

private:
    dmsoft* _dm;
    long _wnd;
};

