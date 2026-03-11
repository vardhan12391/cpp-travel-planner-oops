#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include "Trip.h"
#include "Destination.h"
#include "Budget.h"
using namespace std;

void line()
{
    cout << "---------------------------------------------------\n";
}

int main()
{
    cout << "===================================================\n";
    cout << "       TRAVEL PLANNING & BUDGET SYSTEM      \n";
    cout << "===================================================\n";

    string tripName;
    cout << "\nEnter your Trip Name: ";
    getline(cin, tripName);
    Trip myTrip(tripName);

    int n;
    cout << "\nHow many destinations do you want to visit? ";
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    //  Destination input using operator overloading
    for (int i = 0; i < n; i++)
    {
        cout << "\nDestination " << i + 1 << ":\n";
        Destination d;
        cin >> d; //  uses overloaded operator>> in Destination.cpp
        myTrip.add_destination(d);
    }

    // --- ROUTES ---
    cout << "\nEnter distances between destinations (in km):\n";
    vector<Destination> dests = myTrip.getDestinations();

    for (size_t i = 0; i < dests.size(); i++)
    {
        for (size_t j = i + 1; j < dests.size(); j++)
        {
            int dist;
            cout << "Distance from " << dests[i].getCityName()
                 << " to " << dests[j].getCityName() << ": ";
            cin >> dist;
            myTrip.add_route(dests[i].getCityName(), dests[j].getCityName(), dist);
        }
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // --- HOTELS ---
    int h;
    cout << "\nHow many hotels do you want to add? ";
    cin >> h;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    double food_total = 0;
    for (int i = 0; i < h; i++)
    {
        string name, city;
        double price;
        int rating, days;
        char bf;

        cout << "\nHotel " << i + 1 << ":\n";
        cout << "Hotel Name: ";
        getline(cin, name);
        cout << "City: ";
        getline(cin, city);
        cout << "Price per day : ";
        cin >> price;

        try
        {
            cout << "Stay duration (days): ";
            cin >> days;
            if (days <= 0)
                throw invalid_argument("Days must be greater than zero!");
        }
        catch (const invalid_argument &e)
        {
            cout << "Input Error: " << e.what() << endl;
            days = 1; // set default to avoid crash
        }

        cout << "Rating (1 to 5): ";
        cin >> rating;
        cout << "Food included (y/n): ";
        cin >> bf;

        bool food = (bf == 'y' || bf == 'Y');
        myTrip.add_accommodation(Accommodation(name, city, price, rating, food, days));

        if (!food)
        {
            double extra_food;
            cout << "Enter separate food cost for this hotel: ";
            cin >> extra_food;
            food_total += extra_food;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // --- TRANSPORT COST ---
    double rate_per_km = 10.0;
    double total_distance = myTrip.getRoutePlanner().get_total_distance();
    double transport = total_distance * rate_per_km;

    cout << "\nTotal transport distance: " << total_distance << " km";
    cout << "\nTransport rate: " << rate_per_km << "/km";
    cout << "\nTransport Cost: " << transport << endl;

    double hotel_cost = myTrip.cal_stay_cost();

    // --- OTHER COSTS ---
    double misc;
    cout << "\nEnter Miscellaneous Cost (): ";
    cin >> misc;

    char choice;
    cout << "\nSelect Budget Type (E - Economy / L - Luxury): ";
    cin >> choice;

    Budget *tripBudget = nullptr; // base class pointer

    if (choice == 'E' || choice == 'e')
    {
        tripBudget = new Economy(transport, hotel_cost, food_total, misc, 8000);
        cout << "\nEconomy Budget selected.\n";
    }
    else if (choice == 'L' || choice == 'l')
    {
        tripBudget = new Luxury(transport, hotel_cost, food_total, misc, 50);
        cout << "\nLuxury Budget selected (+ Rs 2000 added automatically).\n";
    }
    else
    {
        tripBudget = new Budget(transport, hotel_cost, food_total, misc);
        cout << "\nDefault Basic Budget selected.\n";
    }

    //  Upcasting: set base pointer to Trip
    myTrip.set_budget(tripBudget); //  pass pointer, not copy

    cout << "\n--- Budget Summary (via Upcasting) ---\n";
    tripBudget->showdetails(); // dynamically calls derived version

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "\nEnter your current location (city name): ";
    string current;
    getline(cin, current);

    if (!current.empty())
        myTrip.getRoutePlanner().set_current_location(current);

    cout << "\nGenerating trip summary...\n";
    line();
    myTrip.plantrip(); //  includes shortest route logic
    line();
    myTrip.savetrip();

    cout << "\nTrip planning completed successfully!\n";
    cout << "Have a safe and joyful journey!\n";
    return 0;
}
