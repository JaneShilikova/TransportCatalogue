#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace transport_catalogue {
    namespace query {
        enum class QueryType {
            StopX,
            BusX
        };

        struct Query {
            QueryType type;
            std::string bus;
            char symbol;
            std::vector<std::string> stops;
            std::map<std::string, uint64_t> di_to_stop;
            std::string stop;
            std::string lat;
            std::string lon;
            double latitude = 0.0;
            double longitude = 0.0;
        };

        std::istream& operator>>(std::istream& is, Query& q);
    }//namespace query
}//namespace transport_catalogu
