#include "util.h"

#include <format>
#include <boost/json.hpp>
#include <boost/asio.hpp>
#include <boost/process.hpp>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <regex>

namespace asio = boost::asio;
namespace process = boost::process;

std::optional<Point> GetCurrPos(dmsoft* dm)
{   
    std::filesystem::path bmpPath = "E:\\code\\dmWorker\\game\\sxft-pro01";

    std::string strCoordinate = (bmpPath / "coordinate.bmp").string();


    long dm_ret = dm->Capture(1140, 130, 1230, 155, strCoordinate.c_str());
    if (dm_ret <= 0)
    {
        std::cout << std::format("Capture failed in GetCurrPos") << std::endl;
        return std::nullopt;
    }

    //BGR打开
    cv::Mat img = cv::imread(strCoordinate.c_str());
    if (img.empty()) {
        std::cout << "无法读取图像" << std::endl;
        return std::nullopt;
    }

    //cvtColor(img, img, cv::COLOR_BGR2RGB);
    cv::Vec3b target(0, 0xff, 0);

    //std::cout << std::format("图片大小: {} * {}", img.cols, img.rows) << std::endl;


    // 遍历所有像素，非目标颜色直接置为黑色(0,0,0)
    for (int y = 0; y < img.rows; y++) {
        for (int x = 0; x < img.cols; x++) {
            cv::Vec3b& pixel = img.at<cv::Vec3b>(y, x);

            if (pixel[0] == 0x00 && pixel[1] == 0xFF && pixel[2] == 0x00)
            {
                continue;
            }

            pixel[0] = pixel[1] = pixel[2] = 0x00;
        }
    }


    cv::imwrite(strCoordinate.c_str(), img);

    asio::io_context ctx;
    asio::readable_pipe rp{ ctx };
    std::string output;

    try
    {
        std::string ocrPath = "E:\\code\\dmWorker\\PaddleOCR-json_v1.4.1";
        boost::process::process  proc(ctx, std::format("{}\\PaddleOCR-json.exe", ocrPath), { std::format("-models_path={}\\models", ocrPath) ,std::format("-image_path={}", strCoordinate)},
            boost::process::process_stdio{ {}, {rp}, nullptr });

        boost::system::error_code ec;
        asio::read(rp, asio::dynamic_buffer(output), ec);

        proc.wait();

        auto jsonStart = output.find("{");
        if (jsonStart == std::string::npos)
        {
            std::cout << std::format("no json, output: {}", output) << std::endl;
            return std::nullopt;
        }

        output.erase(0, jsonStart);
        

        auto jsonRoot = boost::json::parse(output).as_object();
        //{ "code":100, "data" : [{"box": [[8, 3], [75, 3], [75, 25], [8, 25]], "score" : 0.924835311041938, "text" : "(194,108)"}] }

        int code = jsonRoot["code"].as_int64();
        if (code != 100)
        {
            std::cout << std::format("error, PaddleOCR-json.exe code: {}", code) << std::endl;
            return std::nullopt;
        }

        boost::json::array data = jsonRoot["data"].as_array();
        if (1 != data.size())
        {
            std::cout << std::format("error, PaddleOCR-json.exe data.size: {}", data.size()) << std::endl;
            return std::nullopt;
        }

        auto& ocrInfoNode = data[0].as_object();

        double score = ocrInfoNode["score"].as_double();
        if (score < 0.9)
        {
            std::cout << std::format("warning, PaddleOCR-json.exe score: {}", score) << std::endl;
        }

        std::string strText = ocrInfoNode["text"].as_string().data();

        std::regex re("[^0-9]*([0-9]{1,3})\\s?,\\s?([0-9]{1,3})[^0-9]*");
        std::smatch result;
        if (!std::regex_match(strText, result, re))
        {
            std::cout << std::format("error, PaddleOCR-json.exe invaild text: {}", strText) << std::endl;
            return std::nullopt;
        }



        Point p;
        p.x = std::stoi(result[1].str());
        p.y = std::stoi(result[2].str());

        return p;
    }
    catch (const std::exception& e)
    {
        std::cout << std::format("PaddleOCR-json.exe exception: {}", e.what()) << std::endl;
        return std::nullopt;
    }

    return std::nullopt;
}

