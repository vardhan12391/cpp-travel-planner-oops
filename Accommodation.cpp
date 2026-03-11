#include "Accommodation.h"
#include <iostream>
using namespace std;

Accommodation::Accommodation(string name, string c, double price, int rate, bool food, int days)
    : hotel_name(name), city(c), price_per_day(price),
      rating(rate), food_included(food), stay_days(days) {}

void Accommodation::show_details() const
{
    cout << "Hotel Name: " << hotel_name
         << " | City: " << city
         << " | Price/Day: " << price_per_day
         << " | Days: " << stay_days
         << " | Rating: " << rating
         << (food_included ? " | Food Included" : " | No Food Included")
         << endl;
}

double Accommodation::cal_stay_cost(int days) const {
    return calculate_total_cost(price_per_day, days);
}


Accommodation::~Accommodation()
{
    //cout << "Destructor called for hotel: " << hotel_name << endl;
}
