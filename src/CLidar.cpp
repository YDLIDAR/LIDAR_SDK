//
// The MIT License (MIT)
//
// Copyright (c) 2019-2020 EAIBOT. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#include <map>
#include <numeric>
#include <algorithm>
#include <math.h>
#include <functional>
#include "CLidar.h"
#include "core/math/angles.h"
#include "core/serial/common.h"
#include "core/common/DriverInterface.h"
#include "core/common/lidar_help.h"
#include "core/common/lidar_def.h"
#include "LidarDriver.h"
#include <core/serial/serial.h>
#ifdef _WIN32
#include <synchapi.h>
#else
#include <unistd.h>
#endif

/*-------------------------------------------------------------
                            CLidar
-------------------------------------------------------------*/
CLidar::CLidar() {
    m_lidarPtr = nullptr;
    m_global_nodes = new node_info[DriverInterface::MAX_SCAN_NODES];
    m_field_of_view = 300;
    m_lidar_model = DriverInterface::LIDAR;

    //参数表
    m_SerialPort = "192.168.0.11";
    m_SerialBaudrate = 8090;
    m_AutoReconnect = true;
    m_MinAngle = 30.f;
    m_MaxAngle = 330.f;
    m_MaxRange = 64.0;
    m_MinRange = 0.01f;
    m_LidarType = TYPE_LIDAR;
    m_ScanFrequency = 10.f;
    m_sampleRate = 20;
}

/*-------------------------------------------------------------
                           ~CLidar
-------------------------------------------------------------*/
CLidar::~CLidar(){
    disconnecting();
    if (m_global_nodes)
    {
        delete[] m_global_nodes;
        m_global_nodes = NULL;
    }
}

/*-------------------------------------------------------------
                          setlidaropt
-------------------------------------------------------------*/
bool CLidar::setlidaropt(int optname, const void *optval, int optlen) {
    if (optval == NULL) {
#if defined(_WIN32)
        SetLastError(EINVAL);
#else
        errno = EINVAL;
#endif
        return false;
    }

    if (optname >= LidarPropFixedResolution) {
        if (optlen != sizeof(bool)) {
#if defined(_WIN32)
            SetLastError(EINVAL);
#else
            errno = EINVAL;
#endif
            return false;
        }
    } else if (optname >= LidarPropMaxRange) {
        if (optlen != sizeof(float)) {
#if defined(_WIN32)
            SetLastError(EINVAL);
#else
            errno = EINVAL;
#endif
            return false;
        }
    } else if (optname >= LidarPropSerialBaudrate) {
        if (optlen != sizeof(int)) {
#if defined(_WIN32)
            SetLastError(EINVAL);
#else
            errno = EINVAL;
#endif
            return false;
        }
    } else {

    }

    bool ret = true;
    switch (optname) {
        case LidarPropLidarType:
            m_LidarType = *(int *)(optval);
            break;

        case LidarPropSerialPort:
            m_SerialPort = (const char *)optval;
            break;

        case LidarPropSerialBaudrate:
            m_SerialBaudrate = *(int *)(optval);
            break;

        case LidarPropAutoReconnect:
            m_AutoReconnect = *(bool *)(optval);
            break;

        case LidarPropMaxAngle:
            m_MaxAngle = *(float *)(optval);
            break;

        case LidarPropMinAngle:
            m_MinAngle = *(float *)(optval);
            break;

        case LidarPropMaxRange:
            m_MaxRange = *(float *)(optval);
            break;

        case LidarPropMinRange:
            m_MinRange = *(float *)(optval);
            break;

        case LidarPropScanFrequency:
            m_ScanFrequency = *(float *)(optval);
            break;
               
        case LidarPropSampleRate:
            m_sampleRate = *(int *)(optval);
            break;

        case LidarPropReversion:
	    m_Reversion = *(bool *)(optval);
            break;

        default:
            ret = false;
            break;
    }
    return ret;
}

