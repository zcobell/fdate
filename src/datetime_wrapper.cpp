/**
 * @file datetime_fortran.cpp
 * @brief C++ wrapper for Fortran interoperability with the datetime library
 *
 * This file implements C-style functions with extern "C" linkage for calling
 * the C++ datetime library functionality from Fortran. Instead of using opaque
 * pointers, we use int64_t values to represent both TimeSpan and DateTime
 * objects.
 */

#include <cstdint>
#include <cstring>
#include <optional>
#include <string>

#include "datetime.hpp"

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
auto f_timespan_create(int days, int hours, int minutes, int seconds,
                       int milliseconds) -> int64_t {
  const TimeSpan time_span(days, hours, minutes, seconds, milliseconds);
  return time_span.totalMilliseconds();
}

/**
 * @brief Create a TimeSpan from a number of days
 *
 * @param days Number of days
 * @return int64_t Total milliseconds representation of the TimeSpan
 */
auto f_timespan_from_days(const int64_t days) -> int64_t {
  return TimeSpan::fromDays(days).totalMilliseconds();
}

/**
 * @brief Create a TimeSpan from a number of hours
 *
 * @param hours Number of hours
 * @return int64_t Total milliseconds representation of the TimeSpan
 */
auto f_timespan_from_hours(const int64_t hours) -> int64_t {
  return TimeSpan::fromHours(hours).totalMilliseconds();
}

/**
 * @brief Create a TimeSpan from a number of minutes
 *
 * @param minutes Number of minutes
 * @return int64_t Total milliseconds representation of the TimeSpan
 */
auto f_timespan_from_minutes(const int64_t minutes) -> int64_t {
  return TimeSpan::fromMinutes(minutes).totalMilliseconds();
}

/**
 * @brief Create a TimeSpan from a number of seconds
 *
 * @param seconds Number of seconds
 * @return int64_t Total milliseconds representation of the TimeSpan
 */
auto f_timespan_from_seconds(const int64_t seconds) -> int64_t {
  return TimeSpan::fromSeconds(seconds).totalMilliseconds();
}

/**
 * @brief Create a TimeSpan from a number of milliseconds
 *
 * @param milliseconds Number of milliseconds
 * @return int64_t Total milliseconds representation of the TimeSpan
 */
auto f_timespan_from_milliseconds(const int64_t milliseconds) -> int64_t {
  return TimeSpan::fromMilliseconds(milliseconds).totalMilliseconds();
}

/**
 * @brief Get the days component from a TimeSpan
 *
 * @param ts_ms TimeSpan as milliseconds
 * @return int Days component
 */
auto f_timespan_get_days(const int64_t ts_ms) -> int64_t {
  return TimeSpan::to_components(ts_ms).days;
}

/**
 * @brief Get the hours component from a TimeSpan
 *
 * @param ts_ms TimeSpan as milliseconds
 * @return int Hours component
 */
auto f_timespan_get_hours(const int64_t ts_ms) -> int64_t {
  return TimeSpan::to_components(ts_ms).hours;
  ;
}

/**
 * @brief Get the minute component from a TimeSpan
 *
 * @param ts_ms TimeSpan as milliseconds
 * @return int Minutes component
 */
auto f_timespan_get_minutes(const int64_t ts_ms) -> int64_t {
  return TimeSpan::to_components(ts_ms).minutes;
}

/**
 * @brief Get the second component from a TimeSpan
 *
 * @param ts_ms TimeSpan as milliseconds
 * @return int Seconds component
 */
auto f_timespan_get_seconds(const int64_t ts_ms) -> int64_t {
  return TimeSpan::to_components(ts_ms).seconds;
}

/**
 * @brief Get the millisecond component from a TimeSpan
 *
 * @param ts_ms TimeSpan as milliseconds
 * @return int Milliseconds component
 */
auto f_timespan_get_milliseconds(const int64_t ts_ms) -> int64_t {
  return TimeSpan::to_components(ts_ms).milliseconds;
}

