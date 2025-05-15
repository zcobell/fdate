#pragma once

#include <chrono>
#include <iostream>
#include <limits>
#include <optional>
#include <sstream>
#include <string>

#include "TimeSpan.hpp"
#include "date_hh.h"

class DateTime {
 private:
  using time_point = std::chrono::time_point<std::chrono::system_clock,
                                             std::chrono::milliseconds>;
  time_point m_tp;

 public:
  // Invalid Timestamp
  static constexpr auto INVALID_TIMESTAMP =
      -std::numeric_limits<int64_t>::max();

  // Constructors
  constexpr DateTime() noexcept : m_tp(std::chrono::milliseconds(0)) {};

  ~DateTime() = default;

  constexpr DateTime(int year, unsigned month, unsigned day, unsigned hour = 0,
                     unsigned minute = 0, unsigned second = 0,
                     unsigned millisecond = 0) noexcept
      : m_tp(std::chrono::time_point_cast<std::chrono::milliseconds>(
            date::sys_days{date::year{year} / date::month{month} /
                           date::day{day}} +
            std::chrono::hours{hour} + std::chrono::minutes{minute} +
            std::chrono::seconds{second} +
            std::chrono::milliseconds{millisecond})) {}

  // Constructor from timestamp (milliseconds since epoch)
  explicit constexpr DateTime(int64_t timestamp) noexcept
      : m_tp(time_point(std::chrono::milliseconds(timestamp))) {}

  // Constructor from time_point64_t (to help with operator implementations)
  constexpr explicit DateTime(const time_point& milliseconds) noexcept
      : m_tp(milliseconds) {}

  constexpr DateTime(const DateTime&) noexcept = default;
  constexpr auto operator=(const DateTime&) noexcept -> DateTime& = default;
  constexpr DateTime(DateTime&&) noexcept = default;
  constexpr auto operator=(DateTime&&) noexcept -> DateTime& = default;

  // Parse from string (not constexpr due to parsing)
  static auto parse(const std::string& str,
                    const std::string& format = "%Y-%m-%d %H:%M:%S")
      -> std::optional<DateTime> {
    std::istringstream input_stream(str);
    date::sys_time<std::chrono::milliseconds> this_time_point;

    // If the user has provided a string that has milliseconds (i.e., position 4
    // from the end has a period)
    if (str.size() >= 4 && str[str.size() - 4] == '.') {
      // Parse using milliseconds
      date::from_stream(input_stream, format.c_str(), this_time_point);
    } else {
      // Parse without milliseconds
      auto tp_temp = date::sys_time<std::chrono::seconds>{};
      date::from_stream(input_stream, format.c_str(), tp_temp);
      this_time_point =
          std::chrono::time_point_cast<std::chrono::milliseconds>(tp_temp);
    }

    // Check if parsing was successful
    if (input_stream.fail() || input_stream.bad()) {
      return std::nullopt;  // Parsing failed
    }

    return std::make_optional<DateTime>(this_time_point);
  }

  // Get timestamp (milliseconds since epoch)
  [[nodiscard]] constexpr auto timestamp() const noexcept -> int64_t {
    return m_tp.time_since_epoch().count();
  }

  [[nodiscard]] auto format(const std::string& fmt = "%Y-%m-%d %H:%M:%S") const
      -> std::string {
    auto tp_sec = std::chrono::time_point_cast<std::chrono::seconds>(m_tp);
    std::ostringstream oss;
    date::to_stream(oss, fmt.c_str(), tp_sec);
    return oss.str();
  }

  [[nodiscard]] auto format_w_milliseconds(
      const std::string& fmt = "%Y-%m-%d %H:%M:%S") const -> std::string {
    std::ostringstream oss;
    date::to_stream(oss, fmt.c_str(), m_tp);
    return oss.str();
  }

  // Convert to ISO 8601 format
  [[nodiscard]] auto toISOString() const -> std::string {
    return format("%Y-%m-%dT%H:%M:%S");
  }

