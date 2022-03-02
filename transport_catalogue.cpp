#include "stat_reader.h"
#include "transport_catalogue.h"

#include <algorithm>

using namespace std;

namespace transport_catalogue {
    void TransportCatalogue::AddStop(string_view name, double latitude, double longitude) {
        stop::Stop stop;
        stop.name = { name.begin(), name.end() };
        stop.coordinates = {latitude, longitude};
        stops_.push_back(move(stop));
    }

    void TransportCatalogue::AddRoute(string_view number, char type, std::vector<std::string> stops) {
        bus::Bus result;
        result.number = { number.begin(), number.end() };
        if (type == '-') result.route_type = RouteType::Direct;
        if (type == '>') result.route_type = RouteType::Round;
        for (auto& stop : stops) {
            for (auto& elem : stops_) {
                if (elem.name.compare(stop) == 0) {
                    result.route.push_back(&elem);
                    break;
                }
            }
        }
        if (result.route_type == RouteType::Direct) {
            auto temp = result.route;
            for (int i = temp.size() - 2; i >= 0; --i) {
                result.route.push_back(result.route[i]);
            }
        }
        result.number_of_stops = result.route.size();
        for (size_t i = 0; i < result.route.size() - 1; ++i) {
            result.distance += ComputeDistance(result.route[i]->coordinates, result.route[i + 1]->coordinates);
            result.real_distance += GetStopDistance(result.route[i]->name, result.route[i + 1]->name);
        }

        auto temp = result.route;
        sort(temp.begin(), temp.end());
        auto it = unique(temp.begin(), temp.end());
        temp.resize(distance(temp.begin(), it));
        result.unique_stops = temp.size();

        result.curvature = result.real_distance / result.distance;

        buses_.push_back(move(result));
    }

    const bus::Bus* TransportCatalogue::GetRoute(string_view name) {
        for (auto& elem : buses_) {
            if (elem.number.compare({name.begin(), name.end()}) == 0) {
                return &elem;
            }
        }
        return {};
    }

    const stop::Stop* TransportCatalogue::GetStop(string_view name) {
        for (auto& elem : stops_) {
            if (elem.name.compare({name.begin(), name.end()}) == 0) {
                return &elem;
            }
        }
        return {};
    }

    void TransportCatalogue::RouteAbout(string_view name) {
        const bus::Bus* bus = GetRoute(name);
        if (bus->number_of_stops == 0) {
            output::OutputRouteAbout(name);
        }
        else {
            output::OutputRouteAbout(name, bus->number_of_stops, bus->unique_stops, bus->real_distance, bus->curvature);
        }
    }

    set<string_view> TransportCatalogue::GetBuses(string_view stop) {
        set<string_view> result;
        for (auto& bus : buses_) {
            for (auto& stop_p : bus.route) {
                string stop_str = {stop.begin(), stop.end()};
                if (stop_p->name == stop_str) {
                    result.insert(bus.number);
                    break;
                }
            }
        }
        return result;
    }

    void TransportCatalogue::StopAbout(string_view stop) {
        set<string_view> buses = GetBuses(stop);
        bool flag = true;
        if (GetStop(stop)->name == "") {
            flag = false;
        }
        output::OutputStopAbout(stop, buses, flag);
    }

    void TransportCatalogue::SetStopDistance(std::string_view stop1, uint64_t dist, std::string_view stop2) {
        for (auto& stop : stops_) {
            string stop1_str = {stop1.begin(), stop1.end()};
            if (stop.name == stop1_str) {
                for (auto& another_stop : stops_) {
                    string stop2_str = {stop2.begin(), stop2.end()};
                    if (another_stop.name == stop2_str) {
                        stop.di_to_stop.insert({{&stop, &another_stop}, dist});
                        return;
                    }
                }
            }
        }
    }

    uint64_t TransportCatalogue::GetStopDistance(std::string_view stop1, std::string_view stop2) {
        auto p_stop1 = GetStop(stop1);
        auto p_stop2 = GetStop(stop2);
        if (p_stop1 != nullptr && p_stop2 != nullptr) {
            if (p_stop1->di_to_stop.count( {p_stop1, p_stop2} )) {
                return p_stop1->di_to_stop.at( {p_stop1, p_stop2} );
            }
            else
            {
                if (p_stop2->di_to_stop.count( {p_stop2, p_stop1 } )) {
                    return p_stop2->di_to_stop.at( {p_stop2, p_stop1} );
                }
            }
        }
        return 0;
    }
}//namespace transport_catalogue
