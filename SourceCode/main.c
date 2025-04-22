#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "mapping.h"
#include "struct.h"

int main(void) {
    printf("=================\n");
    printf("Seneca Polytechnic Deliveries\n");
    printf("=================\n");

    struct Map routeMap = populateMap();
    if (routeMap.numRows == 0 || routeMap.numCols == 0) {
        fprintf(stderr, "Error: Map initialization failed.\n");
        return -1;
    }

    struct Map baseMap = populateMap();
    if (baseMap.numRows == 0 || baseMap.numCols == 0) {
        fprintf(stderr, "Error: Map initialization failed.\n");
        return -1;
    }

    struct Route blueRoute = getBlueRoute();
    if (blueRoute.numPoints == 0) {
        fprintf(stderr, "Error: Blue route initialization failed.\n");
        return -1;
    }

    struct Route greenRoute = getGreenRoute();
    if (greenRoute.numPoints == 0) {
        fprintf(stderr, "Error: Green route initialization failed.\n");
        return -1;
    }

    struct Route yellowRoute = getYellowRoute();
    if (yellowRoute.numPoints == 0) {
        fprintf(stderr, "Error: Yellow route initialization failed.\n");
        return -1;
    }

    trucks[0].truckVolume = 100.0;
    trucks[0].truckWeight = 2500.0;
    trucks[0].truckRoute = &blueRoute;

    trucks[1].truckVolume = 100.0;
    trucks[1].truckWeight = 2500.0;
    trucks[1].truckRoute = &greenRoute;

    trucks[2].truckVolume = 100.0;
    trucks[2].truckWeight = 2500.0;
    trucks[2].truckRoute = &yellowRoute;

    int keepRunning = 1;
    while (keepRunning) {
        keepRunning = shipmentProcess(trucks);
    }

    printf("Thanks for shipping with Seneca Polytechnic!\n");

    return 0;
}