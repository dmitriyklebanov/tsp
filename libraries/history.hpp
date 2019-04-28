#ifndef __HISTORY_HPP__
#define __HISTORY_HPP__

#ifdef MAKE_HISTORY
    #include <ostream>
    #include <iomanip>
    #include <chrono>
    #define HISTORY_STREAM std::ostream& os
    #define HISTORY_START auto hist_start_time = std::chrono::steady_clock().now();
    #define HISTORY(value) os << (std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock().now() - hist_start_time)).count() << " " << (value) << "\n";
#else
    #define HISTORY_STREAM
    #define HISTORY(value)
#endif //MAKE_HISTORY

#endif //__HISTORY_HPP__
