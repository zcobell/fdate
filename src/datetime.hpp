#pragma once

#include <chrono>
#include <compare>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <optional>
#include <limits>

#include "date_hh.h"

class TimeSpan {
 private:
  std::chrono::milliseconds duration_;

  explicit constexpr TimeSpan(std::chrono::milliseconds milliseconds) noexcept
      : duration_(milliseconds) {}

 public:
  // Wrapper for the components
  struct s_TimespanComponents {
    int days;
    int hours;
    int minutes;
    int seconds;
    int milliseconds;
  };

  // Constructors
  constexpr TimeSpan() noexcept : duration_(std::chrono::milliseconds{0}) {}

  constexpr TimeSpan(s_TimespanComponents components) noexcept
      : duration_(std::chrono::milliseconds{
            date::days(components.days) + std::chrono::hours(components.hours) +
            std::chrono::minutes(components.minutes) +
            std::chrono::seconds(components.seconds) +
            std::chrono::milliseconds(components.milliseconds)}) {}

  constexpr TimeSpan(int days, int hours, int minutes, int seconds,
                     int milliseconds) noexcept
      : duration_(std::chrono::milliseconds{
            date::days(days) + std::chrono::hours(hours) +
            std::chrono::minutes(minutes) + std::chrono::seconds(seconds) +
            std::chrono::milliseconds(milliseconds)}) {}

  constexpr TimeSpan(const TimeSpan&) noexcept = default;
  constexpr TimeSpan& operator=(const TimeSpan&) noexcept = default;

  // Static factory methods
  static constexpr TimeSpan fromDays(int days) noexcept {
    return TimeSpan(s_TimespanComponents{days, 0, 0, 0, 0});
  }

  static constexpr TimeSpan fromHours(int hours) noexcept {
    return TimeSpan(s_TimespanComponents{0, hours, 0, 0, 0});
  }

  static constexpr TimeSpan fromMinutes(int minutes) noexcept {
    return TimeSpan(s_TimespanComponents{0, 0, minutes, 0, 0});
  }

  static constexpr TimeSpan fromSeconds(int seconds) noexcept {
    return TimeSpan(s_TimespanComponents{0, 0, 0, seconds, 0});
  }

  static constexpr TimeSpan fromMilliseconds(int milliseconds) noexcept {
    return TimeSpan(s_TimespanComponents{0, 0, 0, 0, milliseconds});
  }

  constexpr auto components() const noexcept {
    return to_components(duration_.count());
  }

  static constexpr s_TimespanComponents to_components(
      int64_t duration) noexcept {
    s_TimespanComponents components{0, 0, 0, 0, 0};

    bool negative = duration < 0;
    if (negative) duration = -duration;  // Work with absolute value

    // Break the duration into its components so that their total is equal to
    // the duration
    components.milliseconds = duration % 1000;
    duration /= 1000;
    components.seconds = duration % 60;
    duration /= 60;  // FIXED: Now correctly dividing by 60 to convert seconds
                     // to minutes
    components.minutes = duration % 60;
    duration /= 60;
    components.hours = duration % 24;
    duration /= 24;
    components.days = static_cast<int>(duration);

    // Apply negative sign if needed
    if (negative) {
      components.days = -components.days;
      components.hours = -components.hours;
      components.minutes = -components.minutes;
      components.seconds = -components.seconds;
      components.milliseconds = -components.milliseconds;
    }

    return components;
  }

  // Accessors
  constexpr int days() const noexcept { return components().days; }

  constexpr int hours() const noexcept { return components().hours; }

  constexpr int minutes() const noexcept { return components().minutes; }

  constexpr int seconds() const noexcept { return components().seconds; }

  constexpr int milliseconds() const noexcept {
    return components().milliseconds;
  }

  constexpr int64_t totalDays() const noexcept {
    return duration_.count() / (24 * 60 * 60 * 1000);
  }

  constexpr int64_t totalHours() const noexcept {
    return duration_.count() / (60 * 60 * 1000);
  }

  constexpr int64_t totalMinutes() const noexcept {
    return duration_.count() / (60 * 1000);
  }

  constexpr int64_t totalSeconds() const noexcept {
    return duration_.count() / 1000;
  }

  constexpr int64_t totalMilliseconds() const noexcept {
    return duration_.count();
  }

