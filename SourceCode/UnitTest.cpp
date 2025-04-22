#include "pch.h"
#include "CppUnitTest.h"
#include "mapping.h"
#include "struct.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BlackboxTests
{
    TEST_CLASS(BlackboxTests)
    {
    public:

        TEST_METHOD(TestAddRoute)
        {
            struct Map map = populateMap();
            struct Route route = getBlueRoute();
            struct Map updatedMap = addRoute(&map, &route);

            // Check if the route was added correctly
            Assert::AreEqual(updatedMap.squares[0][0], BLUE);
            Assert::AreEqual(updatedMap.squares[1][0], BLUE);
        }
        TEST_METHOD(TestPopulateMap)
        {
            struct Map map = populateMap();

            // Check if the map is populated correctly
            Assert::AreEqual(map.squares[0][4], 1);
            Assert::AreEqual(map.squares[1][1], 1);
        }

        TEST_METHOD(TestEqPt)
        {
            struct Point p1 = { 0, 0 };
            struct Point p2 = { 0, 0 };
            struct Point p3 = { 1, 1 };

            // Test point equality
            Assert::IsTrue(eqPt(p1, p2));
            Assert::IsFalse(eqPt(p1, p3));
        }
    };
}

namespace WhiteBoxTests
{
    TEST_CLASS(CheckDestinationTests)
    {
    public:

        TEST_METHOD(DestinationNotInBuilding)
        {
            struct Shipment shipment1 = { 10.0, 5.0, { 0 , 1} }; // Test destination point
            struct Map map = populateMap();
            Assert::AreEqual(0, checkDestination(shipment1, &map));
        }

        TEST_METHOD(DestinationIsInBuilding)
        {
            struct Shipment shipment2 = { 10.0, 5.0, {  1 , 1 } }; // Test destination point
            struct Map map = populateMap();
            Assert::AreEqual(1, checkDestination(shipment2, &map));
        }

        TEST_METHOD(DestinationOutOfBounds)
        {
            struct Shipment shipment3 = { 10.0, 5.0, { -1 , 0 } }; // Test destination point out of bounds
            struct Map map = populateMap();
            Assert::AreEqual(0, checkDestination(shipment3, &map));
        }

        TEST_METHOD(DestinationOnBoundaryNotBuilding)
        {
            struct Shipment shipment4 = { 10.0, 5.0, { 26, 26} }; // Test destination point on boundary
            struct Map map = populateMap();

            Assert::AreEqual(0, checkDestination(shipment4, &map));
        }

        TEST_METHOD(DestinationOnBoundaryIsBuilding)
        {
            struct Shipment shipment5 = { 10.0, 5.0, { 24 , 1 } }; // Test destination point on boundary
            struct Map map = populateMap();
            Assert::AreEqual(1, checkDestination(shipment5, &map));
        }
    };

    TEST_CLASS(validtruckpathsTests)
    {
    public:
        TEST_METHOD(AllPositivePaths)
        {
            int numPaths = 3;
            int paths[] = { 1, 2, 3 };
            Assert::AreEqual(1, validtruckpaths(numPaths, paths), L"Expected validtruckpaths to return 1 for all positive paths");
        }

        TEST_METHOD(OneNegativePath)
        {
            int numPaths = 3;
            int paths[] = { 1, -2, 3 };
            Assert::AreEqual(0, validtruckpaths(numPaths, paths), L"Expected validtruckpaths to return 0 for a negative path");
        }
    };

