#include "stat_reader.h"

#include <iomanip>
#include <iostream>

using namespace std;

namespace transport_catalogue {
    namespace output {
        void OutputRouteAbout(string_view number, size_t number_of_stops, size_t unique_stops, uint64_t real_distance, double curvature) {
            if (number_of_stops == 0 && unique_stops == 0) {
                cout << "Bus "s << number << ": not found"s << endl;
            }
            else {
                cout << "Bus "s << number << ": "s << number_of_stops << " stops on route, "s << unique_stops << " unique stops, "s << std::setprecision(6) << real_distance << " route length, "s << std::setprecision(6) << curvature << " curvature"s << endl;
            }
        }

        void OutputStopAbout(string_view name, set<string_view> buses, bool flag) {
            if (flag) {
                if (buses.size() == 0) {
                    cout << "Stop "s << name << ": no buses"s << endl;
                }
                else {
                    cout << "Stop "s << name << ": buses "s;
                    for (auto it = buses.begin(); it != buses.end(); ++it) {
                        if (next(it) != buses.end()) {
                            cout << *it << " "s;
                        }
                        else {
                            cout << *it;
                        }
                    }
                    cout << endl;
                }
            }
            else {
                cout << "Stop "s << name << ": not found"s << endl;
            }
        }
    }//namespace output
}//namespace transport_catalogue
