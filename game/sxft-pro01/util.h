#pragma once

#include "dm/obj.h"

#include <optional>

struct Point
{
    int x = -1;
    int y = -1;
};

//1280 * 1024
std::optional<Point> GetCurrPos(dmsoft* dm);