/**
 * @brief Get the total days representation of a TimeSpan
 *
 * @param ts_ms TimeSpan as milliseconds
 * @return int64_t Total days
 */
auto f_timespan_get_total_days(const int64_t ts_ms) -> int64_t {
  const TimeSpan time_span(TimeSpan::to_components(ts_ms));
  return time_span.totalDays();
}

/**
 * @brief Get the total hours representation of a TimeSpan
 *
 * @param ts_ms TimeSpan as milliseconds
 * @return int64_t Total hours
 */
auto f_timespan_get_total_hours(const int64_t ts_ms) -> int64_t {
  const TimeSpan time_span(TimeSpan::to_components(ts_ms));
  return time_span.totalHours();
}

/**
 * @brief Get the total minutes representation of a TimeSpan
 *
 * @param ts_ms TimeSpan as milliseconds
 * @return int64_t Total minutes
 */
auto f_timespan_get_total_minutes(const int64_t ts_ms) -> int64_t {
  const TimeSpan time_span(TimeSpan::to_components(ts_ms));
  return time_span.totalMinutes();
}

/**
 * @brief Get the total seconds representation of a TimeSpan
 *
 * @param ts_ms TimeSpan as milliseconds
 * @return int64_t Total seconds
 */
auto f_timespan_get_total_seconds(const int64_t ts_ms) -> int64_t {
  const TimeSpan time_span(TimeSpan::to_components(ts_ms));
  return time_span.totalSeconds();
}

/**
 * @brief Add two TimeSpans together
 *
 * @param ts1_ms First TimeSpan as milliseconds
 * @param ts2_ms Second TimeSpan as milliseconds
 * @return int64_t Resulting TimeSpan as milliseconds
 */
auto f_timespan_add(const int64_t ts1_ms, const int64_t ts2_ms) -> int64_t {
  return ts1_ms + ts2_ms;
}

/**
 * @brief Subtract one TimeSpan from another
 *
 * @param ts1_ms First TimeSpan as milliseconds
 * @param ts2_ms Second TimeSpan as milliseconds
 * @return int64_t Resulting TimeSpan as milliseconds
 */
auto f_timespan_subtract(const int64_t ts1_ms, const int64_t ts2_ms)
    -> int64_t {
  return ts1_ms - ts2_ms;
}

/**
 * @brief Multiply a TimeSpan by a factor
 *
 * @param ts_ms TimeSpan as milliseconds
 * @param factor Multiplication factor
 * @return int64_t Resulting TimeSpan as milliseconds
 */
auto f_timespan_multiply(const int64_t ts_ms, const int factor) -> int64_t {
  return ts_ms * factor;
}

/**
 * @brief Divide a TimeSpan by a divisor
 *
 * @param ts_ms TimeSpan as milliseconds
 * @param divisor Division factor
 * @return int64_t Resulting TimeSpan as milliseconds
 */
auto f_timespan_divide(const int64_t ts_ms, const int64_t divisor) -> int64_t {
  return ts_ms / divisor;
}

/**
 * @brief Convert a TimeSpan to a string representation
 *
 * @param ts_ms TimeSpan as milliseconds
 * @param buffer Output buffer for the string
 * @param buffer_size Size of the output buffer
 */
void f_timespan_to_string(const int64_t ts_ms, char* buffer,
                          const int buffer_size) {
  const TimeSpan time_span(TimeSpan::to_components(ts_ms));
  const std::string str = time_span.toString();
  const size_t buffer_size_t = static_cast<size_t>(buffer_size - 1);
  strncpy(buffer, str.c_str(), buffer_size_t);
  buffer[buffer_size - 1] = '\0';
}

/**
 * @brief Compare two TimeSpans for equality
 *
 * @param ts1_ms First TimeSpan as milliseconds
 * @param ts2_ms Second TimeSpan as milliseconds
 * @return true if equal, false otherwise
 */
