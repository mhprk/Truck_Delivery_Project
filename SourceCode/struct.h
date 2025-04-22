#pragma once
#ifndef STRUCT_H
#define STRUCT_H

#include <stdio.h>
#include "mapping.h"

#define MAX_POINTS 625  
#define MIN_WEIGHT 1
#define MAX_WEIGHT 2500

/*
Struct: Truck - Determines if it can take shipments based on the truck's size,
                weight and the path it will take
*/
struct Truck {
    double truckVolume;
    double truckWeight;
    struct Route* truckRoute;
};

/*
Struct: Shipment - For the shipment package that includes shipment's weight,
                    volume, and destination
*/
struct Shipment {
    float shipmentWeight;
    float shipmentVolume;
    struct Point destination;
};

// To identify the 3 trucks: yellow, blue, and green
struct Truck trucks[3];

/*
1.
Verifies if a shipment's destination corresponds to a building on the map.
@param shipment - Shipment struct with shipment destination
@param map - Pointer to the map structure that contains information about buildings
@returns - Returns 1 if the destination is in the route, otherwise 0
*/
int checkDestination(struct Shipment shipment, const struct Map* map);

/*
2.
Finds the shortest and best route to deliver the shipment.
@param routes - An array of all valid routes
@param shipment - Shipment struct that holds the weight, volume, and destination
@param size - Pointer to an integer holding the size of the 'routes' array
@returns - The index of the best route
*/
int bestRoute(struct Route routes[MAX_ROUTE], struct Shipment shipment, int size);

/*
3.
Checks if the box sizes are valid based on the truck restrictions.
@param shipment - Shipment struct that decides if the size of it is valid or not
@returns - 1 if valid, 0 if invalid
*/
int validShipment(struct Shipment shipment);

/*
4.
Checks for the location of the building.
@param route - Route struct with all the points in the given route
@param map - Map struct with all the points in the map
@returns - 1 if points on the map is a building, 0 if not
*/
int checkBuilding(const struct Route* route, const struct Map* map);

/*
5.
Handles a shipment by determining the best truck and route, and updating the truck's capacity.
@param shipment - Shipment struct with shipment details
@param trucks - Array of Truck structs
@param numTrucks - Number of trucks in the array
@param map - Pointer to the Map struct
@returns - void
*/
void handleShipment(struct Shipment shipment, struct Truck trucks[], int numTrucks, const struct Map* map);

/*
6.
Checks if the cargo size is valid.
@param boxSize - Size of the cargo box
@returns - 1 if valid, 0 if invalid
*/
int validCargo(float boxSize);

/*
7.
Processes a shipment, assigning it to the appropriate truck.
@param trucks - Array of Truck structs
@returns - void
*/
int shipmentProcess(struct Truck* trucks);

/*
8.
checks for valid truck paths only
@param numPaths - the number of paths in the paths array
@param paths - an array of integer representing the paths
@returns 1 if valid and 0 if invalid
*/
int validtruckpaths(int numPaths, int paths[]);

/*
9.
Checks and identifies valid routes for a truck to deliver a shipment based on the given map.
@param shipment - shipment struct that holds the weight, vol and destination
@param truck - truck struct that holds the current cargo weight, vol and the route it will take
@param map - map struct with all the points in the map
@param routes - holds all valid routes
@param size - pointer to an integer holding the size of the ‘routes’ array
@returns - void, the "routes" array
*/
void validTruckPaths(struct Shipment shipment, struct Truck truck, const struct Map* map, struct Route routes[MAX_ROUTE], int* size);


#endif
