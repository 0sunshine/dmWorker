#include "DuoLuoTe.h"

#include <thread>
#include <stdio.h>
#include <format>
#include <iostream>
#include <chrono>
#include <filesystem>


DuoLuoTe::DuoLuoTe(dmsoft* dm, long wnd)
{
    _dm = dm;
    _wnd = wnd;
}

//1280 * 1024


static std::vector<Point> track = {
    {354, 593},
    {527, 547},
    {},
    {}
};


void DuoLuoTe::doit()
{

    MoveToPos(track[0]);
    auto pos = GetCurrPos(_dm);
    if (pos)
    {
        std::cout << std::format("curr pos: ({}, {})", pos->x, pos->y) << std::endl;
    }

    //while (true)
    //{

    //    auto pos = GetCurrPos(_dm);
    //    if (!pos)
    //    {
    //        continue;
    //    }
    //    std::cout << std::format("curr pos: ({}, {})", pos->x, pos->y) << std::endl;

    //    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    //}

}


void DuoLuoTe::MoveToPos(const Point& pos)
{
    std::filesystem::path bmpPath = "E:\\code\\dmWorker\\game\\sxft-pro01";

    std::string strMapDesc = (bmpPath / "mapDesc.bmp").string();


    long dm_ret = _dm->Capture(487, 378, 614, 405, strMapDesc.c_str());
    if (dm_ret <= 0)
    {
        std::cout << std::format("Capture failed in MoveToPos") << std::endl;
        return;
    }



}
