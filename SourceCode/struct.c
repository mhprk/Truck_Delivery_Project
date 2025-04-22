#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "struct.h"
#include "mapping.h"

struct Truck trucks[3];
struct Map routeMap;

int checkDestination(struct Shipment shipment, const struct Map* map) {
    int row = shipment.destination.row;
    int col = shipment.destination.col;

    if (row >= 0 && row < map->numRows && col >= 0 && col < map->numCols) {
        if (map->squares[row][col] == 1) {
            return 1; // The destination is a building
        }
    }
    return 0; // The destination is not a building
}

int bestRoute(struct Route routes[MAX_ROUTE], struct Shipment shipment, int size) {
    int bestIndex = -1;
    double bestDistance = 999999.99;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < routes[i].numPoints; j++) {
            double dist = distance(&routes[i].points[j], &shipment.destination);
            if (dist < bestDistance) {
                bestDistance = dist;
                bestIndex = i;
            }
        }
    }

    return bestIndex;
}


int validtruckpaths(int numPaths, int paths[]) {

    for (int i = 0; i < numPaths; i++) {
        if (paths[i] < 0) {
            return 0;
        }
    }
    return 1;
}


void validTruckPaths(struct Shipment shipment, struct Truck truck, const struct Map* map, struct Route routes[MAX_ROUTE], int* size) {

    // Initialize the size to 0
    *size = 0;

    // Iterate through each truck
    for (int i = 0; i < 3; i++) {

        // Get the route for the current truck
        struct Route* route = trucks[i].truckRoute;
        if (route != NULL && validShipment(shipment) && checkBuilding(&route, map)) {
            // If the route is valid, the shipment is valid, and the route does not pass through buildings
            routes[(*size)++] = *route; // Add the route to the valid routes array and increment the size
        }
    }
}

int validShipment(struct Shipment shipment) {
    return shipment.shipmentVolume <= 100 && shipment.shipmentWeight <= 2500;
}

int checkBuilding(const struct Route* route, const struct Map* map) {
    for (int i = 0; i < route->numPoints; i++) {
        int row = route->points[i].row;
        int col = route->points[i].col;

        if (row >= 0 && row < map->numRows && col >= 0 && col < map->numCols) {
            if (map->squares[row][col] == 1) {
                return 1;
            }
        }
    }
    return 0;
}

int validCargo(float boxSize) {
    const float validSizes[] = { 1.0f, 3.0f, 5.0f };
    const float tolerance = 0.001f;  // Tolerance value for floating-point comparison
    int valid = 0;

    for (int i = 0; i < 3; i++) {
        if (fabs(boxSize - validSizes[i]) < tolerance) {
            valid = 1;
            break;
        }
    }

    if (!valid) {
        printf("Invalid box size\n");
    }

    return valid;
}
//BUG
void handleShipment(struct Shipment shipment, struct Truck trucks[], int numTrucks, const struct Map* map) {
    if (!validShipment(shipment)) {
        printf("Invalid shipment\n");
        return;
    }

    if (!checkDestination(shipment, map)) {
        printf("Invalid destination\n");
        return;
    }

    double bestDistance = 999999.99;
    int bestTruckIndex = -1;
    struct Route bestDiversion = { {0}, 0, 0 };

    for (int i = 0; i < numTrucks; i++) {
        struct Route diversionRoute = shortestPath(map, trucks[i].truckRoute->points[trucks[i].truckRoute->numPoints - 1], shipment.destination);
        double distanceToDestination = diversionRoute.numPoints;
        if (distanceToDestination < bestDistance && trucks[i].truckWeight >= shipment.shipmentWeight && trucks[i].truckVolume >= shipment.shipmentVolume) {
            bestDistance = distanceToDestination;
            bestTruckIndex = i;
            bestDiversion = diversionRoute;
        }
    }

    if (bestTruckIndex == -1) {
        printf("Ships tomorrow\n");
        return;
    }

    trucks[bestTruckIndex].truckWeight -= shipment.shipmentWeight;
    trucks[bestTruckIndex].truckVolume -= shipment.shipmentVolume;

    printf("Ship on %s LINE, ", (trucks[bestTruckIndex].truckRoute->routeSymbol == BLUE ? "BLUE" : (trucks[bestTruckIndex].truckRoute->routeSymbol == GREEN ? "GREEN" : "YELLOW")));
    if (bestDiversion.numPoints > 0) {
        printf("divert: ");
        for (int i = 0; i < bestDiversion.numPoints; i++) {
            printf("%d%c", bestDiversion.points[i].row, 'A' + bestDiversion.points[i].col);
            if (i < bestDiversion.numPoints - 1) printf(", ");
        }
    }
    else {
        printf("no diversion");
    }
    printf("\n");
}

//no longer a BUG
int shipmentProcess(struct Truck* trucks) {
    struct Shipment shipment;
    int destination1;
    char destination2;

    printf("Enter shipment weight, box size and destination (0 0 x to stop): ");
    if (scanf("%f %f %d %c", &shipment.shipmentWeight, &shipment.shipmentVolume, &destination1, &destination2) != 4) {
        printf("Invalid input format.\n");
        return -1; // Indicate an error
    }

    if (shipment.shipmentWeight == 0 && shipment.shipmentVolume == 0) {
        exit(0); // Stop the program
    }

    if (shipment.shipmentWeight < MIN_WEIGHT || shipment.shipmentWeight > MAX_WEIGHT) {
        printf("Invalid weight.\n");
        return -1; // Indicate an error
    }

    if (!validCargo(shipment.shipmentVolume)) {
        return -1; // Indicate an error
    }

    if (destination2 >= 'A' && destination2 <= 'Y') {
        if (destination1 >= 1 && destination1 <= 25) {
            shipment.destination.row = destination1 - 1;
            shipment.destination.col = destination2 - 'A';

            if (checkDestination(shipment, &routeMap)) {
                handleShipment(shipment, trucks, 3, &routeMap);
            }
            else {
                printf("Invalid destination\n");
                return -1; // Indicate an error
            }
        }
        else {
            printf("Invalid destination\n");
            return -1; // Indicate an error
        }
    }
    else {
        printf("Invalid destination\n");
        return -1; // Indicate an error
    }

    return 0; // Success
}
