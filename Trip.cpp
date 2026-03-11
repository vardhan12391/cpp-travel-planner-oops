#include "Trip.h"
#include <iostream>
#include <fstream>
using namespace std;


template <typename T>
void display_details(const vector<T> &items) {
    for (const auto &item : items)
        item.show_details();
}


Trip::Trip(string name) : tripname(name) {}

void Trip::add_destination(const Destination &d)
{
    destinations.push_back(d);
}

void Trip::add_accommodation(const Accommodation &a)
{
    hotels.push_back(a);
}

void Trip::add_route(string from, string to, int dist)
{
    route.add_route(from, to, dist);
}

void Trip::set_budget(Budget* b)
{
    trip_budget = b;
}


void Trip::plantrip()
{
    cout << "\n===== Trip Plan: " << tripname << " =====\n";

    // --- Destinations ---
    cout << "\n--- Destinations ---\n";
    display_details(destinations);

    // --- Accommodations ---
    cout << "\n--- Accommodations ---\n";
    display_details(hotels);



    // --- Routes ---
    cout << "\n--- Routes ---\n";
    route.display_routes();

    //  NEW: Optimal tour printed immediately after routes
    if (!route.get_current_location().empty())
    {
        auto result = route.compute_optimal_tour(route.get_current_location());
        if (!result.first.empty())
        {
            cout << "\n===== Optimal tour visiting all cities (start: "
                 << route.get_current_location() << ") =====\n";
            for (size_t i = 0; i < result.first.size(); ++i)
            {
                cout << result.first[i];
                if (i + 1 < result.first.size()) cout << " -> ";
            }
            cout << "\nTotal distance for the tour (approx): " << result.second << " km\n";
        }
        else
        {
            cout << "\n(No optimal tour could be generated — check routes connectivity.)\n";
        }
    }

    // --- Budget Summary ---
    cout << "\n--- Budget Summary ---\n";
    trip_budget->showdetails();
}


void Trip::savetrip()
{
    ofstream file(tripname + "_Summary.txt");
    if (!file)
    {
        cout << "Error saving trip summary!\n";
        return;
    }
    file << "===== Trip Summary: " << tripname << " =====\n";
    file << "\nDestinations:\n";
    for (auto &d : destinations)
        file << "- " << d.getCityName() << " (" << d.getCountry() << ")\n";
    file << "\nTotal Estimated Budget: Rs." << trip_budget->cost() << "\n";
    file.close();
    cout << "Trip summary saved as " << tripname << "_Summary.txt \n";
}

double Trip::cal_stay_cost() const
{
    double total = 0;
    for (const auto &hotel : hotels)
    {
        total += hotel.cal_stay_cost(hotel.getStayDays());
    }
    return total;
}

RoutePlanner &Trip::getRoutePlanner() { return route; }
vector<Destination> &Trip::getDestinations() { return destinations; }

Trip::~Trip()
{
     delete trip_budget;   // free memory safely
    //cout << "Destructor called for Trip: " << tripname << endl;
}

ostream &operator<<(ostream &out, const Trip &t)
{
    out << "===== Trip: " << t.tripname << " =====\n";

    out << "\n--- Destinations ---\n";
    for (const auto &d : t.destinations)
        out << d << endl;

    out << "\n--- Budget ---\n";
    out << t.trip_budget << endl;

    return out;
}

istream &operator>>(istream &in, Trip &t)
{
    cout << "Enter Trip Name: ";
    getline(in, t.tripname);

    int n;
    cout << "Enter number of destinations: ";
    in >> n;
    in.ignore();

    t.destinations.clear();
    for (int i = 0; i < n; ++i)
    {
        Destination d;
        cout << "\nDestination " << i + 1 << ":\n";
        in >> d;
        t.destinations.push_back(d);
    }

    // cout << "\nEnter Budget Details:\n";
    // in >> t.trip_budget;

    return in;
}
