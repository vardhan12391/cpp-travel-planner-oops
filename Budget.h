#ifndef BUDGET_H
#define BUDGET_H

#include <iostream>
using namespace std;

// ---------------- Base class ----------------
class Budget
{
private:
    double transport_cost;
    double accommodation_cost;
    double food_cost;
    double misc_cost;
    double total_cost;

public:
    Budget(double transport_cost = 0,
           double accommodation_cost = 0,
           double food_cost = 0,
           double misc_cost = 0);

    virtual double cost();
    virtual void showdetails();
    friend ostream &operator<<(ostream &out, const Budget &b);
    friend istream &operator>>(istream &in, Budget &b);

    virtual ~Budget();
    // Operator overloading

    void add_food_cost(double extra_food);
    void set_transport_cost(double cost);
};

// ---------------- Derived class 1 ----------------
class Economy : virtual public Budget
{
private:
    int savings_money;

public:
    Economy(double t, double a, double f, double m, int d);
    virtual ~Economy();
    void showdetailsEconomy();
};

// ---------------- Derived class 2 ----------------
class Luxury : virtual public Budget
{
private:
    int premium_basiccost;

public:
    Luxury(double t = 0,
           double a = 0,
           double f = 0,
           double m = 0,
           int premium_basiccost = 25);
    double cost() override;
    void showdetails() override;
    virtual ~Luxury();
    void show3();
};

#endif
