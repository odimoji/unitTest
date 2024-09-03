#define BOOST_TEST_MODULE OdisMostEquitorialWaypointTestModule
#include <boost/test/included/unit_test.hpp>

#include "gpx-parser.h"
#include "track.h"
#include "earth.h"


using namespace GPS;


using std::cout;
using std::endl;

BOOST_AUTO_TEST_SUITE(MostEquitorialWaypointTestModule)
Waypoint wp1 = Waypoint(1,10,100);
Waypoint wp2 = Waypoint(2,20,200);
Waypoint wp3 = Waypoint(3,30,300);
Waypoint wp4 = Waypoint(4,40,400);
Waypoint wp0 = Waypoint(8,80,800);
Waypoint nearZeroLatitudeWaypoint = Waypoint(0.001, 0.0, 0.0);



Trackpoint tp1 = Trackpoint{wp1,1000};
Trackpoint tp2 = Trackpoint{wp2,2000};
Trackpoint tp3 = Trackpoint{wp3,3000};
Trackpoint tp4 = Trackpoint{wp4,4000};
Trackpoint tp0 = Trackpoint{wp0,8000};
Trackpoint tp9 = Trackpoint{nearZeroLatitudeWaypoint,256};
std::vector<Trackpoint> trackpoints = {tp1, tp2, tp3, tp4};

Trackpoint tp5 = Trackpoint{Waypoint(5,50,500),5000};
Trackpoint tp56 = Trackpoint{Waypoint(5,60,600),6000};
Trackpoint tp7 = Trackpoint{Waypoint(-5,50,500),5000};

std::vector<Trackpoint> trackpoints2 ={tp5, tp56, tp0 };
std::vector<Trackpoint> trackpoints9 ={tp9, tp1, tp2};


BOOST_AUTO_TEST_CASE(NoWaypoint){
    std::vector<Trackpoint> trackpoints3 = {};


    // Create a track with no waypoint
    Track track(trackpoints3);

    // Ensure that error indicating no trackpoints is thrown
    BOOST_CHECK_THROW(track.mostEquatorialWaypoint(),std::domain_error);


}

BOOST_AUTO_TEST_CASE(OneClosestWaypoint){
    // Create a track with a single closest waypoint
    Track track(trackpoints);

    // Ensure the single closest waypoint is returned as the most equatorial waypoint
    BOOST_CHECK(track.mostEquatorialWaypoint().latitude() == wp1.latitude());


}

BOOST_AUTO_TEST_CASE(MultipleClosestWaypoint){

    // Create a track with a more than  closest waypoint
    Track track(trackpoints2);

    // Ensure the closest waypoint which comes first is returned as the most equatorial waypoint
    BOOST_CHECK_EQUAL(track.mostEquatorialWaypoint().latitude(), tp5.waypoint.latitude());


}

BOOST_AUTO_TEST_CASE(SingleWaypoint){
    // Create a track with a single waypoint
    std::vector<Trackpoint> trackpoints = {tp1};
    Track track(trackpoints);

    // Ensure the single waypoint is returned as the most equatorial waypoint
    BOOST_CHECK_EQUAL(track.mostEquatorialWaypoint().latitude(), wp1.latitude());
}

BOOST_AUTO_TEST_CASE(SingleNegativeWaypoint){
    // Create a track with a single waypoint
    std::vector<Trackpoint> trackpoints = {tp7};
    Track track(trackpoints);

    // Ensure the single waypoint is returned as the most equatorial waypoint
    BOOST_CHECK_EQUAL(track.mostEquatorialWaypoint().latitude(), tp7.waypoint.latitude());
}



BOOST_AUTO_TEST_CASE(Extremes) {
    // Create a track with waypoints at the North Pole and the South Pole
    std::vector<Trackpoint> trackpoints4 = {
        Trackpoint{Waypoint(90, 0, 0), 1000},    // North Pole
        Trackpoint{Waypoint(-90, 0, 0), 2000}    // South Pole
    };

    Track track(trackpoints4);

    // Ensure that the North Pole is returned as the most equatorial waypoint
    BOOST_CHECK_EQUAL(track.mostEquatorialWaypoint().latitude(), 90.0);
}

BOOST_AUTO_TEST_CASE(MixedHemisphere) {
    // Create a track with waypoints in both the northern and southern hemispheres
    std::vector<Trackpoint> trackpoints5 = {
        Trackpoint{Waypoint(10, 50, 100), 1000},   // Northern Hemisphere
        Trackpoint{Waypoint(-20, 40, 200), 2000},  // Southern Hemisphere
        Trackpoint{Waypoint(30, 60, 300), 3000}    // Northern Hemisphere
    };

    Track track(trackpoints5);

    // Ensure that the waypoint in the Northern Hemisphere is returned as the most equatorial waypoint
    BOOST_CHECK_EQUAL(track.mostEquatorialWaypoint().latitude(), 10.0);
}

BOOST_AUTO_TEST_CASE(NegativeClosestWaypoint) {
    // Create a track with its closest waypoint being negative
    std::vector<Trackpoint> trackpoints5 = {
        Trackpoint{Waypoint(-10, 50, 100), 1000},   // Northern Hemisphere
        Trackpoint{Waypoint(20, 40, 200), 2000},  // Southern Hemisphere
        Trackpoint{Waypoint(30, 60, 300), 3000}    // Northern Hemisphere
    };

    Track track(trackpoints5);

    // Ensure that the waypoint being returned is the one containing the negative waypoint.
    BOOST_CHECK_EQUAL(track.mostEquatorialWaypoint().latitude(), -10.0);
}



BOOST_AUTO_TEST_CASE(NearZeroLatitudeBoundary) {
    // Create a track with a waypoint near zero latitude (close to the equator)

    Track track(trackpoints9);


    // Ensure that the waypoint near zero latitude is returned as the most equatorial waypoint
    BOOST_CHECK_EQUAL(track.mostEquatorialWaypoint().latitude(), 0.001);
}






}
