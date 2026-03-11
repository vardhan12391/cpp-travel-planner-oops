#ifndef ROUTEPLANNER_H
#define ROUTEPLANNER_H

#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <climits>
#include <string>
#include <algorithm>
using namespace std;

class RoutePlanner
{
private:
    map<string, vector<pair<string, int>>> routes;
    string current_location;

public:
    void add_route(const string &from, const string &to, int distance);
    void set_current_location(const string &location);
    string get_current_location() const;
    int get_total_distance() const; //  total distance calculator

    void display_routes() const;
    void find_shortest_route(const string &from, const string &to);
    void suggest_next_destination();

    // NEW: helpers for computing optimal tour (visit all cities)
    vector<string> get_all_cities() const;
    // Compute an optimal tour starting at 'start'. Returns pair<path, total_distance>.
    // If empty path returned, indicates failure (e.g., start not known or disconnected).
    pair<vector<string>, int> compute_optimal_tour(const string &start) const;
};

#endif