/*-------------------------------------------------------------
                          getlidaropt
-------------------------------------------------------------*/
bool CLidar::getlidaropt(int optname, void *optval, int optlen) {
    if (optval == NULL) {
#if defined(_WIN32)
        SetLastError(EINVAL);
#else
        errno = EINVAL;
#endif
        return false;
    }

    if (optname >= LidarPropFixedResolution) {
        if (optlen != sizeof(bool)) {
#if defined(_WIN32)
            SetLastError(EINVAL);
#else
            errno = EINVAL;
#endif
            return false;
        }
    } else if (optname >= LidarPropMaxRange) {
        if (optlen != sizeof(float)) {
#if defined(_WIN32)
            SetLastError(EINVAL);
#else
            errno = EINVAL;
#endif
            return false;
        }
    } else if (optname >= LidarPropSerialBaudrate) {
        if (optlen != sizeof(int)) {
#if defined(_WIN32)
            SetLastError(EINVAL);
#else
            errno = EINVAL;
#endif
            return false;
        }
    } else {

    }

    bool ret = true;
    switch (optname) {
        case LidarPropLidarType:
            memcpy(optval, &m_LidarType, optlen);
            break;

        case LidarPropSerialPort:
            memcpy(optval, m_SerialPort.c_str(), optlen);
            break;

        case LidarPropSerialBaudrate:
            memcpy(optval, &m_SerialBaudrate, optlen);
            break;

        case LidarPropAutoReconnect:
            memcpy(optval, &m_AutoReconnect, optlen);
            break;

        case LidarPropMaxAngle:
            memcpy(optval, &m_MaxAngle, optlen);
            break;

        case LidarPropMinAngle:
            memcpy(optval, &m_MinAngle, optlen);
            break;

        case LidarPropMaxRange:
            memcpy(optval, &m_MaxRange, optlen);
            break;

        case LidarPropMinRange:
            memcpy(optval, &m_MinRange, optlen);
            break;

        case LidarPropScanFrequency:
            memcpy(optval, &m_ScanFrequency, optlen);
            break;
        case LidarPropSampleRate:
            memcpy(optval, &m_sampleRate, optlen);
        default:
            ret = false;
            break;
    }
    return ret;
}

/*-------------------------------------------------------------
                          initialize
-------------------------------------------------------------*/
bool CLidar::initialize() {
    LOGD("Lidar SDK initializing");
    uint32_t t = getms();
    if (!checkCOMMs()) {
        LOGE("initializing lidar fail.");
        return false;
    }
    // if (!checkStatus())
    // {
    //     LOGE("[CLidar::initialize] Error initializing LIDAR check status under [%s] and [%d].",m_SerialPort.c_str(), m_SerialBaudrate);
    //     return false;
    // }
    //LOGD("LiDAR init success, Elapsed time== %u ms", getms() - t);
    return true;
}

/*-------------------------------------------------------------
                          checkCOMMs
-------------------------------------------------------------*/
bool CLidar::checkCOMMs() {
    if (!m_lidarPtr) {
        if (isLidar(m_LidarType)) {
            m_lidarPtr = new lidar::LidarDriver();
        } else {
            //LOGW("An unsupported model:%d", m_LidarType);
        }

        if (!m_lidarPtr) {
            fprintf(stderr, "Create lidar fail");
            return false;
        }
       
        //LOGD("SDK Version: %s", m_lidarPtr->getSDKVersion().c_str());
    } else {
        LOGD("Lidar SDK has been initialized");
    }

    if (m_lidarPtr->getIsConnected()) {
        return true;
    }
    //make connection...
    result_t op_result = m_lidarPtr->connect(m_SerialPort.c_str(), m_SerialBaudrate);
    if (!IS_OK(op_result)) {
        //LOGE("[CLidar] Error, cannot bind to the specified IP Address[%s]", m_SerialPort.c_str());     
        return false;
    }
    LOGD("LiDAR successfully connected");
    return true;
}

/*-------------------------------------------------------------
                           turnOn
-------------------------------------------------------------*/
bool CLidar::turnOn() {
    if(!m_lidarPtr) {
        return false;
    }

    if (m_lidarPtr->getIsScanning()) {
        LOGD("The radar is scanning.");
        return true;
    }
    if (isSupportScanFrequency(m_lidar_model, m_ScanFrequency)) {
        scan_frequency _scan_frequency;
        _scan_frequency.frequency = m_ScanFrequency;
        sampling_rate _sampling_rate;
        _sampling_rate.rate =  m_sampleRate;
		#ifdef _WIN32
		Sleep(1000);
		#else
        usleep(1000);
		#endif
        m_lidarPtr->setScanFrequency(_scan_frequency);
        m_lidarPtr->setSamplingRate(_sampling_rate);
    }

    result_t op_result = m_lidarPtr->startScan();
    if (!IS_OK(op_result)) {
        //LOGE("[CLidar] Failed to start scan mode: %x", op_result);
        return false;
    }
    m_field_of_view = m_MaxAngle - m_MinAngle;
    m_lidarPtr->setIsAutoReconnect(m_AutoReconnect);
    LOGD("Successful radar activation.");
    return true;
}

