#include "RoutePlanner.h"
using namespace std;

void RoutePlanner::add_route(const string &from, const string &to, int distance)
{
    routes[from].push_back({to, distance});
    routes[to].push_back({from, distance});
}

void RoutePlanner::set_current_location(const string &location)
{
    if (routes.find(location) != routes.end())
    {
        current_location = location;
        cout << "Current location set to: " << current_location << endl;
    }
    else
    {
        cout << "Location not found in route data.\n";
    }
}

string RoutePlanner::get_current_location() const
{
    return current_location;
}

void RoutePlanner::display_routes() const
{
    cout << "\n===== All Routes =====\n";
    for (auto &r : routes)
    {
        cout << r.first << " -> ";
        for (auto &dest : r.second)
        {
            cout << dest.first << "(" << dest.second << " km)  ";
        }
        cout << endl;
    }
}

int RoutePlanner::get_total_distance() const
{
    int total = 0;
    map<pair<string, string>, bool> counted; // avoid double-counting

    for (const auto &fromEntry : routes)
    {
        const string &from = fromEntry.first;
        for (const auto &toEntry : fromEntry.second)
        {
            string to = toEntry.first;
            int distance = toEntry.second;

            pair<string, string> key1 = {from, to};
            pair<string, string> key2 = {to, from};

            // count each route only once
            if (!counted[key1] && !counted[key2])
            {
                total += distance;
                counted[key1] = counted[key2] = true;
            }
        }
    }
    return total;
}

void RoutePlanner::find_shortest_route(const string &from, const string &to)
{

    try
    {
        if (routes.find(from) == routes.end() || routes.find(to) == routes.end())
            throw runtime_error("City not found in routes!");
    }
    catch (const exception &e)
    {
        cout << "Route Error: " << e.what() << endl;
    }
    
    map<string, int> dist;
    map<string, string> prev;
    for (auto &r : routes)
        dist[r.first] = INT_MAX;
    dist[from] = 0;

    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;
    pq.push({0, from});

    while (!pq.empty())
    {
        auto top = pq.top();
        pq.pop();
        int d = top.first;
        string city = top.second;

        if (d > dist[city])
            continue;

        for (auto &edge : routes[city])
        {
            string neighbor = edge.first;
            int w = edge.second;

            if (dist[neighbor] > dist[city] + w)
            {
                dist[neighbor] = dist[city] + w;
                prev[neighbor] = city;
                pq.push({dist[neighbor], neighbor});
            }
        }
    }

    if (dist[to] == INT_MAX)
    {
        cout << "No path found between " << from << " and " << to << endl;
        return;
    }

    vector<string> path;
    for (string at = to; at != ""; at = prev[at])
        path.push_back(at);
    reverse(path.begin(), path.end());

    cout << "\n===== Shortest Route =====\n";
    for (auto &city : path)
        cout << city << " ";
    cout << "\nTotal Distance: " << dist[to] << " km\n";
}

void RoutePlanner::suggest_next_destination()
{
    if (current_location.empty())
    {
        cout << "Current location not set.\n";
        return;
    }

    if (routes[current_location].empty())
    {
        cout << "No destinations connected to " << current_location << endl;
        return;
    }

    auto next = min_element(routes[current_location].begin(), routes[current_location].end(),
                            [](auto &a, auto &b)
                            { return a.second < b.second; });

    cout << "Nearest next destination from " << current_location << " is: "
         << next->first << " (" << next->second << " km away)\n";
}

// ---------- NEW: gather all unique city names ----------
vector<string> RoutePlanner::get_all_cities() const
{
    vector<string> cities;
    for (const auto &p : routes)
        cities.push_back(p.first);
    return cities;
}

// ---------- NEW: Held-Karp DP to compute optimal Hamiltonian path starting at 'start' ----------
pair<vector<string>, int> RoutePlanner::compute_optimal_tour(const string &start) const
{
    // Gather cities
    vector<string> cities = get_all_cities();
    int n = (int)cities.size();
    if (n == 0)
        return {{}, 0};

    // map city -> index
    map<string, int> idx;
    for (int i = 0; i < n; ++i)
        idx[cities[i]] = i;

    if (idx.find(start) == idx.end())
    {
        // start not present
        return {{}, -1};
    }

    // Build adjacency matrix (use large cost if no direct edge)
    const int INF = 1000000000;
    vector<vector<int>> dist(n, vector<int>(n, INF));
    for (int i = 0; i < n; ++i)
        dist[i][i] = 0;
    for (const auto &fromEntry : routes)
    {
        int u = idx.at(fromEntry.first);
        for (const auto &edge : fromEntry.second)
        {
            int v = idx.at(edge.first);
            dist[u][v] = min(dist[u][v], edge.second); // take min if multiple
        }
    }

    // Held-Karp DP
    int startIdx = idx[start];
    int FULL = (1 << n);
    // dp[mask][v] = min cost to reach set=mask and end at v
    vector<vector<long long>> dp(FULL, vector<long long>(n, (long long)INF));
    vector<vector<int>> parent(FULL, vector<int>(n, -1));

    dp[1 << startIdx][startIdx] = 0;

    for (int mask = 0; mask < FULL; ++mask)
    {
        if (!(mask & (1 << startIdx)))
            continue; // only consider masks including start
        for (int u = 0; u < n; ++u)
        {
            if (!(mask & (1 << u)))
                continue;
            if (dp[mask][u] >= INF)
                continue;
            for (int v = 0; v < n; ++v)
            {
                if (mask & (1 << v))
                    continue;
                if (dist[u][v] >= INF)
                    continue; // no edge
                int nmask = mask | (1 << v);
                long long nd = dp[mask][u] + dist[u][v];
                if (nd < dp[nmask][v])
                {
                    dp[nmask][v] = nd;
                    parent[nmask][v] = u;
                }
            }
        }
    }

    // Find best end node for full mask (we don't force return to start)
    int fullMask = FULL - 1;
    long long bestCost = (long long)INF;
    int bestEnd = -1;
    for (int v = 0; v < n; ++v)
    {
        if (dp[fullMask][v] < bestCost)
        {
            bestCost = dp[fullMask][v];
            bestEnd = v;
        }
    }

    if (bestEnd == -1 || bestCost >= INF)
    {
        // Could be disconnected graph: try approximate greedy fallback (nearest neighbor)
        // We'll build a simple fallback tour:
        vector<bool> used(n, false);
        vector<string> path;
        int cur = startIdx;
        used[cur] = true;
        path.push_back(cities[cur]);
        int total = 0;
        for (int step = 1; step < n; ++step)
        {
            int next = -1;
            int bestd = INF;
            for (int v = 0; v < n; ++v)
                if (!used[v] && dist[cur][v] < bestd)
                {
                    bestd = dist[cur][v];
                    next = v;
                }
            if (next == -1 || bestd >= INF)
            {
                // fail
                return {{}, -2};
            }
            used[next] = true;
            path.push_back(cities[next]);
            total += bestd;
            cur = next;
        }
        return {path, total};
    }

    // Reconstruct path from parent
    vector<int> rev;
    int mask = fullMask;
    int cur = bestEnd;
    while (cur != -1)
    {
        rev.push_back(cur);
        int p = parent[mask][cur];
        if (p == -1)
            break;
        mask ^= (1 << cur);
        cur = p;
    }
    // should end at startIdx
    reverse(rev.begin(), rev.end());
    vector<string> tour;
    for (int id : rev)
        tour.push_back(cities[id]);

    return {tour, (int)bestCost};
}
