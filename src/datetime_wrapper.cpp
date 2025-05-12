/**
 * @file datetime_fortran.cpp
 * @brief C++ wrapper for Fortran interoperability with the datetime library
 * 
 * This file implements C-style functions with extern "C" linkage for calling
 * the C++ datetime library functionality from Fortran. Instead of using opaque
 * pointers, we use int64_t values to represent both TimeSpan and DateTime objects.
 */

#include "datetime.hpp"
#include <string>
#include <cstring>
#include <optional>

extern "C" {

//=============================================================================
// TimeSpan functions
//=============================================================================

/**
 * @brief Create a TimeSpan from days, hours, minutes, seconds, and milliseconds
 * 
 * @param days Number of days
 * @param hours Number of hours
 * @param minutes Number of minutes
 * @param seconds Number of seconds
 * @param milliseconds Number of milliseconds
 * @return int64_t Total milliseconds representation of the TimeSpan
 */
int64_t f_timespan_create(int days, int hours, int minutes, int seconds, int milliseconds) {
    TimeSpan ts(days, hours, minutes, seconds, milliseconds);
    return ts.totalMilliseconds();
}

/**
 * @brief Create a TimeSpan from a number of days
 * 
 * @param days Number of days
 * @return int64_t Total milliseconds representation of the TimeSpan
 */
int64_t f_timespan_from_days(int days) {
    return TimeSpan::fromDays(days).totalMilliseconds();
}

/**
 * @brief Create a TimeSpan from a number of hours
 * 
 * @param hours Number of hours
 * @return int64_t Total milliseconds representation of the TimeSpan
 */
int64_t f_timespan_from_hours(int hours) {
    return TimeSpan::fromHours(hours).totalMilliseconds();
}

/**
 * @brief Create a TimeSpan from a number of minutes
 * 
 * @param minutes Number of minutes
 * @return int64_t Total milliseconds representation of the TimeSpan
 */
int64_t f_timespan_from_minutes(int minutes) {
    return TimeSpan::fromMinutes(minutes).totalMilliseconds();
}

/**
 * @brief Create a TimeSpan from a number of seconds
 * 
 * @param seconds Number of seconds
 * @return int64_t Total milliseconds representation of the TimeSpan
 */
int64_t f_timespan_from_seconds(int seconds) {
    return TimeSpan::fromSeconds(seconds).totalMilliseconds();
}

/**
 * @brief Create a TimeSpan from a number of milliseconds
 * 
 * @param milliseconds Number of milliseconds
 * @return int64_t Total milliseconds representation of the TimeSpan
 */
int64_t f_timespan_from_milliseconds(int milliseconds) {
    return TimeSpan::fromMilliseconds(milliseconds).totalMilliseconds();
}

/**
 * @brief Get the days component from a TimeSpan
 * 
 * @param ts_ms TimeSpan as milliseconds
 * @return int Days component
 */
int f_timespan_get_days(int64_t ts_ms) {
    TimeSpan ts(TimeSpan::to_components(ts_ms));
    return ts.days();
}

/**
 * @brief Get the hours component from a TimeSpan
 * 
 * @param ts_ms TimeSpan as milliseconds
 * @return int Hours component
 */
int f_timespan_get_hours(int64_t ts_ms) {
    TimeSpan ts(TimeSpan::to_components(ts_ms));
    return ts.hours();
}

/**
 * @brief Get the minutes component from a TimeSpan
 * 
 * @param ts_ms TimeSpan as milliseconds
 * @return int Minutes component
 */
int f_timespan_get_minutes(int64_t ts_ms) {
    TimeSpan ts(TimeSpan::to_components(ts_ms));
    return ts.minutes();
}

/**
 * @brief Get the seconds component from a TimeSpan
 * 
 * @param ts_ms TimeSpan as milliseconds
 * @return int Seconds component
 */
int f_timespan_get_seconds(int64_t ts_ms) {
    TimeSpan ts(TimeSpan::to_components(ts_ms));
    return ts.seconds();
}

/**
 * @brief Get the milliseconds component from a TimeSpan
 * 
 * @param ts_ms TimeSpan as milliseconds
 * @return int Milliseconds component
 */
int f_timespan_get_milliseconds(int64_t ts_ms) {
    TimeSpan ts(TimeSpan::to_components(ts_ms));
    return ts.milliseconds();
}

/**
 * @brief Get the total days representation of a TimeSpan
 * 
 * @param ts_ms TimeSpan as milliseconds
 * @return int64_t Total days
 */
int64_t f_timespan_get_total_days(int64_t ts_ms) {
    TimeSpan ts(TimeSpan::to_components(ts_ms));
    return ts.totalDays();
}

/**
 * @brief Get the total hours representation of a TimeSpan
 * 
 * @param ts_ms TimeSpan as milliseconds
 * @return int64_t Total hours
 */
int64_t f_timespan_get_total_hours(int64_t ts_ms) {
    TimeSpan ts(TimeSpan::to_components(ts_ms));
    return ts.totalHours();
}

/**
 * @brief Get the total minutes representation of a TimeSpan
 * 
 * @param ts_ms TimeSpan as milliseconds
 * @return int64_t Total minutes
 */
int64_t f_timespan_get_total_minutes(int64_t ts_ms) {
    TimeSpan ts(TimeSpan::to_components(ts_ms));
    return ts.totalMinutes();
}

/**
 * @brief Get the total seconds representation of a TimeSpan
 * 
 * @param ts_ms TimeSpan as milliseconds
 * @return int64_t Total seconds
 */
int64_t f_timespan_get_total_seconds(int64_t ts_ms) {
    TimeSpan ts(TimeSpan::to_components(ts_ms));
    return ts.totalSeconds();
}

/**
 * @brief Add two TimeSpans together
 * 
 * @param ts1_ms First TimeSpan as milliseconds
 * @param ts2_ms Second TimeSpan as milliseconds
 * @return int64_t Resulting TimeSpan as milliseconds
 */
int64_t f_timespan_add(int64_t ts1_ms, int64_t ts2_ms) {
    return ts1_ms + ts2_ms;
}

/**
 * @brief Subtract one TimeSpan from another
 * 
 * @param ts1_ms First TimeSpan as milliseconds
 * @param ts2_ms Second TimeSpan as milliseconds
 * @return int64_t Resulting TimeSpan as milliseconds
 */
int64_t f_timespan_subtract(int64_t ts1_ms, int64_t ts2_ms) {
    return ts1_ms - ts2_ms;
}

/**
 * @brief Multiply a TimeSpan by a factor
 * 
 * @param ts_ms TimeSpan as milliseconds
 * @param factor Multiplication factor
 * @return int64_t Resulting TimeSpan as milliseconds
 */
int64_t f_timespan_multiply(int64_t ts_ms, int factor) {
    return ts_ms * factor;
}

/**
 * @brief Divide a TimeSpan by a divisor
 * 
 * @param ts_ms TimeSpan as milliseconds
 * @param divisor Division factor
 * @return int64_t Resulting TimeSpan as milliseconds
 */
int64_t f_timespan_divide(int64_t ts_ms, int divisor) {
    return ts_ms / divisor;
}

/**
 * @brief Convert a TimeSpan to a string representation
 * 
 * @param ts_ms TimeSpan as milliseconds
 * @param buffer Output buffer for the string
 * @param buffer_size Size of the output buffer
 */
void f_timespan_to_string(int64_t ts_ms, char* buffer, int buffer_size) {
    TimeSpan ts(TimeSpan::to_components(ts_ms));
    std::string str = ts.toString();
    strncpy(buffer, str.c_str(), buffer_size - 1);
    buffer[buffer_size - 1] = '\0';
}

/**
 * @brief Compare two TimeSpans for equality
 * 
 * @param ts1_ms First TimeSpan as milliseconds
 * @param ts2_ms Second TimeSpan as milliseconds
 * @return int 1 if equal, 0 if not
 */
int f_timespan_equals(int64_t ts1_ms, int64_t ts2_ms) {
    return ts1_ms == ts2_ms ? 1 : 0;
}

/**
 * @brief Check if one TimeSpan is less than another
 * 
 * @param ts1_ms First TimeSpan as milliseconds
 * @param ts2_ms Second TimeSpan as milliseconds
 * @return int 1 if ts1 < ts2, 0 otherwise
 */
int f_timespan_less_than(int64_t ts1_ms, int64_t ts2_ms) {
    return ts1_ms < ts2_ms ? 1 : 0;
}

/**
 * @brief Check if one TimeSpan is greater than another
 * 
 * @param ts1_ms First TimeSpan as milliseconds
 * @param ts2_ms Second TimeSpan as milliseconds
 * @return int 1 if ts1 > ts2, 0 otherwise
 */
int f_timespan_greater_than(int64_t ts1_ms, int64_t ts2_ms) {
    return ts1_ms > ts2_ms ? 1 : 0;
}

/**
 * @brief Check if one TimeSpan is less than or equal to another
 * 
 * @param ts1_ms First TimeSpan as milliseconds
 * @param ts2_ms Second TimeSpan as milliseconds
 * @return int 1 if ts1 <= ts2, 0 otherwise
 */
int f_timespan_less_equal(int64_t ts1_ms, int64_t ts2_ms) {
    return ts1_ms <= ts2_ms ? 1 : 0;
}

/**
 * @brief Check if one TimeSpan is greater than or equal to another
 * 
 * @param ts1_ms First TimeSpan as milliseconds
 * @param ts2_ms Second TimeSpan as milliseconds
 * @return int 1 if ts1 >= ts2, 0 otherwise
 */
int f_timespan_greater_equal(int64_t ts1_ms, int64_t ts2_ms) {
    return ts1_ms >= ts2_ms ? 1 : 0;
}

//=============================================================================
// DateTime functions
//=============================================================================

/**
 * @brief Create a DateTime from year, month, day, etc.
 * 
 * @param year Year
 * @param month Month (1-12)
 * @param day Day (1-31)
 * @param hour Hour (0-23)
 * @param minute Minute (0-59)
 * @param second Second (0-59)
 * @param millisecond Millisecond (0-999)
 * @return int64_t DateTime as milliseconds since epoch
 */
int64_t f_datetime_create(int year, int month, int day, int hour, int minute, int second, int millisecond) {
    DateTime dt(year, month, day, hour, minute, second, millisecond);
    return dt.timestamp();
}

/**
 * @brief Get the current DateTime
 * 
 * @return int64_t Current DateTime as milliseconds since epoch
 */
int64_t f_datetime_now() {
    return DateTime::now().timestamp();
}

/**
 * @brief Parse a DateTime from a string
 * 
 * @param str String representation of a DateTime
 * @param format Format string (similar to strftime)
 * @param str_len Length of the string
 * @param format_len Length of the format string
 * @return int64_t DateTime as milliseconds since epoch
 */
int64_t f_datetime_parse(const char* str, const char* format, int str_len, int format_len) {
    // Create null-terminated strings from Fortran character arrays
    std::string str_cpp(str, str_len);
    std::string format_cpp(format, format_len);
    
    auto dt = DateTime::parse(str_cpp, format_cpp);
    if(dt.has_value()){
        return dt->timestamp();
    } else {
        // Handle parsing error (return 0 or some error code)
        return DateTime::INVALID_TIMESTAMP;
    }
}

/**
 * @brief Get the year component from a DateTime
 * 
 * @param dt_ms DateTime as milliseconds since epoch
 * @return int Year component
 */
int f_datetime_get_year(int64_t dt_ms) {
    DateTime dt(dt_ms);
    return dt.year();
}

/**
 * @brief Get the month component from a DateTime
 * 
 * @param dt_ms DateTime as milliseconds since epoch
 * @return int Month component (1-12)
 */
int f_datetime_get_month(int64_t dt_ms) {
    DateTime dt(dt_ms);
    return dt.month();
}

/**
 * @brief Get the day component from a DateTime
 * 
 * @param dt_ms DateTime as milliseconds since epoch
 * @return int Day component (1-31)
 */
int f_datetime_get_day(int64_t dt_ms) {
    DateTime dt(dt_ms);
    return dt.day();
}

/**
 * @brief Get the hour component from a DateTime
 * 
 * @param dt_ms DateTime as milliseconds since epoch
 * @return int Hour component (0-23)
 */
int f_datetime_get_hour(int64_t dt_ms) {
    DateTime dt(dt_ms);
    return dt.hour();
}

/**
 * @brief Get the minute component from a DateTime
 * 
 * @param dt_ms DateTime as milliseconds since epoch
 * @return int Minute component (0-59)
 */
int f_datetime_get_minute(int64_t dt_ms) {
    DateTime dt(dt_ms);
    return dt.minute();
}

/**
 * @brief Get the second component from a DateTime
 * 
 * @param dt_ms DateTime as milliseconds since epoch
 * @return int Second component (0-59)
 */
int f_datetime_get_second(int64_t dt_ms) {
    DateTime dt(dt_ms);
    return dt.second();
}

/**
 * @brief Get the millisecond component from a DateTime
 * 
 * @param dt_ms DateTime as milliseconds since epoch
 * @return int Millisecond component (0-999)
 */
int f_datetime_get_millisecond(int64_t dt_ms) {
    DateTime dt(dt_ms);
    return dt.millisecond();
}

/**
 * @brief Add a TimeSpan to a DateTime
 * 
 * @param dt_ms DateTime as milliseconds since epoch
 * @param ts_ms TimeSpan as milliseconds
 * @return int64_t Resulting DateTime as milliseconds since epoch
 */
int64_t f_datetime_add_timespan(int64_t dt_ms, int64_t ts_ms) {
    DateTime dt(dt_ms);
    TimeSpan ts(TimeSpan::to_components(ts_ms));
    return (dt + ts).timestamp();
}

/**
 * @brief Subtract a TimeSpan from a DateTime
 * 
 * @param dt_ms DateTime as milliseconds since epoch
 * @param ts_ms TimeSpan as milliseconds
 * @return int64_t Resulting DateTime as milliseconds since epoch
 */
int64_t f_datetime_subtract_timespan(int64_t dt_ms, int64_t ts_ms) {
    DateTime dt(dt_ms);
    TimeSpan ts(TimeSpan::to_components(ts_ms));
    return (dt - ts).timestamp();
}

/**
 * @brief Calculate the difference between two DateTimes
 * 
 * @param dt1_ms First DateTime as milliseconds since epoch
 * @param dt2_ms Second DateTime as milliseconds since epoch
 * @return int64_t Resulting TimeSpan as milliseconds
 */
int64_t f_datetime_difference(int64_t dt1_ms, int64_t dt2_ms) {
    DateTime dt1(dt1_ms);
    DateTime dt2(dt2_ms);
    return (dt1 - dt2).totalMilliseconds();
}

/**
 * @brief Format a DateTime to a string
 * 
 * @param dt_ms DateTime as milliseconds since epoch
 * @param format Format string (similar to strftime)
 * @param buffer Output buffer for the string
 * @param format_len Length of the format string
 * @param buffer_size Size of the output buffer
 */
void f_datetime_format(int64_t dt_ms, const char* format, char* buffer, int format_len, int buffer_size) {
    DateTime dt(dt_ms);
    std::string format_cpp(format, format_len);
    std::string str = dt.format(format_cpp);
    strncpy(buffer, str.c_str(), buffer_size - 1);
    buffer[buffer_size - 1] = '\0';
}

/**
 * @brief Format a DateTime to a string with milliseconds
 * 
 * @param dt_ms DateTime as milliseconds since epoch
 * @param format Format string (similar to strftime)
 * @param buffer Output buffer for the string
 * @param format_len Length of the format string
 * @param buffer_size Size of the output buffer
 */
void f_datetime_format_milliseconds(int64_t dt_ms, const char* format, char* buffer, int format_len, int buffer_size) {
    DateTime dt(dt_ms);
    std::string format_cpp(format, format_len);
    std::string str = dt.format_w_milliseconds(format_cpp);
    strncpy(buffer, str.c_str(), buffer_size - 1);
    buffer[buffer_size - 1] = '\0';
}


/**
 * @brief Convert a DateTime to ISO string format
 * 
 * @param dt_ms DateTime as milliseconds since epoch
 * @param buffer Output buffer for the string
 * @param buffer_size Size of the output buffer
 */
void f_datetime_to_iso_string(int64_t dt_ms, char* buffer, int buffer_size) {
    DateTime dt(dt_ms);
    std::string str = dt.toISOString();
    strncpy(buffer, str.c_str(), buffer_size - 1);
    buffer[buffer_size - 1] = '\0';
}

/**
 * @brief Compare two DateTimes for equality
 * 
 * @param dt1_ms First DateTime as milliseconds since epoch
 * @param dt2_ms Second DateTime as milliseconds since epoch
 * @return int 1 if equal, 0 if not
 */
int f_datetime_equals(int64_t dt1_ms, int64_t dt2_ms) {
    return dt1_ms == dt2_ms ? 1 : 0;
}

/**
 * @brief Check if one DateTime is less than another
 * 
 * @param dt1_ms First DateTime as milliseconds since epoch
 * @param dt2_ms Second DateTime as milliseconds since epoch
 * @return int 1 if dt1 < dt2, 0 otherwise
 */
int f_datetime_less_than(int64_t dt1_ms, int64_t dt2_ms) {
    return dt1_ms < dt2_ms ? 1 : 0;
}

/**
 * @brief Check if one DateTime is greater than another
 * 
 * @param dt1_ms First DateTime as milliseconds since epoch
 * @param dt2_ms Second DateTime as milliseconds since epoch
 * @return int 1 if dt1 > dt2, 0 otherwise
 */
int f_datetime_greater_than(int64_t dt1_ms, int64_t dt2_ms) {
    return dt1_ms > dt2_ms ? 1 : 0;
}

/**
 * @brief Check if one DateTime is less than or equal to another
 * 
 * @param dt1_ms First DateTime as milliseconds since epoch
 * @param dt2_ms Second DateTime as milliseconds since epoch
 * @return int 1 if dt1 <= dt2, 0 otherwise
 */
int f_datetime_less_equal(int64_t dt1_ms, int64_t dt2_ms) {
    return dt1_ms <= dt2_ms ? 1 : 0;
}

/**
 * @brief Check if one DateTime is greater than or equal to another
 * 
 * @param dt1_ms First DateTime as milliseconds since epoch
 * @param dt2_ms Second DateTime as milliseconds since epoch
 * @return int 1 if dt1 >= dt2, 0 otherwise
 */
int f_datetime_greater_equal(int64_t dt1_ms, int64_t dt2_ms) {
    return dt1_ms >= dt2_ms ? 1 : 0;
}

} // extern "C"