auto f_timespan_equals(const int64_t ts1_ms, const int64_t ts2_ms) -> bool {
  return ts1_ms == ts2_ms;
}

/**
 * @brief Check if one TimeSpan is less than another
 *
 * @param ts1_ms First TimeSpan as milliseconds
 * @param ts2_ms Second TimeSpan as milliseconds
 * @return true if ts1 < ts2, false otherwise
 */
auto f_timespan_less_than(const int64_t ts1_ms, const int64_t ts2_ms) -> bool {
  return ts1_ms < ts2_ms;
}

/**
 * @brief Check if one TimeSpan is greater than another
 *
 * @param ts1_ms First TimeSpan as milliseconds
 * @param ts2_ms Second TimeSpan as milliseconds
 * @return true if ts1 > ts2, false otherwise
 */
auto f_timespan_greater_than(const int64_t ts1_ms, const int64_t ts2_ms)
    -> bool {
  return ts1_ms > ts2_ms;
}

/**
 * @brief Check if one TimeSpan is less than or equal to another
 *
 * @param ts1_ms First TimeSpan as milliseconds
 * @param ts2_ms Second TimeSpan as milliseconds
 * @return int 1 if ts1 <= ts2, 0 otherwise
 */
auto f_timespan_less_equal(const int64_t ts1_ms, const int64_t ts2_ms) -> int {
  return ts1_ms <= ts2_ms ? 1 : 0;
}

/**
 * @brief Check if one TimeSpan is greater than or equal to another
 *
 * @param ts1_ms First TimeSpan as milliseconds
 * @param ts2_ms Second TimeSpan as milliseconds
 * @return int 1 if ts1 >= ts2, 0 otherwise
 */
