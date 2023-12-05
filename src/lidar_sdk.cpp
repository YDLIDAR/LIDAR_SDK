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

#include <sstream>
#include "lidar_sdk.h"
#include "CLidar.h"
#include "lidar_config.h"

Lidar *lidarCreate() {
    Lidar *instance = new Lidar;
    instance->lidar = (void *)new CLidar();
    return instance;
}

void lidarDestroy(Lidar **lidar) {
    if (lidar == NULL || *lidar == NULL) {
        return;
    }

    CLidar *drv = static_cast<CLidar *>((*lidar)->lidar);

    if (drv) {
        delete drv;
        drv = NULL;
    }

    (*lidar)->lidar = NULL;
    delete *lidar;
    *lidar = NULL;
    return;
}

bool setlidaropt(Lidar *lidar, int optname, const void *optval, int optlen) {
    if (lidar == NULL || lidar->lidar == NULL || optval == NULL) {
        return false;
    }

    CLidar *drv = static_cast<CLidar *>(lidar->lidar);

    if (drv) {
        return drv->setlidaropt(optname, optval, optlen);
    }

    return false;
}

bool getlidaropt(Lidar *lidar, int optname, void *optval, int optlen) {
    if (lidar == NULL || lidar->lidar == NULL || optval == NULL) {
        return false;
    }

    CLidar *drv = static_cast<CLidar *>(lidar->lidar);

    if (drv) {
        return drv->getlidaropt(optname, optval, optlen);
    }

    return false;
}


void GetSdkVersion(char *version) {
    strcpy(version, LIDAR_SDK_VERSION_STR);
}

bool initialize(Lidar *lidar) {
    if (lidar == NULL || lidar->lidar == NULL) {
        return false;
    }

    CLidar *drv = static_cast<CLidar *>(lidar->lidar);

    if (drv) {
        return drv->initialize();
    }

    return false;
}


bool turnOn(Lidar *lidar) {
    if (lidar == NULL || lidar->lidar == NULL) {
        return false;
    }

    CLidar *drv = static_cast<CLidar *>(lidar->lidar);

    if (drv) {
        return drv->turnOn();
    }

    return false;
}

bool doProcessSimple(Lidar *lidar, LaserFan *outscan) {
    if (lidar == NULL || lidar->lidar == NULL || outscan == NULL) {
        return false;
    }

    LaserFanDestroy(outscan);
    outscan->npoints = 0;

    CLidar *drv = static_cast<CLidar *>(lidar->lidar);
    
    if (drv) {
        LaserScan scan;
        bool ret = drv->doProcessSimple(scan);
        outscan->config = scan.config;
        outscan->stamp = scan.stamp;
        outscan->npoints = scan.points.size();
        outscan->points = (LaserPoint *)malloc(sizeof(LaserPoint) * outscan->npoints);
        std::copy(scan.points.begin(), scan.points.end(), outscan->points);
        return ret;
    }

    return false;
}

bool turnOff(Lidar *lidar) {
    if (lidar == NULL || lidar->lidar == NULL) {
        return false;
    }

    CLidar *drv = static_cast<CLidar *>(lidar->lidar);

    if (drv) {
        return drv->turnOff();
    }

    return false;
}

void disconnecting(Lidar *lidar) {
    if (lidar == NULL || lidar->lidar == NULL) {
        return;
    }

    CLidar *drv = static_cast<CLidar *>(lidar->lidar);

    if (drv) {
        drv->disconnecting();
    }
}

const char *DescribeError(Lidar *lidar) {
    char const *value = "";

    if (lidar == NULL || lidar->lidar == NULL) {
        return value;
    }

    CLidar *drv = static_cast<CLidar *>(lidar->lidar);

    if (drv) {
        return drv->DescribeError();
    }

    return value;
}

void os_init() {
    lidar::os_init();
}

bool os_isOk() {
    return lidar::os_isOk();
}

void os_shutdown() {
    lidar::os_shutdown();
}

int lidarPortList(Lidar *lidar, LidarPort *ports) {
    if (lidar == NULL || ports == NULL) {
        return 0;
    }

    CLidar *drv = static_cast<CLidar *>(lidar->lidar);

    memset(ports, 0, sizeof(LidarPort));
    std::map<std::string, std::string> lists = drv->lidarPortList();
    std::map<std::string, std::string>::iterator it;

    int i = 0;
    for (it = lists.begin(); it != lists.end(); it++) {
        string_t port;
        memset(&port, 0, sizeof(string_t));

        if (i < sizeof(LidarPort) / sizeof(string_t)) {
            memcpy(ports->port[i].data, it->second.c_str(), it->second.size());
        }

        i++;
    }
    return i;
}
