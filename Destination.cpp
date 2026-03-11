#include "Destination.h"
#include <iostream>
using namespace std;

Destination::Destination(string city_name, string country, string description)
{
    this->city_name = city_name;
    this->country = country;
    this->description = description;
}

void Destination::show_details() const
{
    cout << "City: " << city_name << ", Country: " << country
         << "\nDescription: " << description << "\n";
}

string Destination::getCityName() const
{
    return city_name;
}
string Destination::getCountry() const
{
    return country;
}

Destination::Destination() : city_name(""), country(""), description("") {}



Destination::~Destination()
{
    //cout<<"Destructor called for destination: "<<city_name<<endl;
}

ostream &operator<<(ostream &out, const Destination &d)
{
    out << "City: " << d.city_name
        << " | Country: " << d.country
        << " | Description: " << d.description;
    return out;
}

istream &operator>>(istream &in, Destination &d)
{
    cout << "Enter City: ";
    getline(in, d.city_name);
    cout << "Enter Country: ";
    getline(in, d.country);
    cout << "Enter Description: ";
    getline(in, d.description);
    return in;
}
