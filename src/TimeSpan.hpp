//
// Created by Zach Cobell on 5/14/25.
//

#pragma once

#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>

#include "date_hh.h"

/**
 * @brief Class representing a time span with millisecond precision
 *
 * This class allows the representation of time intervals which can
 * be used to add/subtract from themselves or from DateTime objects.
 */
class TimeSpan {
 private:

  std::chrono::milliseconds m_duration;

  /**
   * @brief Constructor from milliseconds to initialize the TimeSpan
   * @param milliseconds
   */
  explicit constexpr TimeSpan(std::chrono::milliseconds milliseconds) noexcept
      : m_duration(milliseconds) {}

 public:
  // Wrapper for the components
  struct s_TimespanComponents {
    int64_t days;
    int64_t hours;
    int64_t minutes;
    int64_t seconds;
    int64_t milliseconds;
  };

  // Constructors

  /**
   * @brief Default constructor initializes the TimeSpan to zero milliseconds
   */
  constexpr TimeSpan() noexcept : m_duration(std::chrono::milliseconds{0}) {}

  /**
   * Destructor
   */
  ~TimeSpan() = default;

  /**
   * @brief Move constructor
   */
  constexpr TimeSpan(TimeSpan&&) noexcept = default;

  /**
   * @brief Initializes the TimeSpan from a components structure
   * @param components
   */
  constexpr explicit TimeSpan(const s_TimespanComponents& components) noexcept
      : m_duration(std::chrono::milliseconds{
            date::days(components.days) + std::chrono::hours(components.hours) +
            std::chrono::minutes(components.minutes) +
            std::chrono::seconds(components.seconds) +
            std::chrono::milliseconds(components.milliseconds)}) {}

  /**
   * @brief Initializes the TimeSpan from individual components
   * @param days Number of days
   * @param hours Number of hours
   * @param minutes Number of minutes
   * @param seconds Number of seconds
   * @param milliseconds Number of milliseconds
   */
  explicit constexpr TimeSpan(const int days, const int hours,
                              const int minutes, const int seconds,
                              const int milliseconds) noexcept
      : TimeSpan(s_TimespanComponents{days, hours, minutes, seconds,
                                      milliseconds}) {}

  constexpr TimeSpan(const TimeSpan&) noexcept = default;

  [[nodiscard]] constexpr auto operator=(const TimeSpan&) noexcept
      -> TimeSpan& = default;

  [[nodiscard]] constexpr auto operator=(TimeSpan&&) noexcept
      -> TimeSpan& = default;

  // Static factory methods

  /**
   * @brief Create a TimeSpan from a number of days
   * @param days Number of days
   * @return TimeSpan object representing the duration
   */
  [[nodiscard]] static constexpr auto fromDays(const int64_t days) noexcept
      -> TimeSpan {
    return TimeSpan(s_TimespanComponents{days, 0, 0, 0, 0});
  }

  /**
   * @brief Create a TimeSpan from a number of hours
   * @param hours Number of hours
   * @return TimeSpan object representing the duration
   */
  [[nodiscard]] static constexpr auto fromHours(const int64_t hours) noexcept
      -> TimeSpan {
    return TimeSpan(s_TimespanComponents{0, hours, 0, 0, 0});
  }

  /**
   * @brief Create a TimeSpan from a number of minutes
   * @param minutes Number of minutes
   * @return TimeSpan object representing the duration
   */
  [[nodiscard]] static constexpr auto fromMinutes(
      const int64_t minutes) noexcept -> TimeSpan {
    return TimeSpan(s_TimespanComponents{0, 0, minutes, 0, 0});
  }

  /**
   * @brief Create a TimeSpan from a number of seconds
   * @param seconds Number of seconds
   * @return TimeSpan object representing the duration
   */
  [[nodiscard]] static constexpr auto fromSeconds(
      const int64_t seconds) noexcept -> TimeSpan {
    return TimeSpan(s_TimespanComponents{0, 0, 0, seconds, 0});
  }

  /**
   * @brief Create a TimeSpan from a number of milliseconds
   * @param milliseconds Number of milliseconds
   * @return TimeSpan object representing the duration
   */
  [[nodiscard]] static constexpr auto fromMilliseconds(
      const int64_t milliseconds) noexcept -> TimeSpan {
    return TimeSpan(s_TimespanComponents{0, 0, 0, 0, milliseconds});
  }

