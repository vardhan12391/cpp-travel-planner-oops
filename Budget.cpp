#include "Budget.h"
#include <iostream>
using namespace std;

// ---------------- Budget class ----------------
Budget::Budget(double transport_cost, double accommodation_cost, double food_cost, double misc_cost)
{
    try {
        if (transport_cost < 0 || accommodation_cost < 0)
            throw invalid_argument("Cost values cannot be negative!");
    }
    catch (const exception &e) {
        cout << "Budget Error: " << e.what() << endl;
        transport_cost = accommodation_cost = 0;
    }

    this->transport_cost = transport_cost;
    this->accommodation_cost = accommodation_cost;
    this->food_cost = food_cost;
    this->misc_cost = misc_cost;
    this->total_cost = 0;
}

double Budget::cost()
{
    // always recompute to avoid stale totals
    total_cost = transport_cost + accommodation_cost + food_cost + misc_cost;
    return total_cost;
}

void Budget::showdetails()
{
    // Ensure total is up-to-date
    cost();
    cout << "Transport Cost     : " << transport_cost << endl;
    cout << "Accommodation Cost : " << accommodation_cost << endl;
    cout << "Food Cost          : " << food_cost << endl;
    cout << "Miscellaneous Cost : " << misc_cost << endl;
    cout << "Total Trip Budget  : " << total_cost << endl;
}

void Budget::add_food_cost(double extra_food)
{
    food_cost += extra_food;
    total_cost = transport_cost + accommodation_cost + food_cost + misc_cost;
    cout << "Additional food cost of " << extra_food << " added to the budget.\n";
    cout << "Updated Total Trip Budget: " << total_cost << endl;
}

Budget::~Budget()
{
    //cout << "Destructor called for Budget object" << endl;
}

// ---------------- Economy class ----------------
Economy::Economy(double t, double a, double f, double m, int d)
    : Budget(t, a, f, m), savings_money(d)
{
    cout << "Economy constructor called" << endl;
}


void Economy::showdetailsEconomy()
{
    cout << "--- Economy Budget Details ---\n";
    cost();
    showdetails();
    cout << "Savings Money: " << savings_money << endl;
}

Economy::~Economy()
{
    cout << "Destructor called for Economy object" << endl;
}

// ---------------- Luxury class ----------------
Luxury::Luxury(double t, double a, double f, double m, int p)
    : Budget(t, a, f, m), premium_basiccost(p)
{
    cout << "Luxury constructor called" << endl;
}

double Luxury::cost()
{
    double base = Budget::cost();
    return base + 2000;  //  add ₹2000 for luxury
}

void Luxury::showdetails()
{
    double final = cost();
    cout << "--- Luxury Budget Details ---\n";
    Budget::showdetails();
    cout << "Luxury Add-on: Rs 2000\n";
    cout << "Final Total  : Rs" << final << endl;
}


Luxury::~Luxury()
{
    cout << "Destructor called for Luxury object" << endl;
}


ostream &operator<<(ostream &out, const Budget &b)
{
    out << "Transport: " << b.transport_cost
        << " | Accommodation: " << b.accommodation_cost
        << " | Food: " << b.food_cost
        << " | Misc: " << b.misc_cost
        << " | Total: " << b.transport_cost + b.accommodation_cost + b.food_cost + b.misc_cost;
    return out;
}

istream &operator>>(istream &in, Budget &b)
{
    cout << "Enter Transport Cost: ";
    in >> b.transport_cost;
    cout << "Enter Accommodation Cost: ";
    in >> b.accommodation_cost;
    cout << "Enter Food Cost: ";
    in >> b.food_cost;
    cout << "Enter Miscellaneous Cost: ";
    in >> b.misc_cost;
    return in;
}
