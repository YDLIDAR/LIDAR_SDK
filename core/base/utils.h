
#pragma once

#ifdef WIN32
#ifdef lidar_IMPORTS
#define LIDAR_API __declspec(dllimport)
#else
#ifdef lidarStatic_IMPORTS
#define LIDAR_API
#else

#define LIDAR_API __declspec(dllexport)
#endif // LIDAR_STATIC_EXPORTS
#endif

#else
#define LIDAR_API
#endif // ifdef WIN32
