#ifndef DESTINATION_H
#define DESTINATION_H

#include <iostream>
#include <string>
using namespace std;

class Destination
{
private:
    string city_name;
    string country;
    string description;

public:
    friend ostream &operator<<(ostream &out, const Destination &d);
    friend istream &operator>>(istream &in, Destination &d);

    Destination();  //  Default constructor

    Destination(string city_name, string country, string description);
    Destination(string city_name, string country);
    string getCityName() const;
    string getCountry() const;

    void show_details() const;



    ~Destination();
};

#endif
