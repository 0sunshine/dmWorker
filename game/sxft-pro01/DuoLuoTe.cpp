#include "DuoLuoTe.h"

#include <thread>
#include <stdio.h>
#include <format>
#include <iostream>
#include <chrono>
#include <filesystem>
#include <boost/json.hpp>
#include <boost/asio.hpp>
#include <boost/process.hpp>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <regex>
#include <boost/algorithm/string.hpp>
#include <string>

namespace asio = boost::asio;
namespace process = boost::process;

DuoLuoTe::DuoLuoTe(dmsoft* dm, long wnd)
{
    _dm = dm;
    _wnd = wnd;
}

//1280 * 1024





void DuoLuoTe::doit()
{

    long dm_ret = _dm->AiYoloSetModel(0, "datasets\\duoluote\\yolov5s_best.onnx", "");
    if (dm_ret < 1)
    {
        std::cout << std::format("AiYoloSetModel error") << std::endl;
        return;
    }

    //_dm->AiEnableFindPicWindow(1);

    DiffClock yoloDedectClock;

    while (true)
    {
        //CheckAndUseSkill2();

        std::cout << std::format("try AiYoloDetectObjects") << std::endl;
        std::string strObjs = _dm->AiYoloDetectObjects(0, 0, 2000, 2000, 0.8, 0.5).GetString();
        std::cout << std::format("AiYoloDetectObjects: {}", strObjs) << std::endl;

        std::vector<std::string> vecObjs;

        if (!strObjs.empty())
        {
            boost::split(vecObjs, strObjs, boost::is_any_of("|"));
        }


        if (!vecObjs.empty())
        {
            yoloDedectClock.Update();

            //打第一个检测出来的boss
            std::vector<std::string> vecAttrs;
            boost::split(vecAttrs, vecObjs[0], boost::is_any_of(","));

            int clickX = std::stoi(vecAttrs[2]) + std::stoi(vecAttrs[4]) / 2;
            int clickY =std::stoi(vecAttrs[3]) + std::stoi(vecAttrs[5]) / 2;


            int c = 20;
            while (c--)
            {
                _dm->MoveTo(clickX, clickY);
                _dm->LeftClick();
                std::this_thread::sleep_for(std::chrono::milliseconds(100));

                CheckAndUseSkill2();
            }

            EnableAutoFightMode();

            CheckAndUseSkill2();

            c = 100;
            while (c--)
            {
                CheckAndUseSkill2();
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                yoloDedectClock.Update();
            }
        }
        else if(yoloDedectClock.DiffWithLastMs() > 5000) //移动
        {
             GoToNextPos();
        }


        //MoveToPos(track[0]);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    
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

static std::vector<Point> track = {
    {354, 593},
    {527, 547},
    {460, 595},
    {388, 723},
    {655, 643},
    {821, 530},
    {533, 319},
    {745, 309},
    {730, 407},
    {960, 545},
    {903, 359},
    {612, 468},
};

void DuoLuoTe::GoToNextPos()
{
    if (_currPos.x < 0)
    {
        _currPos = track.front();
        ++_currRunCount;
    }

    //auto pos = GetCurrPos(_dm);
    //if (!pos)
    //{
    //    std::cout << std::format("GetCurrPos error", pos->x, pos->y) << std::endl;
    //    return;
    //}

    //int diffX = abs(_currPos.x - pos->x);
    //int diffY = abs(_currPos.y - pos->y);

    //if (diffX < 10 && diffY < 10) //认为已经到了目的地
    //{
    //    _currPos = track[_currRunCount % track.size()];
    //}

    if (_moveClock.DiffWithLastMs() > 30000)
    {
        //_currPos = track[rand() % track.size()];
        _currPos = track[_currRunCount++ % track.size()];
        _moveClock.Update();
    }
   

    //std::cout << std::format("diff pos: ({},{}),  try move to ({},{})", diffX, diffY, _currPos.x, _currPos.y) << std::endl;
    MoveToPos(_currPos);
}

void DuoLuoTe::MoveToPos(const Point& pos)
{
    OpenMap();

    _dm->MoveTo(pos.x, pos.y);
    _dm->LeftClick();

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    _dm->MoveTo(pos.x, pos.y);
    _dm->LeftClick();
    
    CloseMap();
}

//bool DuoLuoTe::IsInAutoFightMode()
//{
//    std::cout << std::format("ennter IsInAutoFightMode check... ") << std::endl;
//    std::filesystem::path bmpPath = "E:\\code\\dmWorker\\game\\sxft-pro01";
//    std::string strAutoFight = (bmpPath / "auto_fight_btn.bmp").string();
//
//    long dm_ret = _dm->Capture(0, 200, 1000, 650, strAutoFight.c_str());
//    if (dm_ret <= 0)
//    {
//        std::cout << std::format("Capture failed in IsInAutoFightMode") << std::endl;
//        return false;
//    }
//    
//    return true;
//}

void DuoLuoTe::EnableAutoFightMode()
{
    //if (!IsInAutoFightMode())
    //{
    //    std::cout << std::format("not auto fight, try auto fight ...") << std::endl;

    //    _dm->KeyDownChar("ctrl");
    //    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    //    _dm->KeyPressChar("z");
    //    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    //    _dm->KeyUpChar("ctrl");

    //    //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    //}

    //std::cout << std::format("auto fighting ...") << std::endl;

    std::cout << std::format("enter EnableAutoFightMode ...") << std::endl;
    OpenAutoFightUIAndStart();
    CloseAutoFightUI();
}

bool DuoLuoTe::IsMapDescFound()
{
    std::string bmpPath = "E:\\code\\dmWorker\\game\\sxft-pro01\\icon\\mapDesc.bmp";
    long x = 0, y = 0;
    int ret = _dm->AiFindPic(250, 290, 420, 330, bmpPath.c_str(), 0.9, 0, &x, &y);

    return ret >= 0;
}

void DuoLuoTe::OpenMap()
{
    _dm->SetWindowState(_wnd, 1);

    long x = 0, y = 0;
    bool isFound = IsMapDescFound();

    while (!isFound)
    {
        std::cout << std::format("map not open, try open ...") << std::endl;

        _dm->SetWindowState(_wnd, 1);
        _dm->KeyPressChar("m");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        isFound = IsMapDescFound();
    }

    std::cout << std::format("map opened ...") << std::endl;
}

void DuoLuoTe::CloseMap()
{
    _dm->SetWindowState(_wnd, 1);

    long x = 0, y = 0;
    bool isFound = IsMapDescFound();

    while (isFound)
    {
        std::cout << std::format("map not close, try close ...") << std::endl;

        _dm->SetWindowState(_wnd, 1);
        _dm->KeyPressChar("m");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        isFound = IsMapDescFound();
    }

    std::cout << std::format("map closed ...") << std::endl;
}

std::optional<Point> DuoLuoTe::IsAutoFightUIFound()
{
    _dm->MoveTo(0, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    _dm->MoveTo(0, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    _dm->MoveTo(0, 0);
    
    std::string bmpPath = "E:\\code\\dmWorker\\game\\sxft-pro01\\icon\\auto_fight_btn.bmp";
    long x = 0, y = 0;
    int ret = _dm->AiFindPic(0, 200, 1000, 650, bmpPath.c_str(), 0.9, 0, &x, &y);


    if (ret >= 0)
    {
        return Point{x, y};
    }

    return std::nullopt;
}

void DuoLuoTe::OpenAutoFightUIAndStart()
{
    _dm->SetWindowState(_wnd, 1);

    long x = 0, y = 0;

    std::optional<Point> picPoint = IsAutoFightUIFound();

    while (!picPoint)
    {
        std::cout << std::format("fight ui not open, try open ...") << std::endl;

        _dm->SetWindowState(_wnd, 1);
        _dm->KeyPressChar("z");
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        picPoint = IsAutoFightUIFound();
    }

    std::cout << std::format("fight ui opened ...") << std::endl;

    //146,55
    _dm->MoveTo(picPoint->x + 146, picPoint->y + 56);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    _dm->LeftClick();

    _dm->MoveTo(picPoint->x + 148, picPoint->y + 55);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    _dm->LeftClick();

    _dm->MoveTo(picPoint->x + 148, picPoint->y + 56);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    _dm->LeftClick();
}

void DuoLuoTe::CloseAutoFightUI()
{
    _dm->SetWindowState(_wnd, 1);

    long x = 0, y = 0;
    std::optional<Point> picPoint = IsAutoFightUIFound();

    while (picPoint)
    {
        std::cout << std::format("fight ui opening, try close ...") << std::endl;

        _dm->SetWindowState(_wnd, 1);
        _dm->KeyPressChar("z");
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        picPoint = IsAutoFightUIFound();
    }

    std::cout << std::format("fight ui closed ...") << std::endl;
}

void DuoLuoTe::CheckAndUseSkill2()
{
    if (_skill2.DiffWithLastMs() > 22 * 1000)
    {
        std::cout << std::format("use skill 2 ...") << std::endl;
        int i = 6;
        while (i--)
        {
            _dm->KeyPressChar("2");
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        _skill2.Update();
    }
}