    TEST_CLASS(CheckBuildingTests)
    {
    public:

        TEST_METHOD(BuildingNotInRoute)
        {
            struct Route route1 = { { { 'C', '5' }, { 'C', '6' }, { 'C', '7' } }, 3, 'B' };
            struct Map map = populateMap();
            Assert::AreEqual(0, checkBuilding(&route1, &map));
        }

        TEST_METHOD(EmptyRoute)
        {
            struct Route route1 = { { }, 0, 'B' };
            struct Map map = populateMap();
            Assert::AreEqual(0, checkBuilding(&route1, &map));
        }

        TEST_METHOD(SingleBuildingPoint)
        {
            struct Route route1 = { {'E','1'}, 1, 'B' };
            struct Map map = populateMap();
            Assert::AreEqual(0, checkBuilding(&route1, &map));
        }


        TEST_METHOD(RandomValues)
        {
            struct Route route1 = { {{'m','s'},{'K','2'}}, 2, 'B' };
            struct Map map = populateMap();
            Assert::AreEqual(0, checkBuilding(&route1, &map));
        }

        TEST_METHOD(SameNotBuildingPoint)
        {
            struct Route route1 = { {{'C','5'},{'C','5'}}, 2, 'B' };
            struct Map map = populateMap();
            Assert::AreEqual(0, checkBuilding(&route1, &map));
        }

        TEST_METHOD(SameBuildingPoint)
        {
            struct Route route1 = { {{'E','1'},{'E','1'}}, 2, 'B' };
            struct Map map = populateMap();
            Assert::AreEqual(0, checkBuilding(&route1, &map));
        }

    };
}

namespace AcceptanceTests
{
    TEST_CLASS(AcceptanceTests)//Acceptance Tests for requirements
    {
    public:

        TEST_METHOD(validtruckPaths)
        {
            //Testing valid paths with validtruckpath function which itself is a requirement
            int numPaths = 4;
            int paths[] = { 4, 5, 6,7 };
            Assert::AreEqual(1, validtruckpaths(numPaths, paths));

            // If This test is passing this means that the Truck Route is valid (The first requirement is being fullfilled).
        }

        TEST_METHOD(testTruckWeightCapacity)
        {
            double truckCapacity = 100.0; 
            Truck truck;
            truck.truckVolume = 50.0;
            truck.truckWeight = truckCapacity;
            truck.truckRoute = nullptr;

        
            double shipmentWeights[] = { 30.0, 20.0, 40.0 }; // Total is 90.0, which is under the capacity
            double overWeightShipment = 20.0; // This would push the total to 110.0, over the capacity

            // Test if the truck can accept the first set of shipments
            bool canAccept = true;
            for (double weight : shipmentWeights) {
                canAccept = canAccept && (truck.truckWeight >= weight);
                truck.truckWeight -= weight; // Simulate loading the truck
            }
            Assert::IsTrue(canAccept); 
            Assert::IsFalse(truck.truckWeight >= overWeightShipment); 
        }
        TEST_METHOD(validDestination)
        {
            struct Map map = populateMap();
            struct Shipment shipment;
            shipment.destination.row = 1;
            shipment.destination.col = 1;

            Assert::AreEqual(1, checkDestination(shipment, &map));
        }

        TEST_METHOD(validShipmentWeight)
        {
            // Define a shipment with valid weight
            struct Shipment shipment;
            shipment.shipmentWeight = 2500;
            shipment.shipmentVolume = 5;

            // Validate the shipment weight limit
            Assert::IsTrue(validShipment(shipment), L"Shipment weight should be within the limit of 2500 Kg.");

            // Define a shipment with invalid weight
            shipment.shipmentWeight = 2600; 

            // Validate the shipment weight limit
            Assert::IsFalse(validShipment(shipment), L"Shipment weight exceeds the limit of 2500 Kg.");
        }

        TEST_METHOD(validBoxSize)
        {
            // Valid box sizes
            float validSizes[] = { 1.0f, 3.0f, 5.0f };

            for (int i = 0; i < 3; i++) {
                // Validate the box size limit
                Assert::IsTrue(validCargo(validSizes[i]), L"Box size is invalid.");
            }

            // Invalid box size
            float invalidSize = 4.0f;

            // Validate the box size limit
            Assert::IsFalse(validCargo(invalidSize), L"Box size is not one of the valid sizes (1, 3, 5 cubic meters).");
        }

        TEST_METHOD(shortestRouteOptimization)
        {
            // Populate the map
            struct Map map = populateMap();

            // Define start and destination points
            struct Point start = { 0, 0 };
            struct Point dest = { 4, 4 };

            // Get the shortest route
            struct Route route = shortestPath(&map, start, dest);

            // Validate that the route reaches the destination
            Assert::IsTrue(eqPt(route.points[route.numPoints - 1], dest), L"Shortest route does not reach the destination.");
        }

        TEST_METHOD(validTruckVolume)
        {
            // Define a truck with maximum volume capacity
            struct Truck truck;
            truck.truckVolume = 100.0;  // Maximum truck volume capacity
            truck.truckWeight = 1500.0; // Arbitrary weight for this test

            // Define a shipment with valid volume (within the truck's capacity)
            struct Shipment shipment;
            shipment.shipmentVolume = 50.0;  // Volume within the truck's capacity
            shipment.shipmentWeight = 500.0;  // Arbitrary weight for this test

            // Validate that the truck can handle the shipment volume
            Assert::IsTrue(truck.truckVolume >= shipment.shipmentVolume, L"Truck should be able to accommodate the shipment volume.");

            // Define a shipment with invalid volume (exceeds the truck's capacity)
            shipment.shipmentVolume = 120.0;  // Volume exceeds the truck's capacity

            // Validate that the truck cannot handle the shipment volume
            Assert::IsFalse(truck.truckVolume >= shipment.shipmentVolume, L"Truck should not be able to accommodate the shipment volume.");
        }
    };
}

