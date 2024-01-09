#ifndef __YDLIDAR_H__
#define __YDLIDAR_H__

#include <stdint.h>
#include <stdbool.h>

#define MAX_SCAN_POINTS 360
#define FirstRobot 0
#define SecondRobot 1
#define MAX_SCAN_BUFFER_SIZE 2
#define RobotNumber 2
#define MaxObstaclePoint 40
#define MaxObstacleArea 18
typedef struct
{
    double distance;
    double angle;

} ScanPoint_t;

// scanpoint with count
typedef struct
{
    ScanPoint_t scanPoint[MaxObstaclePoint];
    uint16_t count;
} ScanPointData_t;

// ydlidar data process status enum : 0 for waitingdata , 1 for processing data
typedef enum
{
    WAITING_DATA = 0,
    PROCESSING_DATA = 1
} ydlidar_data_process_status_t;

///< INit hte Ydlidar
void YdlidarInit(void);
///< recevied scan data with DMA
void YdlidarReceivedScanDataWithDMA(void);
///< data process
void dataProcess(void);
///< get scan angle
bool getAngleAndDistanceAfterProcess(double *angles, double *distances, int LSN);
///< restart scan
void restartScan(void);
///< start receive scan data task
void createYdlidarTask(void);

#endif // __YDLIDAR_H__