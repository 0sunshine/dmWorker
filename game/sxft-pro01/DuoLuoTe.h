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
    void GoToNextPos();

    void MoveToPos(const Point& pos);

    //bool IsInAutoFightMode();

    void EnableAutoFightMode();

    bool IsMapDescFound();
    void OpenMap();
    void CloseMap();

    std::optional<Point> IsAutoFightUIFound();
    void OpenAutoFightUIAndStart();
    void CloseAutoFightUI();


    void CheckAndUseSkill2();

private:
    dmsoft* _dm;
    long _wnd;

    const char* _currPosColorFmt = "00ff00-000000";

    DiffClock _skill2;
    DiffClock _moveClock;

    Point _currPos{ -1, -1 };
    int64_t _currRunCount = 0;
};
