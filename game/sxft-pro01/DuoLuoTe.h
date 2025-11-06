#pragma once

#include "dm/obj.h"
#include "util.h"

#include <optional>

class DuoLuoTe
{
public:
    DuoLuoTe(dmsoft* dm, long wnd);

    void doit();

private:
    void MoveToPos(const Point& pos);

private:
    dmsoft* _dm;
    long _wnd;

    const char* _currPosColorFmt = "00ff00-000000";
};