  // Internal access
  constexpr std::chrono::milliseconds duration() const noexcept {
    return duration_;
  }

  // Operators
  constexpr TimeSpan operator+(const TimeSpan& other) const noexcept {
    return TimeSpan(to_components(duration_.count() + other.duration_.count()));
  }

  constexpr TimeSpan operator-(const TimeSpan& other) const noexcept {
    return TimeSpan(to_components(duration_.count() - other.duration_.count()));
  }

  constexpr TimeSpan operator*(int factor) const noexcept {
    return TimeSpan(to_components(duration_.count() * factor));
  }

  constexpr TimeSpan operator/(int divisor) const noexcept {
    return TimeSpan(to_components(duration_.count() / divisor));
  }

  constexpr bool operator==(const TimeSpan& other) const noexcept {
    return duration_ == other.duration_;
  }

  constexpr bool operator!=(const TimeSpan& other) const noexcept {
    return duration_ != other.duration_;
  }

  constexpr bool operator<(const TimeSpan& other) const noexcept {
    return duration_ < other.duration_;
  }

  constexpr bool operator>(const TimeSpan& other) const noexcept {
    return duration_ > other.duration_;
  }

  constexpr bool operator<=(const TimeSpan& other) const noexcept {
    return duration_ <= other.duration_;
  }

  constexpr bool operator>=(const TimeSpan& other) const noexcept {
    return duration_ >= other.duration_;
  }

  // String representation
  std::string toString() const {
    std::ostringstream oss;
    if (days() != 0) {
      oss << days() << "d ";
    }
    oss << std::setfill('0') << std::setw(2) << hours() << ":"
        << std::setfill('0') << std::setw(2) << minutes() << ":"
        << std::setfill('0') << std::setw(2) << seconds();

    if (milliseconds() != 0) {
      oss << "." << std::setfill('0') << std::setw(3) << milliseconds();
    }

    return oss.str();
  }
};

class DateTime {
 private:
  using time_point = std::chrono::time_point<std::chrono::system_clock,
                                             std::chrono::milliseconds>;
  time_point tp_;

 public:
  // Invalid Timestamp
  static constexpr auto INVALID_TIMESTAMP = -std::numeric_limits<int64_t>::max();

  // Constructors
  constexpr DateTime() noexcept
      : tp_(std::chrono::time_point<std::chrono::system_clock,
                                    std::chrono::milliseconds>{}) {}

  constexpr DateTime(int year, unsigned month, unsigned day, unsigned hour = 0,
                     unsigned minute = 0, unsigned second = 0,
                     unsigned millisecond = 0) noexcept
      : tp_(std::chrono::time_point_cast<std::chrono::milliseconds>(
            date::sys_days{date::year{year} / date::month{month} /
                           date::day{day}} +
            std::chrono::hours{hour} + std::chrono::minutes{minute} +
            std::chrono::seconds{second} +
            std::chrono::milliseconds{millisecond})) {}

  // Constructor from timestamp (milliseconds since epoch)
  explicit constexpr DateTime(int64_t timestamp) noexcept
      : tp_(time_point(std::chrono::milliseconds(timestamp))) {}

  // Constructor from time_point (to help with operator implementations)
  constexpr DateTime(const time_point& tp) noexcept : tp_(tp) {}

  constexpr DateTime(const DateTime&) noexcept = default;
  constexpr DateTime& operator=(const DateTime&) noexcept = default;
  constexpr DateTime(DateTime&&) noexcept = default;
  constexpr DateTime& operator=(DateTime&&) noexcept = default;

  // Parse from string (not constexpr due to parsing)
  static std::optional<DateTime> parse(const std::string& str,
                        const std::string& format = "%Y-%m-%d %H:%M:%S") {
    std::istringstream is(str);
    date::sys_time<std::chrono::milliseconds> tp;

    // If the user has provided a string that has milliseconds (i.e. position 4 from the end has a period)
    if (str.size() >= 4 && str[str.size() - 4] == '.') {
      // Parse using milliseconds
      date::from_stream(is, format.c_str(), tp);
    } else {
      // Parse without milliseconds
      auto tp_temp = date::sys_time<std::chrono::seconds>{};
      date::from_stream(is, format.c_str(), tp_temp);
      tp = std::chrono::time_point_cast<std::chrono::milliseconds>(tp_temp);
    }

    // Check if parsing was successful
    if (is.fail() || is.bad()) {
      return std::nullopt;  // Parsing failed
    }

    return std::make_optional<DateTime>(tp);
  }

