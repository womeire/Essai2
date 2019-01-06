// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

#ifndef PCH_H
#define PCH_H

#include <fcntl.h>
#include "io.h"
#include <iostream> //cout, cin
#include <string> //std::string
#include <cstdio>
#include <fcntl.h>
#include <fstream> //fopen, fread, fwrite, ...
#include <random> //random_engine
#include <bitset>
#include <thread>
#include <stdio.h>
#include <vector> //std::vector
#include <queue> //std::priority_queue
#include <iomanip>      // std::put_time
#include <ctime>        // std::time_t, struct std::tm, std::localtime
#include <chrono>       // std::chrono::system_clock
#include <boost/chrono/chrono.hpp> //chrono, steady_clock, time_point, duration...
#include <boost/regex.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>
/*Must be downloaded from https://www.boost.org/users/history/version_1_68_0.html
and path to the library added in Project properties -> C/C++ -> General -> Additional Include Directories */

#ifndef NOMINMAX // otherwise windows.h "takes over" min() and max()
#define NOMINMAX
#endif
#include <windows.h> 

#endif //PCH_H
