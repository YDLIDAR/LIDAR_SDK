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
/** @page LIDAR C API
 * LIDAR C API
    <table>
        <tr><th>Library     <td>lidar_sdk
        <tr><th>File        <td>lidar_sdk.h
        <tr><th>Author      <td>Tony
        <tr><th>Source      <td>None
        <tr><th>Version     <td>1.0.0
    </table>

* @copyright    Copyright (c) 2018-2020  EAIBOT
    Jump to the lidar_sdk.h interface documentation.
*/

#ifndef LIDAR_SDK_H_
#define LIDAR_SDK_H_

#include <stdbool.h>
#include <stdint.h>
#include <core/common/lidar_def.h>

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @ref "LIDAR_C_API"
 * @par LIDAR_C_API
 *
 */

/**
 * @brief create a Lidar instance
 * @note call ::lidarDestroy destroy
 * @return created instance
 */
LIDAR_API PubLidar *lidarCreate(void);

/**
 * @brief Destroy Lidar instance by ::lidarCreate create
 * @param lidar     CLidar instance
 */
LIDAR_API void lidarDestroy(PubLidar **lidar);

/**
 * @brief set lidar properties
 * @param lidar           a lidar instance
 * @param optname        option name
 * @todo string properties
 * - @ref LidarPropSerialPort
 * - @ref LidarPropIgnoreArray
 * @note set string property example
 * @code
 * CLidar laser;
 * std::string lidar_port = "/dev/lidar";
 * laser.setlidaropt(LidarPropSerialPort,lidar_port.c_str(), lidar_port.size());
 * @endcode
 * @todo int properties
 * - @ref LidarPropSerialBaudrate
 * - @ref LidarPropLidarType
 * - @ref LidarPropDeviceType
 * - @ref LidarPropSampleRate
 * @note set int property example
 * @code
 * CLidar laser;
 * int lidar_baudrate = 230400;
 * laser.setlidaropt(LidarPropSerialPort,&lidar_baudrate, sizeof(int));
 * @endcode
 * @todo bool properties
 * - @ref LidarPropFixedResolution
 * - @ref LidarPropReversion
 * - @ref LidarPropInverted
 * - @ref LidarPropAutoReconnect
 * - @ref LidarPropSingleChannel
 * - @ref LidarPropIntenstiy
 * @note set bool property example
 * @code
 * CLidar laser;
 * bool lidar_fixedresolution = true;
 * laser.setlidaropt(LidarPropSerialPort,&lidar_fixedresolution, sizeof(bool));
 * @endcode
 * @todo float properties
 * - @ref LidarPropMaxRange
 * - @ref LidarPropMinRange
 * - @ref LidarPropMaxAngle
 * - @ref LidarPropMinAngle
 * - @ref LidarPropScanFrequency
 * @note set float property example
 * @code
 * CLidar laser;
 * float lidar_maxrange = 16.0f;
 * laser.setlidaropt(LidarPropSerialPort,&lidar_maxrange, sizeof(float));
 * @endcode
 * @param optval         option value
 * - std::string(or char*)
 * - int
 * - bool
 * - float
 * @param optlen         option length
 * - data type size
 * @return true if the Property is set successfully, otherwise false.
 * @see LidarProperty
 */
LIDAR_API bool setlidaropt(PubLidar *lidar, int optname, const void *optval,
                             int optlen);

/**
 * @brief get lidar property
 * @param lidar           a lidar instance
 * @param optname         option name
 * @todo string properties
 * - @ref LidarPropSerialPort
 * - @ref LidarPropIgnoreArray
 * @note get string property example
 * @code
 * CLidar laser;
 * char lidar_port[30];
 * laser.getlidaropt(LidarPropSerialPort,lidar_port, sizeof(lidar_port));
 * @endcode
 * @todo int properties
 * - @ref LidarPropSerialBaudrate
 * - @ref LidarPropLidarType
 * - @ref LidarPropDeviceType
 * - @ref LidarPropSampleRate
 * @note get int property example
 * @code
 * CLidar laser;
 * int lidar_baudrate;
 * laser.getlidaropt(LidarPropSerialPort,&lidar_baudrate, sizeof(int));
 * @endcode
 * @todo bool properties
 * - @ref LidarPropFixedResolution
 * - @ref LidarPropReversion
 * - @ref LidarPropInverted
 * - @ref LidarPropAutoReconnect
 * - @ref LidarPropSingleChannel
 * - @ref LidarPropIntenstiy
 * @note get bool property example
 * @code
 * CLidar laser;
 * bool lidar_fixedresolution;
 * laser.getlidaropt(LidarPropSerialPort,&lidar_fixedresolution, sizeof(bool));
 * @endcode
 * @todo float properties
 * - @ref LidarPropMaxRange
 * - @ref LidarPropMinRange
 * - @ref LidarPropMaxAngle
 * - @ref LidarPropMinAngle
 * - @ref LidarPropScanFrequency
 * @note set float property example
 * @code
 * CLidar laser;
 * float lidar_maxrange;
 * laser.getlidaropt(LidarPropSerialPort,&lidar_maxrange, sizeof(float));
 * @endcode
 * @param optval          option value
 * - std::string(or char*)
 * - int
 * - bool
 * - float
 * @param optlen          option length
 * - data type size
 * @return true if the Property is get successfully, otherwise false.
 * @see LidarProperty
 */
LIDAR_API bool getlidaropt(PubLidar *lidar, int optname, void *optval,
                             int optlen);

/**
* Return SDK's version information in a numeric form.
* @param version Pointer to a version for returning the version information.
*/
LIDAR_API void GetSdkVersion(char *version);

/**
 * Initialize the SDK.
 * @return true if successfully initialized, otherwise false.
 */
LIDAR_API bool initialize(PubLidar *lidar);

/**
 * Start the device scanning routine which runs on a separate thread.
 * @return true if successfully started, otherwise false.
 */
LIDAR_API bool turnOn(PubLidar *lidar);

/**
 * @brief Get the LiDAR Scan Data. turnOn is successful before doProcessSimple scan data.
 * @param[in] lidar          LiDAR instance
 * @param[out] outscan       LiDAR Scan Data
 * @return true if successfully started, otherwise false.
 */
LIDAR_API bool doProcessSimple(PubLidar *lidar, LaserFan *outscan);
/**
 * @brief Stop the device scanning thread and disable motor.
 * @return true if successfully Stoped, otherwise false.
 */
LIDAR_API bool turnOff(PubLidar *lidar);
/**
 * @brief Uninitialize the SDK and Disconnect the LiDAR.
 */
LIDAR_API void disconnecting(PubLidar *lidar);

/**
 * @brief Get the last error information of a (socket or serial)
 * @return a human-readable description of the given error information
 * or the last error information of a (socket or serial)
 */
LIDAR_API const char *DescribeError(PubLidar *lidar);

/**
 * @brief initialize system signals
 */
LIDAR_API void os_init();
/**
 * @brief isOk
 * @return true if successfully initialize, otherwise false.
 */
LIDAR_API bool os_isOk();
/**
 * @brief os_shutdown
 */
LIDAR_API void os_shutdown();

/**
 * @brief get lidar serial port
 * @param ports serial port lists
 * @return valid port number
 */
LIDAR_API int lidarPortList(PubLidar *lidar, LidarPort *ports);

#ifdef __cplusplus
}
#endif

#endif  // LIDAR_SDK_H_
