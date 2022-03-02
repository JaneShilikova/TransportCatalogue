#include "input_reader.h"

#include <unordered_map>

using namespace std;

namespace transport_catalogue {
    namespace query {

        std::istream& operator>>(std::istream& is, Query& q) {
            static std::unordered_map<std::string, QueryType> const table = {
                {"Stop", QueryType::StopX}, {"Bus", QueryType::BusX}
            };
            std::string type;
            is >> type;

            switch (table.at(type)) {
                case QueryType::StopX: {
                    q.type = QueryType::StopX;
                    std::string s;
                    getline(is, s);
                    size_t position = s.find(":");

                    if (position == std::string::npos) {
                        for (size_t i = 1; i < s.size(); ++i) {
                            q.stop += s[i];
                        }
                        break;
                    }

                    for (size_t i = 1; i < position; ++i) {
                        q.stop += s[i];
                    }

                    position += 2;
                    auto t = position;
                    position = s.find(",");

                    for (size_t i = t; i < position; ++i) {
                        q.lat += s[i];
                    }

                    size_t comma = s.find(",", position + 1);

                    if (comma != std::string::npos) {
                        for (size_t i = position + 1; i < comma; ++i) {
                            q.lon += s[i];
                        }

                        while (comma < s.size()) {
                            auto temp = comma;
                            comma = s.find(",", comma + 1);
                            size_t m_pos = s.find("m", temp);
                            string number, str_stop;

                            for (size_t i = temp + 2; i < m_pos; ++i) {
                                number += s[i];
                            }

                            m_pos += 4;

                            if (comma != std::string::npos) {
                                for (size_t i = m_pos + 1; i < comma; ++i) {
                                    str_stop += s[i];
                                }
                            }
                            else {
                                for (size_t i = m_pos + 1; i < s.size(); ++i) {
                                    str_stop += s[i];
                                }
                            }

                            q.di_to_stop[str_stop] = stoll(number);
                        }
                    }
                    else {
                        for (size_t i = position + 1; i < s.size(); ++i) {
                            q.lon += s[i];
                        }
                    }
                    q.latitude = stod(q.lat);
                    q.longitude = stod(q.lon);
                }
                    break;
                case QueryType::BusX: {
                    q.type = QueryType::BusX;
                    std::string str;
                    getline(is, str);
                    size_t pos = str.find(":");

                    if (pos == std::string::npos) {
                        for (size_t i = 1; i < str.size(); ++i) {
                            q.bus += str[i];
                        }
                        break;
                    }

                    for (size_t i = 1; i < pos; ++i) {
                        q.bus += str[i];
                    }

                    while (pos < str.size()) {
                        auto temp = pos + 2;
                        size_t found = str.find('-');

                        if (found != std::string::npos) {
                            q.symbol = '-';
                        }

                        found = str.find('>');

                        if (found != std::string::npos) {
                            q.symbol = '>';
                        }

                        pos = str.find(q.symbol, pos + 1);

                        if (pos == std::string::npos) {
                            q.stops.push_back({str.begin() + temp, str.end()});
                            break;
                        }

                        q.stops.push_back({str.begin() + temp, str.begin() + pos - 1});
                    }
                }
                    break;
            }
            return is;
        }
    }//namespace query
}//namespace transport_catalogue
