#include "input_reader.h"
#include "stat_reader.h"
#include "transport_catalogue.h"

#include <iostream>
#include <iomanip>
#include <string_view>

using namespace std;
using namespace transport_catalogue;
using namespace query;

int main() {
    int query_count;
    cin >> query_count;

    vector<Query> stop_vec;
    vector<Query> bus_vec;

    for (int i = 0; i < query_count; ++i) {
        Query q;
        cin >> q;
        switch (q.type) {
            case QueryType::StopX:
                stop_vec.push_back(q);
                break;
            case QueryType::BusX:
                bus_vec.push_back(q);
                break;
        }
    }

    TransportCatalogue tc;

    for (auto & query : stop_vec) {
        tc.AddStop(query.stop, query.latitude, query.longitude);
    }

    for (auto & query : stop_vec) {
        for (auto& [stop2, dist] : query.di_to_stop) {
            tc.SetStopDistance(query.stop, dist, stop2);
        }
    }

    for (auto& query : bus_vec) {
        tc.AddRoute(query.bus, query.symbol, query.stops);
    }

    query_count = 0;
    cin >> query_count;

    for (int i = 0; i < query_count; ++i) {
        Query q;
        cin >> q;
        switch (q.type) {
            case QueryType::StopX:
                tc.StopAbout(q.stop);
                break;
            case QueryType::BusX:
                tc.RouteAbout(q.bus);
                break;
        }
    }
    return 0;
}
