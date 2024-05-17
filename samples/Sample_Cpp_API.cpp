#include "CLidar.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
using namespace std;
using namespace lidar;

#if defined(_MSC_VER)
#pragma comment(lib, "LIDAR_SDK.lib")
#endif

int main(int argc, char *argv[]) {
    printf("__   ______  _     ___ ____    _    ____  \n");
    printf("\\ \\ / /  _ \\| |   |_ _|  _ \\  / \\  |  _ \\ \n");
    printf(" \\ V /| | | | |    | || | | |/ _ \\ | |_) | \n");
    printf("  | | | |_| | |___ | || |_| / ___ \\|  _ <  \n");
    printf("  |_| |____/|_____|___|____/_/   \\_\\_| \\_\\ \n");
    printf("\n");
    fflush(stdout);
    std::string port;
    lidar::os_init();

    printf("Please enter the lidar IP:");
    std::cin >> port;

    if (!lidar::os_isOk()) {
        return 0;
    }

    std::string input_frequency;
    float frequency = 20.0;
    while (lidar::os_isOk()) {
        printf("Please enter the lidar scan frequency[10-30]:");
        std::cin >> input_frequency;
        frequency = atof(input_frequency.c_str());
        if (frequency <= 30 && frequency >= 10.0) {
            break;
        }
        fprintf(stderr,
                "Invalid scan frequency,The scanning frequency range is 10 to 30 HZ, Please re-enter.\n");
    }

    if (!lidar::os_isOk()) {
        return 0;
    }

    CLidar lidar;

    //////////////////////string property/////////////////  
    lidar.setlidaropt(LidarPropSerialPort, port.c_str(), port.size());///雷达ip

    //////////////////////int property/////////////////
    int optval = 8090;
    lidar.setlidaropt(LidarPropSerialBaudrate, &optval, sizeof(int));///tcp端口，用于配置

    optval = TYPE_LIDAR;
    lidar.setlidaropt(LidarPropLidarType, &optval, sizeof(int));///雷达型号

    //////////////////////bool property/////////////////
    bool b_optvalue = true;
    lidar.setlidaropt(LidarPropAutoReconnect, &b_optvalue, sizeof(bool));///是否重连/热插拔

    //////////////////////float property/////////////////
    /// unit: °
    float f_optvalue = 330.f;
    lidar.setlidaropt(LidarPropMaxAngle, &f_optvalue, sizeof(float));///扫描最大角度

    f_optvalue = 30.f;
    lidar.setlidaropt(LidarPropMinAngle, &f_optvalue, sizeof(float));///扫描最小角度

    /// unit: m
    f_optvalue = 64.f;
    lidar.setlidaropt(LidarPropMaxRange, &f_optvalue, sizeof(float));///最大有效扫描半径

    f_optvalue = 0.01;
    lidar.setlidaropt(LidarPropMinRange, &f_optvalue, sizeof(float));///最小有效扫描半径

    /// unit: Hz
    f_optvalue = frequency;
    lidar.setlidaropt(LidarPropScanFrequency, &f_optvalue, sizeof(float));///扫描频率

    optval = 20;	//采样频率
    lidar.setlidaropt(LidarPropSampleRate, &optval, sizeof(int));///采样频率
    

    bool ret = lidar.initialize();///雷达初始化

    if (ret) {
        ret = lidar.turnOn();///启动
    }  else {
        fprintf(stderr, "%s\n", lidar.DescribeError());
        fflush(stderr);
    }

    LaserScan scan;

    while (ret && lidar::os_isOk()) {
        if (lidar.doProcessSimple(scan)) {//获取一圈点云数据
            fprintf(stdout, "Scan received[%llu]: %u pionts.\n",
                    scan.stamp,
                    (unsigned int)scan.points.size());
            fflush(stdout);
        } else {
            fprintf(stderr, "Failed to get Lidar Data\n");
            fflush(stderr);
        }
    }

    if (ret) {
        lidar.turnOff();///关闭
    }

    lidar.disconnecting();///断开连接

    return 0;
}