/*-------------------------------------------------------------
                           turnOff
-------------------------------------------------------------*/
bool CLidar::turnOff() {
    if(!m_lidarPtr) {
        return false;
    }

    if (!m_lidarPtr->getIsScanning()) {
        LOGD("Now LIDAR Scanning has stopped.");
        return true;
    }

    result_t op_result = m_lidarPtr->stopScan();
    if (!IS_OK(op_result)) {
        //LOGE("[CLidar] Failed to stop scan mode: %x", op_result);
        return false;
    }
    LOGD("The radar has stopped scanning.");
    return true;
}

/*-------------------------------------------------------------
                        doProcessSimple
-------------------------------------------------------------*/
bool CLidar::doProcessSimple(LaserScan &outscan) {
    size_t count = DriverInterface::MAX_SCAN_NODES;
    //从缓存中获取已采集的一圈扫描数据
    result_t op_result = m_lidarPtr->grabScanData(m_global_nodes, count);
    outscan.points.clear();

    // Fill in scan data:
    if (!IS_OK(op_result)) {
        return false;
    }

    outscan.config.min_angle = math::from_degrees(m_MinAngle);
    outscan.config.max_angle = math::from_degrees(m_MaxAngle);
    outscan.config.scan_time = (m_global_nodes[count - 1].stamp - m_global_nodes[0].stamp) * 0.001;//单位：s
    outscan.config.angle_increment = math::from_degrees(m_field_of_view) / count;
    outscan.config.time_increment = outscan.config.scan_time / count;
    outscan.config.min_range = m_MinRange;
    outscan.config.max_range = m_MaxRange;

    //将一圈中第一个点采集时间作为该圈数据采集时间
    outscan.stamp = (m_global_nodes[0].stamp > 0) ? m_global_nodes[0].stamp : 0;

    float range = 0.0;
    float intensity = 0.0;
    float angle = 0.0;
    for(size_t i = 0; i < count; i++) {
        range = static_cast<float>(m_global_nodes[i].distance_q2 / 1000.f);//单位：m
        intensity = static_cast<float>(m_global_nodes[i].sync_quality);
        angle = static_cast<float>(m_global_nodes[i].angle_q6_checkbit / 100.0f);//单位：度
        
        LaserPoint point;
        if (m_Reversion) {
            angle = 360 - angle;
        } 
        point.angle = angle;
        point.range = range;
        point.intensity = intensity;
        outscan.points.push_back(point);
    }
    return true;
}

/*-------------------------------------------------------------
                        disconnecting
-------------------------------------------------------------*/
void CLidar::disconnecting() {
    if (m_lidarPtr) {
        m_lidarPtr->disconnect();
        delete m_lidarPtr;
        m_lidarPtr = nullptr;
    }
}

/*-------------------------------------------------------------
                        DescribeError
-------------------------------------------------------------*/
const char *CLidar::DescribeError() const {
    char const *value = "";
    if (m_lidarPtr) {
        return m_lidarPtr->DescribeError();
    }
    return value;
}

/*-------------------------------------------------------------
                        getDriverError
-------------------------------------------------------------*/
DriverError CLidar::getDriverError() const {
    DriverError er = UnknownError;
    if (m_lidarPtr) {
        return m_lidarPtr->getDriverError();
    }
    return er;
}

/*-------------------------------------------------------------
                        lidarPortList
-------------------------------------------------------------*/
map<string, string> CLidar::lidarPortList()
{
    map<string, string> lstMap;
    lstMap.clear();
    if (m_lidarPtr) {
        lstMap = m_lidarPtr->lidarPortList();
    }
    return lstMap;
}

namespace lidar{
    
void os_init() {
    lidar::core::base::init();
}

bool os_isOk() {
    return lidar::core::base::ok();
}

void os_shutdown() {
    lidar::core::base::shutdown();
}

}//namespace lidar