namespace IntegrationTests
{

    TEST_CLASS(CheckDestinationIntegrationTests)
    {
    public:

        TEST_METHOD(CheckDestinationAndCheckBuilding001) {
            struct Route route = {
             {
                 { 1, 0}, {2, 0}, {2, 1}
             },
             3,
             'B'
            };
            struct Shipment shipment = { 10.0, 5.0, {2, 1} }; // Valid destination point on the map
            struct Map map = populateMap();

            int destinationCheck = checkDestination(shipment, &map);
            int buildingCheck = checkBuilding(&route, &map);
            Assert::AreEqual(1, destinationCheck);
            Assert::AreEqual(1, buildingCheck);
        }

        TEST_METHOD(CheckDestinationAndCheckBuilding002) {
            struct Route route = {
             {
                 {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {5, 1}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {5, 6},
                {5, 7}, {5, 8}, {5, 9}, {6, 9}, {7, 9}, {7, 8}
             },
             17,
             'B'
            };
            struct Shipment shipment = { 10.0, 3.0, {7,8} }; 
            struct Map map = populateMap();
            int destinationCheck = checkDestination(shipment, &map);
            int buildingCheck = checkBuilding(&route, &map);
            Assert::AreEqual(1, destinationCheck);
            Assert::AreEqual(1, buildingCheck);
        }

        TEST_METHOD(CheckDestinationAndCheckBuilding003) {
            struct Route route = {
             {
                 { 1, 0}, {2, 0}, {3, 0},{4,0},{5,0},{5,1},{5,2},{5,3},{6,3},{7,3},{7,4}
             },
             11,
             'B'
            };
            struct Shipment shipment = { 15.0, 5.0, {7, 4} }; // Valid destination point on the map
            struct Map map = populateMap();
            int destinationCheck = checkDestination(shipment, &map);
            int buildingCheck = checkBuilding(&route, &map);
            Assert::AreEqual(1, destinationCheck);
            Assert::AreEqual(1, buildingCheck);
        }

        TEST_METHOD(CheckDestinationAndCheckBuilding004) {
            struct Route route = {
             {
                   { 1, 0}, {2, 0},{3,0},{ 3,1 }
             },
             4,
             'B'
            };
            struct Shipment shipment = { 20, 3.0, {3,1} }; // Valid destination point on the map
            struct Map map = populateMap();
            int destinationCheck = checkDestination(shipment, &map);
            int buildingCheck = checkBuilding(&route, &map);
            Assert::AreEqual(1, destinationCheck);
            Assert::AreEqual(1, buildingCheck);
        }
    };

    TEST_CLASS(CheckbuildingIntegration)
    {
    public:
        TEST_METHOD(validTruckPath) //This test will check that valid truck path has any building in the route or not
        {
            struct Map baseMap = populateMap();
            struct Truck trucks[3];
            trucks[0].truckVolume = 50.0;
            trucks[0].truckWeight = 100.0;
            trucks[0].truckRoute = NULL;

            struct Shipment shipment = { 10.0, 5.0, {  'A' , '5' } };

            int size = 0;



            //Passing the given routes
            struct Route routes = { {{'C','5'},{'C','5'}}, 2, 'B' };

            validTruckPaths(shipment, trucks[0], &baseMap, &routes, &size);

            Assert::AreEqual(0, checkBuilding(&routes, &baseMap));

        }

        TEST_METHOD(isDestinationBuilding)//This test will check if the destination given the checkDestination is building or not
        {
            struct Route routes[MAX_ROUTE];
            struct Map baseMap = populateMap();

            //Passing the given routes
            struct Route blueRoute = getBlueRoute();
            struct Route greenRoute = getGreenRoute();
            struct Route yellowRoute = getYellowRoute();

            routes[0] = blueRoute;
            routes[1] = greenRoute;
            routes[2] = yellowRoute;

            struct Shipment shipment = { 10.0, 5.0, {  'A' , '5' } };

            int size = 3;
            int best = bestRoute(routes, shipment, size);
            Assert::IsTrue(best >= 0 && best < size);
            Assert::AreEqual(0, checkBuilding(&routes[best], &baseMap));

        }
        TEST_METHOD(testValidTruckPath)
        {
            // Basic test to check if validTruckPaths returns valid routes when shipment and truck details are correct
            struct Map baseMap = populateMap();

            struct Truck truck;
            truck.truckVolume = 50.0;
            truck.truckWeight = 100.0;
            truck.truckRoute = nullptr;

            struct Shipment shipment = { 10.0, 5.0, {  'A' , 5 } };


            int size = 0;

            struct Route routes = { {'E','1'}, 1, 'B' };


            // Call validTruckPaths to determine valid routes for the shipment
            validTruckPaths(shipment, truck, &baseMap, &routes, &size);

            // Passing the same route in checkBuilding

            Assert::AreEqual(0, checkBuilding(&routes, &baseMap));

        }

        TEST_METHOD(testTruckOverCapacity)
        {
            // Test to check behavior when shipment exceeds truck's weight capacity
            struct Map baseMap = populateMap();

            struct Truck truck;
            truck.truckVolume = 50.0;
            truck.truckWeight = 50.0; // Lower weight capacity
            truck.truckRoute = nullptr;

            struct Shipment shipment = { 100.0, 5.0, {  'A' , 5 } }; // Shipment weight exceeds truck's capacity

            struct Route routes[MAX_ROUTE];
            int size = 0;

            // Call validTruckPaths to determine valid routes for the shipment
            validTruckPaths(shipment, truck, &baseMap, routes, &size);

            // Check if no paths are found since the shipment exceeds capacity
            Assert::AreEqual(0, size);
        }


        TEST_METHOD(testInvalidTruckVolume)
        {
            // Test to check if the function correctly handles invalid truck volume (e.g., zero or negative)
            struct Map baseMap = populateMap();

            struct Truck truck;
            truck.truckVolume = 0.0; // Invalid volume
            truck.truckWeight = 100.0;
            truck.truckRoute = nullptr;

            struct Shipment shipment = { 10.0, 5.0, {  'A' , '5' } };

            struct Route routes[MAX_ROUTE];
            int size = 0;

            // Call validTruckPaths to determine valid routes for the shipment
            validTruckPaths(shipment, truck, &baseMap, routes, &size);

            // Check if no valid paths are found due to invalid truck volume
            Assert::AreEqual(0, size);
        }
    };
}