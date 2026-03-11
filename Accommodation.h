#ifndef ACCOMMODATION_H
#define ACCOMMODATION_H

#include <iostream>
#include <string>
using namespace std;

template <typename T>
T calculate_total_cost(T price_per_day, int days) {
    return price_per_day * days;
}

class Accommodation
{
private:
    string hotel_name;
    string city;
    double price_per_day;
    int rating;
    bool food_included;
    int stay_days; // Added stay duration

public:
    Accommodation(string name, string c, double price, int rate, bool food, int days);

    double cal_stay_cost(int days) const;
    void show_details() const;
    ~Accommodation();

    bool isFoodIncluded() const { return food_included; }
    string getCity() const { return city; }
    string getHotelName() const { return hotel_name; }
    int getStayDays() const { return stay_days; } // getter for days
};
#endif
