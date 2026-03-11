#ifndef TRIP_H
#define TRIP_H

#include "Destination.h"
#include "Budget.h"
#include "Accommodation.h"
#include "RoutePlanner.h"
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

class Trip
{
    string tripname;
    vector<Destination> destinations;
    vector<Accommodation> hotels;
    Budget *trip_budget; // pointer to base class (needed for upcasting)

    RoutePlanner route;

public:
    Trip(string name = "Default");
    void add_destination(const Destination &d);
    void add_accommodation(const Accommodation &a);
    void add_route(string from, string to, int dist);
    void set_budget(Budget* b);

    void plantrip();
    void savetrip();
    double cal_stay_cost() const; // changed: no parameter
    friend ostream &operator<<(ostream &out, const Trip &t);
    friend istream &operator>>(istream &in, Trip &t);

    RoutePlanner &getRoutePlanner();
    vector<Destination> &getDestinations();

    ~Trip();
};
#endif