  [[nodiscard]] auto toISOStringMsec() const -> std::string {
    return format_w_milliseconds("%Y-%m-%dT%H:%M:%S");
  }

  // Getters
  [[nodiscard]] constexpr auto year() const noexcept -> int64_t {
    return static_cast<int>(
        date::year_month_day{date::floor<date::days>(m_tp)}.year());
  }

  [[nodiscard]] constexpr auto month() const noexcept -> unsigned {
    return static_cast<unsigned>(
        date::year_month_day{date::floor<date::days>(m_tp)}.month());
  }

  [[nodiscard]] constexpr auto day() const noexcept -> unsigned {
    return static_cast<unsigned>(
        date::year_month_day{date::floor<date::days>(m_tp)}.day());
  }

  [[nodiscard]] constexpr auto hour() const noexcept -> unsigned {
    return static_cast<unsigned>((m_tp - date::floor<date::days>(m_tp)) /
                                 std::chrono::hours(1));
  }

  [[nodiscard]] constexpr auto minute() const noexcept -> unsigned {
    const auto time_since_midnight = m_tp - date::floor<date::days>(m_tp);
    return static_cast<unsigned>(
        (time_since_midnight - std::chrono::hours(hour())) /
        std::chrono::minutes(1));
  }

  [[nodiscard]] constexpr auto second() const noexcept -> unsigned {
    const auto time_since_midnight = m_tp - date::floor<date::days>(m_tp);
    return static_cast<unsigned>((time_since_midnight -
                                  std::chrono::hours(hour()) -
                                  std::chrono::minutes(minute())) /
                                 std::chrono::seconds(1));
  }

  [[nodiscard]] constexpr auto millisecond() const noexcept -> unsigned {
    const auto time_since_midnight = m_tp - date::floor<date::days>(m_tp);
    return static_cast<unsigned>(
        (time_since_midnight - std::chrono::hours(hour()) -
         std::chrono::minutes(minute()) - std::chrono::seconds(second())) /
        std::chrono::milliseconds(1));
  }

  [[nodiscard]] constexpr auto get_time_point() const noexcept -> time_point {
    return m_tp;
  }

  // TimeSpan operations
  constexpr auto operator+(const TimeSpan& span) const noexcept -> DateTime {
    return DateTime(m_tp + span.duration());
  }

  constexpr auto operator-(const TimeSpan& span) const noexcept -> DateTime {
    return DateTime(m_tp - span.duration());
  }

  // Calculate the difference between two DateTimes
  constexpr auto operator-(const DateTime& other) const noexcept -> TimeSpan {
    const auto dt_diff = m_tp - other.m_tp;
    return TimeSpan::fromMilliseconds(dt_diff.count());
  }

  // Comparison operators
  constexpr auto operator==(const DateTime& other) const noexcept -> bool {
    return m_tp == other.m_tp;
  }

  constexpr auto operator<(const DateTime& other) const noexcept -> bool {
    return m_tp < other.m_tp;
  }

  constexpr auto operator>(const DateTime& other) const noexcept -> bool {
    return m_tp > other.m_tp;
  }

  constexpr auto operator<=(const DateTime& other) const noexcept -> bool {
    return m_tp <= other.m_tp;
  }

  constexpr auto operator>=(const DateTime& other) const noexcept -> bool {
    return m_tp >= other.m_tp;
  }

  // Convert to string representation (for ease of use)
  [[nodiscard]] auto toString() const -> std::string { return format(); }

  // Stream insertion operator for easy printing
  friend auto operator<<(std::ostream& output_stream, const DateTime& date_time)
      -> std::ostream& {
    return output_stream << date_time.toString();
  }

  // Static method to create DateTime from the current time
  [[nodiscard]] static auto now() -> DateTime {
    auto now_tp = std::chrono::time_point_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now());
    return DateTime(now_tp);
  }
};
