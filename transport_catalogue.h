#pragma once

#include "geo.h"

#include <deque>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace transport_catalogue {

    enum RouteType {
        Direct,
        Round
    };

    namespace detail {
        template<typename Type>
        class StopHasher {
        public:
            size_t operator()(std::pair<const Type*, const Type*> stop) const {
                return hasher_(stop.first) + 47 * hasher_(stop.second);
            }
        private:
            std::hash<const Type*> hasher_;
        };
    }//namespace detail

    namespace stop {
        struct Stop {
            std::string name;
            geo::Coordinates coordinates;
        };
    }//namespace stop

    namespace bus {
        struct Bus {
            std::string number;
            std::deque<const stop::Stop*> route;
            RouteType route_type = RouteType::Direct;
            double distance = 0.0;
            uint64_t real_distance = 0;
            std::size_t number_of_stops = 0;
            std::size_t unique_stops = 0;
            double curvature = 0.0;
        };
    }//namespace bus

    class TransportCatalogue {
    public:
        void AddStop(std::string_view name, double latitude, double longitude);

        void AddRoute(std::string_view number, char type, std::vector<std::string> stops);

        const bus::Bus* GetRoute(std::string_view name);

        const stop::Stop* GetStop(std::string_view name);

        void RouteAbout(std::string_view name);

        std::set<std::string_view> GetBuses(std::string_view stop);

        void StopAbout(std::string_view stop);

        void SetStopDistance(std::string_view stop1, uint64_t dist, std::string_view stop2);

        uint64_t GetStopDistance(std::string_view stop1, std::string_view stop2);

    private:
        std::deque<bus::Bus> buses_;
        std::deque<stop::Stop> stops_;
        std::unordered_map<std::pair<const stop::Stop*, const stop::Stop*>, uint64_t, detail::StopHasher<stop::Stop>> di_to_stop;
    };
}//namespace transport_catalogue