  // Get timestamp (milliseconds since epoch)
  constexpr int64_t timestamp() const noexcept {
    return tp_.time_since_epoch().count();
  }

  std::string format(const std::string& fmt = "%Y-%m-%d %H:%M:%S") const {
    std::ostringstream oss;
    auto tp_sec = std::chrono::time_point_cast<std::chrono::seconds>(tp_);
    date::to_stream(oss, fmt.c_str(), tp_sec);
    return oss.str();
  }

  std::string format_w_milliseconds(const std::string& fmt = "%Y-%m-%d %H:%M:%S") const {
    std::ostringstream oss;
    date::to_stream(oss, fmt.c_str(), tp_);
    return oss.str();
  }

  // Convert to ISO 8601 format
  std::string toISOString() const {
    return format("%Y-%m-%dT%H:%M:%S");
  }

  std::string toISOStringMsec() const {
    return format_w_milliseconds("%Y-%m-%dT%H:%M:%S");
  }

  // Getters
  constexpr int year() const noexcept {
    return static_cast<int>(
        date::year_month_day{date::floor<date::days>(tp_)}.year());
  }

  constexpr unsigned month() const noexcept {
    return static_cast<unsigned>(
        date::year_month_day{date::floor<date::days>(tp_)}.month());
  }

  constexpr unsigned day() const noexcept {
    return static_cast<unsigned>(
        date::year_month_day{date::floor<date::days>(tp_)}.day());
  }

  constexpr unsigned hour() const noexcept {
    return static_cast<unsigned>((tp_ - date::floor<date::days>(tp_)) /
                                 std::chrono::hours(1));
  }

  constexpr unsigned minute() const noexcept {
    const auto time_since_midnight = tp_ - date::floor<date::days>(tp_);
    return static_cast<unsigned>(
        (time_since_midnight - std::chrono::hours(hour())) /
        std::chrono::minutes(1));
  }

  constexpr unsigned second() const noexcept {
    const auto time_since_midnight = tp_ - date::floor<date::days>(tp_);
    return static_cast<unsigned>((time_since_midnight -
                                  std::chrono::hours(hour()) -
                                  std::chrono::minutes(minute())) /
                                 std::chrono::seconds(1));
  }

  constexpr unsigned millisecond() const noexcept {
    const auto time_since_midnight = tp_ - date::floor<date::days>(tp_);
    return static_cast<unsigned>(
        (time_since_midnight - std::chrono::hours(hour()) -
         std::chrono::minutes(minute()) - std::chrono::seconds(second())) /
        std::chrono::milliseconds(1));
  }

  constexpr time_point get_time_point() const noexcept { return tp_; }

  // TimeSpan operations
  constexpr DateTime operator+(const TimeSpan& span) const noexcept {
    return DateTime(tp_ + span.duration());
  }

  constexpr DateTime operator-(const TimeSpan& span) const noexcept {
    return DateTime(tp_ - span.duration());
  }

  // Calculate difference between two DateTimes
  constexpr TimeSpan operator-(const DateTime& other) const noexcept {
    return TimeSpan::fromMilliseconds(
        std::chrono::duration_cast<std::chrono::milliseconds>(tp_ - other.tp_)
            .count());
  }

  // Comparison operators
  constexpr bool operator==(const DateTime& other) const noexcept {
    return tp_ == other.tp_;
  }

  constexpr bool operator<(const DateTime& other) const noexcept {
    return tp_ < other.tp_;
  }

  constexpr bool operator>(const DateTime& other) const noexcept {
    return tp_ > other.tp_;
  }

  constexpr bool operator<=(const DateTime& other) const noexcept {
    return tp_ <= other.tp_;
  }

  constexpr bool operator>=(const DateTime& other) const noexcept {
    return tp_ >= other.tp_;
  }

  // Convert to string representation (for ease of use)
  std::string toString() const { return format(); }

  // Stream insertion operator for easy printing
  friend std::ostream& operator<<(std::ostream& os, const DateTime& dt) {
    return os << dt.toString();
  }

  // Static method to create DateTime from current time
  static DateTime now() {
    auto now_tp = std::chrono::time_point_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now());
    return DateTime(now_tp);
  }
};