auto f_timespan_greater_equal(const int64_t ts1_ms, const int64_t ts2_ms) -> int {
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
auto f_datetime_create(const int year, const int month, const int day,
                       const int hour, const int minute, const int second,
                       const int millisecond) -> int64_t {
  // Check that all the ints coming from fortran are valid (>0), then cast
  // them to unsigned
  if (year < 0 || month < 0 || day < 0 || hour < 0 || minute < 0 ||
      second < 0 || millisecond < 0) {
    return DateTime::INVALID_TIMESTAMP;
  } else {
    const auto u_month = static_cast<unsigned>(month);
    const auto u_day = static_cast<unsigned>(day);
    const auto u_hour = static_cast<unsigned>(hour);
    const auto u_minute = static_cast<unsigned>(minute);
    const auto u_second = static_cast<unsigned>(second);
    const auto u_millisecond = static_cast<unsigned>(millisecond);
    const DateTime date(year, u_month, u_day, u_hour, u_minute, u_second,
                        u_millisecond);
    return date.timestamp();
  }
}

/**
 * @brief Get the current DateTime
 *
 * @return int64_t Current DateTime as milliseconds since epoch
 */
auto f_datetime_now() -> int64_t { return DateTime::now().timestamp(); }

/**
 * @brief Parse a DateTime from a string
 *
 * @param str String representation of a DateTime
 * @param format Format string (similar to strftime)
 * @param str_len Length of the string
 * @param format_len Length of the format string
 * @return int64_t DateTime as milliseconds since epoch
 */
auto f_datetime_parse(const char* str, const char* format, const int str_len,
                      const int format_len) -> int64_t {
  // Cast the lengths coming from fortran over to size_t
  if (str_len <= 0) {
    return DateTime::INVALID_TIMESTAMP;
  }

  if (format_len <= 0) {
    return DateTime::INVALID_TIMESTAMP;
  }

  const auto str_len_t = static_cast<size_t>(str_len);
  const auto format_len_t = static_cast<size_t>(format_len);

  // Create null-terminated strings from Fortran character arrays
  const std::string str_cpp(str, str_len_t);
  const std::string format_cpp(format, format_len_t);

  const auto date_time = DateTime::parse(str_cpp, format_cpp);
  if (date_time.has_value()) {
    return date_time->timestamp();
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
auto f_datetime_get_year(const int64_t dt_ms) -> int64_t {
  const DateTime date(dt_ms);
  return date.year();
}

/**
 * @brief Get the month component from a DateTime
 *
 * @param dt_ms DateTime as milliseconds since epoch
 * @return int Month component (1-12)
 */
auto f_datetime_get_month(const int64_t dt_ms) -> int64_t {
  const DateTime date(dt_ms);
  return date.month();
}

/**
 * @brief Get the day component from a DateTime
 *
 * @param dt_ms DateTime as milliseconds since epoch
 * @return int Day component (1-31)
 */
auto f_datetime_get_day(const int64_t dt_ms) -> int64_t {
  const DateTime date(dt_ms);
  return date.day();
}

/**
 * @brief Get the hour component from a DateTime
 *
 * @param dt_ms DateTime as milliseconds since epoch
 * @return int Hour component (0-23)
 */
auto f_datetime_get_hour(const int64_t dt_ms) -> int64_t {
  const DateTime date(dt_ms);
  return date.hour();
}

/**
 * @brief Get the minute component from a DateTime
 *
 * @param dt_ms DateTime as milliseconds since epoch
 * @return int Minute component (0-59)
 */
auto f_datetime_get_minute(const int64_t dt_ms) -> int64_t {
  const DateTime date(dt_ms);
  return date.minute();
}

/**
 * @brief Get the second component from a DateTime
 *
 * @param dt_ms DateTime as milliseconds since epoch
 * @return int Second component (0-59)
 */
auto f_datetime_get_second(const int64_t dt_ms) -> int64_t {
  const DateTime date(dt_ms);
  return date.second();
}

/**
 * @brief Get the millisecond component from a DateTime
 *
 * @param dt_ms DateTime as milliseconds since epoch
 * @return int Millisecond component (0-999)
 */
auto f_datetime_get_millisecond(const int64_t dt_ms) -> int64_t {
  const DateTime date(dt_ms);
  return date.millisecond();
}

/**
 * @brief Add a TimeSpan to a DateTime
 *
 * @param dt_ms DateTime as milliseconds since epoch
 * @param ts_ms TimeSpan as milliseconds
 * @return int64_t Resulting DateTime as milliseconds since epoch
 */
auto f_datetime_add_timespan(const int64_t dt_ms, const int64_t ts_ms)
    -> int64_t {
  const DateTime date(dt_ms);
  const TimeSpan time_span(TimeSpan::to_components(ts_ms));
  return (date + time_span).timestamp();
}

/**
 * @brief Subtract a TimeSpan from a DateTime
 *
 * @param dt_ms DateTime as milliseconds since epoch
 * @param ts_ms TimeSpan as milliseconds
 * @return int64_t Resulting DateTime as milliseconds since epoch
 */
auto f_datetime_subtract_timespan(const int64_t dt_ms, const int64_t ts_ms)
    -> int64_t {
  const DateTime date(dt_ms);
  const TimeSpan time_span(TimeSpan::to_components(ts_ms));
  return (date - time_span).timestamp();
}

/**
 * @brief Calculate the difference between two DateTimes
 *
 * @param dt1_ms First DateTime as milliseconds since epoch
 * @param dt2_ms Second DateTime as milliseconds since epoch
 * @return int64_t Resulting TimeSpan as milliseconds
 */
auto f_datetime_difference(const int64_t dt1_ms, const int64_t dt2_ms)
    -> int64_t {
  const DateTime dt1(dt1_ms);
  const DateTime dt2(dt2_ms);
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
void f_datetime_format(const int64_t dt_ms, const char* format, char* buffer,
                       const int format_len, const int buffer_size) {
  if (format_len <= 0 || buffer_size <= 0) {
    std::cerr << "Invalid format or buffer size\n";
    return;
  }

  const auto format_len_t = static_cast<size_t>(format_len);
  const auto buffer_size_t = static_cast<size_t>(buffer_size);

  const DateTime date(dt_ms);
  const std::string format_cpp(format, format_len_t);
  const std::string str = date.format(format_cpp);
  strncpy(buffer, str.c_str(), buffer_size_t - 1);
  buffer[buffer_size_t - 1] = '\0';
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
void f_datetime_format_milliseconds(const int64_t dt_ms, const char* format,
                                    char* buffer, const int format_len,
                                    const int buffer_size) {
  if (format_len <= 0 || buffer_size <= 0) {
    std::cerr << "Invalid format or buffer size\n";
    return;
  }

  const auto format_len_t = static_cast<size_t>(format_len);
  const auto buffer_size_t = static_cast<size_t>(buffer_size);

  const DateTime date(dt_ms);
  const std::string format_cpp(format, format_len_t);
  const std::string str = date.format_w_milliseconds(format_cpp);
  strncpy(buffer, str.c_str(), buffer_size_t - 1);
  buffer[buffer_size_t - 1] = '\0';
}

/**
 * @brief Convert a DateTime to ISO string format
 *
 * @param dt_ms DateTime as milliseconds since epoch
 * @param buffer Output buffer for the string
 * @param buffer_size Size of the output buffer
 */
void f_datetime_to_iso_string(const int64_t dt_ms, char* buffer,
                              const int buffer_size) {

  if (buffer_size <= 0) {
    std::cerr << "Invalid buffer size\n";
    return;
  }

  const auto buffer_size_t = static_cast<size_t>(buffer_size);

  const DateTime date(dt_ms);
  const std::string str = date.toISOString();
  strncpy(buffer, str.c_str(), buffer_size_t - 1);
  buffer[buffer_size_t - 1] = '\0';
}

/**
 * @brief Compare two DateTimes for equality
 *
 * @param dt1_ms First DateTime as milliseconds since epoch
 * @param dt2_ms Second DateTime as milliseconds since epoch
 * @return true if equal, false otherwise
 */
auto f_datetime_equals(const int64_t dt1_ms, const int64_t dt2_ms) -> bool {
  return dt1_ms == dt2_ms;
}

/**
 * @brief Check if one DateTime is less than another
 *
 * @param dt1_ms First DateTime as milliseconds since epoch
 * @param dt2_ms Second DateTime as milliseconds since epoch
 * @return true if dt1 < dt2, false otherwise
 */
auto f_datetime_less_than(const int64_t dt1_ms, const int64_t dt2_ms) -> bool {
  return dt1_ms < dt2_ms;
}

/**
 * @brief Check if one DateTime is greater than another
 *
 * @param dt1_ms First DateTime as milliseconds since epoch
 * @param dt2_ms Second DateTime as milliseconds since epoch
 * @return true if dt1 > dt2, false otherwise
 */
auto f_datetime_greater_than(const int64_t dt1_ms, const int64_t dt2_ms)
    -> bool {
  return dt1_ms > dt2_ms;
}

/**
 * @brief Check if one DateTime is less than or equal to another
 *
 * @param dt1_ms First DateTime as milliseconds since epoch
 * @param dt2_ms Second DateTime as milliseconds since epoch
 * @return true if dt1 <= dt2, false otherwise
 */
auto f_datetime_less_equal(const int64_t dt1_ms, const int64_t dt2_ms) -> bool {
  return dt1_ms <= dt2_ms;
}

/**
 * @brief Check if one DateTime is greater than or equal to another
 *
 * @param dt1_ms First DateTime as milliseconds since epoch
 * @param dt2_ms Second DateTime as milliseconds since epoch
 * @return true if dt1 >= dt2, false otherwise
 */
auto f_datetime_greater_equal(const int64_t dt1_ms, const int64_t dt2_ms)
    -> bool {
  return dt1_ms >= dt2_ms;
}

}  // extern "C"
