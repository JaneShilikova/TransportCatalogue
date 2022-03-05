#include "stat_reader.h"
#include "transport_catalogue.h"

#include <algorithm>

using namespace std;

namespace transport_catalogue {
    void TransportCatalogue::AddStop(string_view name, double latitude, double longitude) {
        stops_.push_back({{ name.begin(), name.end() }, latitude, longitude});
    }

    void TransportCatalogue::AddRoute(string_view number, char type, std::vector<std::string> stops) {
        bus::Bus result;
        result.number = { number.begin(), number.end() };
        if (type == '-') result.route_type = RouteType::Direct;
        if (type == '>') result.route_type = RouteType::Round;
        for (auto& stop : stops) {
            auto found_stop = GetStop(stop);
            if (found_stop != nullptr) {
                result.route.push_back(found_stop);
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
            string name_str = {name.begin(), name.end()};
            if (elem.number.compare(name_str) == 0) {
                return &elem;
            }
        }
        return nullptr;
    }

    const stop::Stop* TransportCatalogue::GetStop(string_view name) {
        for (auto& elem : stops_) {
            string name_str = {name.begin(), name.end()};
            if (elem.name.compare(name_str) == 0) {
                return &elem;
            }
        }
        return nullptr;
    }

    void TransportCatalogue::RouteAbout(string_view name) {
        const bus::Bus* bus = GetRoute(name);
        if (bus == nullptr) {
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
        auto found_stop = GetStop(stop);
        if (found_stop == nullptr) {
            flag = false;
        }
        output::OutputStopAbout(stop, buses, flag);
    }

    void TransportCatalogue::SetStopDistance(std::string_view stop1, uint64_t dist, std::string_view stop2) {
        auto p_stop1 = GetStop(stop1);
        auto p_stop2 = GetStop(stop2);
        if (p_stop1 != nullptr && p_stop2 != nullptr) {
            di_to_stop[{p_stop1, p_stop2}] = dist;
        }
    }

    uint64_t TransportCatalogue::GetStopDistance(std::string_view stop1, std::string_view stop2) {
        auto p_stop1 = GetStop(stop1);
        auto p_stop2 = GetStop(stop2);
        if (p_stop1 != nullptr && p_stop2 != nullptr) {
            if (di_to_stop.count( {p_stop1, p_stop2} )) {
                return di_to_stop.at( {p_stop1, p_stop2} );
            }
            else
            {
                if (di_to_stop.count( {p_stop2, p_stop1 } )) {
                    return di_to_stop.at( {p_stop2, p_stop1} );
                }
            }
        }
        return 0;
    }
}//namespace transport_catalogue
