// datetime_fuzzer.cpp - updated version without external dependencies
#include <cstdint>
#include <cstddef>
#include <string>
#include "DateTime.hpp"
#include "TimeSpan.hpp"
#include "fuzzer/FuzzedDataProvider.h"

// Hardcoded test cases to use when fuzzer is run with no inputs
const char* kTestCases[] = {
    // ISO dates
    "2000-01-01",
    "1970-01-01 00:00:00",
    "2000-02-29",
    "9999-12-31 23:59:59.999",
    "-0001-01-01 00:00:00",

    // Timespan values
    "24:00:00",
    "1d 12:30:45.500",
    "-10:15:30",

    // Format strings
    "%Y-%m-%d",
    "%Y-%m-%dT%H:%M:%S",
    "%H:%M:%S",
    "%a, %d %b %Y"
};

// TimeSpan Fuzzer with FuzzedDataProvider
extern "C" int LLVMFuzzerTestOneInput_TimeSpan(const uint8_t* data, size_t size) {
  if (size < 8) return 0;

  FuzzedDataProvider fdp(data, size);

  try {
    // Extract values with full range - no artificial bounds
    int64_t days = fdp.ConsumeIntegral<int64_t>();
    int hours = fdp.ConsumeIntegral<int>();
    int minutes = fdp.ConsumeIntegral<int>();
    int seconds = fdp.ConsumeIntegral<int>();
    int milliseconds = fdp.ConsumeIntegral<int>();

    // Test TimeSpan constructor
    TimeSpan ts(days, hours, minutes, seconds, milliseconds);

    // Test factory methods
    auto ts1 = TimeSpan::fromDays(days);
    auto ts2 = TimeSpan::fromHours(hours);
    auto ts3 = TimeSpan::fromMinutes(minutes);
    auto ts4 = TimeSpan::fromSeconds(seconds);
    auto ts5 = TimeSpan::fromMilliseconds(milliseconds);

    // Test arithmetic operations
    auto ts_add = ts + ts1;
    auto ts_sub = ts - ts2;

    // For multiplication and division, we can use additional values
    int64_t factor = fdp.ConsumeIntegral<int64_t>();
    auto ts_mul = ts * factor;

    int64_t divisor = fdp.ConsumeIntegral<int64_t>();
    if (divisor != 0) {
      auto ts_div = ts / divisor;
    }

    // Test comparison operators
    bool eq = ts == ts1;
    bool neq = ts != ts2;
    bool lt = ts < ts3;
    bool gt = ts > ts4;
    bool lte = ts <= ts5;
    bool gte = ts >= ts1;

    // Test conversion methods
    auto str = ts.toString();
    auto days_comp = ts.days();
    auto hours_comp = ts.hours();
    auto minutes_comp = ts.minutes();
    auto seconds_comp = ts.seconds();
    auto ms_comp = ts.milliseconds();

    auto total_days = ts.totalDays();
    auto total_hours = ts.totalHours();
    auto total_minutes = ts.totalMinutes();
    auto total_seconds = ts.totalSeconds();
    auto total_ms = ts.totalMilliseconds();

  } catch (const std::exception& e) {
    // Catch exceptions but don't report as crashes
  }

  return 0;
}

// DateTime Fuzzer with FuzzedDataProvider
extern "C" int LLVMFuzzerTestOneInput_DateTime(const uint8_t* data, size_t size) {
  if (size < 16) return 0;

  FuzzedDataProvider fdp(data, size);

  try {
    // Extract potentially extreme values for DateTime components
    int year = fdp.ConsumeIntegral<int>();
    unsigned month = fdp.ConsumeIntegral<unsigned int>();
    unsigned day = fdp.ConsumeIntegral<unsigned int>();
    unsigned hour = fdp.ConsumeIntegral<unsigned int>();
    unsigned minute = fdp.ConsumeIntegral<unsigned int>();
    unsigned second = fdp.ConsumeIntegral<unsigned int>();
    unsigned millisecond = fdp.ConsumeIntegral<unsigned int>();

    // Try to create DateTime with any values
    DateTime dt(year, month, day, hour, minute, second, millisecond);

    // Test timestamp constructor
    int64_t timestamp = fdp.ConsumeIntegral<int64_t>();
    DateTime dt_from_ts(timestamp);

    // Test current time
    auto now = DateTime::now();

    // Test parsing with potentially malformed strings
    std::string dateString = fdp.ConsumeRandomLengthString(100);
    std::string formatString = fdp.ConsumeRandomLengthString(50);

    auto parsed = DateTime::parse(dateString, formatString);

    // If we got a valid DateTime from any method, test operations on it
    if (parsed) {
      // Create a TimeSpan with extreme values
      TimeSpan span(
          fdp.ConsumeIntegral<int64_t>(),
          fdp.ConsumeIntegral<int>(),
          fdp.ConsumeIntegral<int>(),
          fdp.ConsumeIntegral<int>(),
          fdp.ConsumeIntegral<int>()
      );

      // Test arithmetic operations
      auto dt_add = *parsed + span;
      auto dt_sub = *parsed - span;
      auto diff = *parsed - dt;

      // Test comparisons
      bool eq = *parsed == dt;
      bool neq = *parsed != dt;
      bool lt = *parsed < dt;
      bool gt = *parsed > dt;
      bool lte = *parsed <= dt;
      bool gte = *parsed >= dt;

      // Test formatting
      auto str1 = parsed->format();
      auto str2 = parsed->format(formatString);
      auto iso = parsed->toISOString();

      // Test component getters
      auto y = parsed->year();
      auto m = parsed->month();
      auto d = parsed->day();
      auto h = parsed->hour();
      auto min = parsed->minute();
      auto s = parsed->second();
      auto ms = parsed->millisecond();
    }

  } catch (const std::exception& e) {
    // Catch exceptions but don't report as crashes
  }

  return 0;
}


// Main fuzzer entry point
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // If data is empty or very small, we'll use some hardcoded test cases
  if (size < 4) {
    // Choose one of our hardcoded test cases
    const char* test_case = kTestCases[size % std::size(kTestCases)];
    return LLVMFuzzerTestOneInput((const uint8_t*)test_case, strlen(test_case));
  }

  FuzzedDataProvider fdp(data, size);

  // Choose which fuzzer to run
  int fuzzer_choice = fdp.ConsumeIntegralInRange<int>(0, 1);

  // Get the remaining data after consumption
  std::vector<uint8_t> remaining_data = fdp.ConsumeRemainingBytes<uint8_t>();

  switch (fuzzer_choice) {
    case 0:
      return LLVMFuzzerTestOneInput_TimeSpan(remaining_data.data(), remaining_data.size());
    case 1:
      return LLVMFuzzerTestOneInput_DateTime(remaining_data.data(), remaining_data.size());
    default:
      return 0;
  }
}

// Optional: Special entry point used by libFuzzer when run with no input files
// This initializes the corpus with our test cases
extern "C" int LLVMFuzzerInitialize(int* argc, char*** argv) {
  return 0;
}
