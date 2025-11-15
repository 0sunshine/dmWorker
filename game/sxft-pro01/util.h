#pragma once

#include "dm/obj.h"

#include <optional>
#include <chrono>






struct Config
{
    std::string strPaddleOCRPath;
};

extern Config g_Config;

void LoadConfig();

struct Point
{
    int x = -1;
    int y = -1;
};


class DiffClock
{
public:
    void Update();
    int64_t DiffWithLastMs();

private:
    std::chrono::steady_clock::time_point _lastPt = std::chrono::steady_clock::now();
};

//1280 * 1024
std::optional<Point> GetCurrPos(dmsoft* dm);
