#pragma once

#include <set>
#include <string_view>

namespace transport_catalogue {
    namespace output {
        void OutputRouteAbout(std::string_view number, std::size_t number_of_stops = 0, std::size_t unique_stops = 0, uint64_t real_distance = 0, double curvature = 0.0);

        void OutputStopAbout(std::string_view name, std::set<std::string_view> buses, bool flag = true);
    }//namespace output
}//transport_catalogue