  /**
   * @brief Get the components of the TimeSpan
   * @return Component structure containing the days, hours, minutes, seconds,
   * and milliseconds
   */
  [[nodiscard]] constexpr auto components() const noexcept {
    return to_components(m_duration.count());
  }

  /**
   * @brief Convert the TimeSpan to its components
   * @param duration_ms Duration in milliseconds
   * @return Components structure containing the days, hours, minutes, seconds,
   * and milliseconds
   */
  static constexpr auto to_components(const int64_t duration_ms) noexcept
      -> s_TimespanComponents {
    s_TimespanComponents components{0, 0, 0, 0, 0};

    bool const negative = duration_ms < 0;
    int64_t duration = negative ? -duration_ms : duration_ms;

    const auto n_days = std::chrono::duration_cast<date::days>(
        std::chrono::milliseconds(duration));
    duration -=
        std::chrono::duration_cast<std::chrono::milliseconds>(n_days).count();
    const auto n_hours = std::chrono::duration_cast<std::chrono::hours>(
        std::chrono::milliseconds(duration));
    duration -=
        std::chrono::duration_cast<std::chrono::milliseconds>(n_hours).count();
    const auto n_minutes = std::chrono::duration_cast<std::chrono::minutes>(
        std::chrono::milliseconds(duration));
    duration -= std::chrono::duration_cast<std::chrono::milliseconds>(n_minutes)
                    .count();
    const auto n_seconds = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::milliseconds(duration));
    duration -= std::chrono::duration_cast<std::chrono::milliseconds>(n_seconds)
                    .count();
    const auto n_milliseconds =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::milliseconds(duration));
    duration -=
        std::chrono::duration_cast<std::chrono::milliseconds>(n_milliseconds)
            .count();

    assert(duration == 0);

    components.days = n_days.count();
    components.hours = n_hours.count();
    components.minutes = n_minutes.count();
    components.seconds = n_seconds.count();
    components.milliseconds = n_milliseconds.count();

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

  /**
   * @brief Get the number of days in the TimeSpan
   * @return Number of days
   */
  [[nodiscard]] constexpr auto days() const noexcept -> int64_t {
    return components().days;
  }

  /**
   * @brief Get the number of hours in the TimeSpan
   * @return Number of hours
   */
  [[nodiscard]] constexpr auto hours() const noexcept -> int64_t {
    return components().hours;
  }

  /**
   * @brief Get the number of minutes in the TimeSpan
   * @return Number of minutes
   */
  [[nodiscard]] constexpr auto minutes() const noexcept -> int64_t {
    return components().minutes;
  }

  /**
   * @brief Get the number of seconds in the TimeSpan
   * @return Number of seconds
   */
  [[nodiscard]] constexpr auto seconds() const noexcept -> int64_t {
    return components().seconds;
  }

  /**
   * @brief Get the number of milliseconds in the TimeSpan
   * @return Number of milliseconds
   */
  [[nodiscard]] constexpr auto milliseconds() const noexcept -> int64_t {
    return components().milliseconds;
  }

  /**
   * @brief Get the total number of days in the TimeSpan
   * @return Total number of days
   */
  [[nodiscard]] constexpr auto totalDays() const noexcept -> int64_t {
    return std::chrono::duration_cast<date::days>(m_duration).count();
  }

  /**
   * @brief Get the total number of hours in the TimeSpan
   * @return Total number of hours
   */
  [[nodiscard]] constexpr auto totalHours() const noexcept -> int64_t {
    return std::chrono::duration_cast<std::chrono::hours>(m_duration).count();
  }

  /**
   * @brief Get the total number of minutes in the TimeSpan
   * @return Total number of minutes
   */
  [[nodiscard]] constexpr auto totalMinutes() const noexcept -> int64_t {
    return std::chrono::duration_cast<std::chrono::minutes>(m_duration).count();
  }

  /**
   * @brief Get the total number of seconds in the TimeSpan
   * @return Total number of seconds
   */
  [[nodiscard]] constexpr auto totalSeconds() const noexcept -> int64_t {
    return std::chrono::duration_cast<std::chrono::seconds>(m_duration).count();
  }

  /**
   * @brief Get the total number of milliseconds in the TimeSpan
   * @return Total number of milliseconds
   */
  [[nodiscard]] constexpr auto totalMilliseconds() const noexcept -> int64_t {
    return m_duration.count();
  }

  // Internal access

  /**
   * @brief Get the internal representation of the duration in
   * std::chrono::milliseconds
   * @return Duration in milliseconds
   */
  [[nodiscard]] constexpr auto duration() const noexcept
      -> std::chrono::milliseconds {
    return m_duration;
  }

  // Operators

  /**
   * @brief Add two timespan objects and return a new TimeSpan
   * @param other The other TimeSpan to add
   * @return A new TimeSpan representing the sum
   */
  [[nodiscard]] constexpr auto operator+(const TimeSpan& other) const noexcept
      -> TimeSpan {
    return TimeSpan(
        to_components(m_duration.count() + other.m_duration.count()));
  }

  /** @brief Subtract two timespan objects and return a new TimeSpan
   * @param other The other TimeSpan to subtract
   * @return A new TimeSpan representing the difference
   */
  [[nodiscard]] constexpr auto operator-(const TimeSpan& other) const noexcept
      -> TimeSpan {
    return TimeSpan(
        to_components(m_duration.count() - other.m_duration.count()));
  }

  /**
   * @brief Multiply the TimeSpan by a factor and return a new TimeSpan
   * @param factor The factor to multiply by
   * @return A new TimeSpan representing the product
   */
  [[nodiscard]] constexpr auto operator*(const int64_t factor) const noexcept
      -> TimeSpan {
    return TimeSpan(to_components(m_duration.count() * factor));
  }

  /**
   * @brief Divide the TimeSpan by a divisor and return a new TimeSpan
   * @param divisor The divisor to divide by
   * @return A new TimeSpan representing the quotient
   */
  [[nodiscard]] constexpr auto operator/(const int64_t divisor) const noexcept
      -> TimeSpan {
    return TimeSpan(to_components(m_duration.count() / divisor));
  }

  /**
   * @brief Equality operator for TimeSpan
   * @param other The other TimeSpan to compare with
   * @return true if both TimeSpans are equal, false otherwise
   */
  [[nodiscard]] constexpr auto operator==(const TimeSpan& other) const noexcept
      -> bool {
    return m_duration == other.m_duration;
  }

  /**
   * @brief Inequality operator for TimeSpan
   * @param other The other TimeSpan to compare with
   * @return true if both TimeSpans are not equal, false otherwise
   */
  [[nodiscard]] constexpr auto operator!=(const TimeSpan& other) const noexcept
      -> bool {
    return m_duration != other.m_duration;
  }

  /**
   * @brief Less than operator for TimeSpan
   * @param other The other TimeSpan to compare with
   * @return true if this TimeSpan is less than the other, false otherwise
   */
  [[nodiscard]] constexpr auto operator<(const TimeSpan& other) const noexcept
      -> bool {
    return m_duration < other.m_duration;
  }

  /**
   * @brief Greater than operator for TimeSpan
   * @param other The other TimeSpan to compare with
   * @return true if this TimeSpan is greater than the other, false otherwise
   */
  [[nodiscard]] constexpr auto operator>(const TimeSpan& other) const noexcept
      -> bool {
    return m_duration > other.m_duration;
  }

  /**
   * @brief Less than or equal to operator for TimeSpan
   * @param other The other TimeSpan to compare with
   * @return true if this TimeSpan is less than or equal to the other, false
   * otherwise
   */
  [[nodiscard]] constexpr auto operator<=(const TimeSpan& other) const noexcept
      -> bool {
    return m_duration <= other.m_duration;
  }

  /**
   * @brief Greater than or equal to operator for TimeSpan
   * @param other The other TimeSpan to compare with
   * @return true if this TimeSpan is greater than or equal to the other, false
   * otherwise
   */
  [[nodiscard]] constexpr auto operator>=(const TimeSpan& other) const noexcept
      -> bool {
    return m_duration >= other.m_duration;
  }

  // String representation

  /**
   * @brief Convert the TimeSpan to a string representation
   * @return String representation of the TimeSpan
   */
  [[nodiscard]] auto toString() const -> std::string {
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
