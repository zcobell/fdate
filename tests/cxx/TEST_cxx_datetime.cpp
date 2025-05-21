#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "DateTime.hpp"
#include "TimeSpan.hpp"
#include <chrono>
#include <thread>

// ====================================================
// Compile-time tests using static_assert
// ====================================================

// TimeSpan constexpr tests
static_assert(TimeSpan().totalMilliseconds() == 0, "Default TimeSpan should have zero duration");
static_assert(TimeSpan(1, 2, 3, 4, 5).days() == 1, "Days component extraction failed");
static_assert(TimeSpan(1, 2, 3, 4, 5).hours() == 2, "Hours component extraction failed");
static_assert(TimeSpan(1, 2, 3, 4, 5).minutes() == 3, "Minutes component extraction failed");
static_assert(TimeSpan(1, 2, 3, 4, 5).seconds() == 4, "Seconds component extraction failed");
static_assert(TimeSpan(1, 2, 3, 4, 5).milliseconds() == 5, "Milliseconds component extraction failed");

static_assert(TimeSpan::fromDays(2).totalDays() == 2, "fromDays factory method failed");
static_assert(TimeSpan::fromHours(24).totalHours() == 24, "fromHours factory method failed");
static_assert(TimeSpan::fromMinutes(60).totalMinutes() == 60, "fromMinutes factory method failed");
static_assert(TimeSpan::fromSeconds(60).totalSeconds() == 60, "fromSeconds factory method failed");
static_assert(TimeSpan::fromMilliseconds(1000).totalMilliseconds() == 1000, "fromMilliseconds factory method failed");

static_assert(TimeSpan::fromDays(1) + TimeSpan::fromDays(2) == TimeSpan::fromDays(3), "TimeSpan addition failed");
static_assert(TimeSpan::fromDays(3) - TimeSpan::fromDays(1) == TimeSpan::fromDays(2), "TimeSpan subtraction failed");
static_assert(TimeSpan::fromDays(2) * 3 == TimeSpan::fromDays(6), "TimeSpan multiplication failed");
static_assert(TimeSpan::fromDays(6) / 2 == TimeSpan::fromDays(3), "TimeSpan division failed");

static_assert(TimeSpan::fromDays(1) == TimeSpan::fromDays(1), "TimeSpan equality failed");
static_assert(TimeSpan::fromDays(1) != TimeSpan::fromDays(2), "TimeSpan inequality failed");
static_assert(TimeSpan::fromDays(1) < TimeSpan::fromDays(2), "TimeSpan less than failed");
static_assert(TimeSpan::fromDays(2) > TimeSpan::fromDays(1), "TimeSpan greater than failed");
static_assert(TimeSpan::fromDays(1) <= TimeSpan::fromDays(1), "TimeSpan less than or equal failed");
static_assert(TimeSpan::fromDays(2) >= TimeSpan::fromDays(2), "TimeSpan greater than or equal failed");

// DateTime constexpr tests
static_assert(DateTime(2022, 1, 1).year() == 2022, "Year extraction failed");
static_assert(DateTime(2022, 1, 1).month() == 1, "Month extraction failed");
static_assert(DateTime(2022, 1, 1).day() == 1, "Day extraction failed");
static_assert(DateTime(2022, 1, 1, 12, 30, 45, 500).hour() == 12, "Hour extraction failed");
static_assert(DateTime(2022, 1, 1, 12, 30, 45, 500).minute() == 30, "Minute extraction failed");
static_assert(DateTime(2022, 1, 1, 12, 30, 45, 500).second() == 45, "Second extraction failed");
static_assert(DateTime(2022, 1, 1, 12, 30, 45, 500).millisecond() == 500, "Millisecond extraction failed");

static_assert(DateTime(2022, 1, 1) == DateTime(2022, 1, 1), "DateTime equality failed");
static_assert(DateTime(2022, 1, 1) < DateTime(2022, 1, 2), "DateTime less than failed");
static_assert(DateTime(2022, 1, 2) > DateTime(2022, 1, 1), "DateTime greater than failed");
static_assert(DateTime(2022, 1, 1) <= DateTime(2022, 1, 1), "DateTime less than or equal failed");
static_assert(DateTime(2022, 1, 1) >= DateTime(2022, 1, 1), "DateTime greater than or equal failed");

static_assert(DateTime(2022, 1, 1) + TimeSpan::fromDays(1) == DateTime(2022, 1, 2), "DateTime addition with TimeSpan failed");
static_assert(DateTime(2022, 1, 2) - TimeSpan::fromDays(1) == DateTime(2022, 1, 1), "DateTime subtraction with TimeSpan failed");
static_assert((DateTime(2022, 1, 2) - DateTime(2022, 1, 1)).totalDays() == 1, "DateTime difference calculation failed");

// ====================================================
// Runtime tests using Catch2
// ====================================================

TEST_CASE("TimeSpan basic functionality", "[timespan]") {
    SECTION("Default constructor") {
        TimeSpan ts;
        REQUIRE(ts.totalMilliseconds() == 0);
    }
    
    SECTION("Component constructor") {
        TimeSpan ts(TimeSpan::s_TimespanComponents{2, 3, 4, 5, 6});
        CHECK(ts.days() == 2);
        CHECK(ts.hours() == 3);
        CHECK(ts.minutes() == 4);
        CHECK(ts.seconds() == 5);
        CHECK(ts.milliseconds() == 6);
    }
    
    SECTION("Milliseconds constructor") {
        TimeSpan ts(TimeSpan::s_TimespanComponents{0,0,0,0,1000*60*60}); // 1 hour in milliseconds
        CHECK(ts.hours() == 1);
        CHECK(ts.days() == 0);
        CHECK(ts.minutes() == 0);
        CHECK(ts.seconds() == 0);
        CHECK(ts.milliseconds() == 0);
    }
    
    SECTION("Copy constructor") {
        TimeSpan ts1(1, 2, 3, 4, 5);
        TimeSpan ts2(ts1);
        CHECK(ts2.days() == 1);
        CHECK(ts2.hours() == 2);
        CHECK(ts2.minutes() == 3);
        CHECK(ts2.seconds() == 4);
        CHECK(ts2.milliseconds() == 5);
    }
    
    SECTION("Assignment operator") {
        TimeSpan ts1(1, 2, 3, 4, 5);
        const auto ts2 = ts1;
        CHECK(ts2.days() == 1);
        CHECK(ts2.hours() == 2);
        CHECK(ts2.minutes() == 3);
        CHECK(ts2.seconds() == 4);
        CHECK(ts2.milliseconds() == 5);
    }
}

TEST_CASE("TimeSpan factory methods", "[timespan]") {
    SECTION("fromDays") {
        TimeSpan ts = TimeSpan::fromDays(2);
        CHECK(ts.days() == 2);
        CHECK(ts.hours() == 0);
        CHECK(ts.totalDays() == 2);
    }
    
    SECTION("fromHours") {
        TimeSpan ts = TimeSpan::fromHours(25);
        CHECK(ts.days() == 1);
        CHECK(ts.hours() == 1);
        CHECK(ts.totalHours() == 25);
    }
    
    SECTION("fromMinutes") {
        TimeSpan ts = TimeSpan::fromMinutes(60);
        CHECK(ts.hours() == 1);
        CHECK(ts.minutes() == 0);
        CHECK(ts.totalMinutes() == 60);
    }
    
    SECTION("fromSeconds") {
        TimeSpan ts = TimeSpan::fromSeconds(60);
        CHECK(ts.minutes() == 1);
        CHECK(ts.seconds() == 0);
        CHECK(ts.totalSeconds() == 60);
    }
    
    SECTION("fromMilliseconds") {
        TimeSpan ts = TimeSpan::fromMilliseconds(1000);
        CHECK(ts.seconds() == 1);
        CHECK(ts.milliseconds() == 0);
        CHECK(ts.totalMilliseconds() == 1000);
    }
}

TEST_CASE("TimeSpan component and total accessors", "[timespan]") {
    SECTION("Components add up correctly") {
        // 1 day, 2 hours, 3 minutes, 4 seconds, 5 milliseconds
        TimeSpan ts(1, 2, 3, 4, 5);
        
        // Check components
        CHECK(ts.days() == 1);
        CHECK(ts.hours() == 2);
        CHECK(ts.minutes() == 3);
        CHECK(ts.seconds() == 4);
        CHECK(ts.milliseconds() == 5);
        
        // Check totals
        int64_t expectedTotalMilliseconds = 
            1LL * 24 * 60 * 60 * 1000 +
            2LL * 60 * 60 * 1000 +
            3LL * 60 * 1000 +
            4LL * 1000 +
            5;
            
        CHECK(ts.totalMilliseconds() == expectedTotalMilliseconds);
        CHECK(ts.totalSeconds() == expectedTotalMilliseconds / 1000);
        CHECK(ts.totalMinutes() == expectedTotalMilliseconds / (60 * 1000));
        CHECK(ts.totalHours() == expectedTotalMilliseconds / (60 * 60 * 1000));
        CHECK(ts.totalDays() == expectedTotalMilliseconds / (24 * 60 * 60 * 1000));
    }
    
    SECTION("Negative durations") {
        // Create a timespan representing -1 day
        TimeSpan negative(TimeSpan::s_TimespanComponents{0,0,0,0,-1 * 24 * 60 * 60 * 1000});
        
        CHECK(negative.days() == -1);
        CHECK(negative.totalDays() == -1);
        CHECK(negative.totalHours() == -24);
        CHECK(negative.totalMinutes() == -24 * 60);
        CHECK(negative.totalSeconds() == -24 * 60 * 60);
        CHECK(negative.totalMilliseconds() == -24LL * 60 * 60 * 1000);
    }
}

TEST_CASE("TimeSpan arithmetic operations", "[timespan]") {
    SECTION("Addition") {
        TimeSpan ts1 = TimeSpan::fromDays(1);
        TimeSpan ts2 = TimeSpan::fromHours(12);
        
        TimeSpan sum = ts1 + ts2;
        CHECK(sum.totalHours() == 36);
    }
    
    SECTION("Subtraction") {
        TimeSpan ts1 = TimeSpan::fromDays(2);
        TimeSpan ts2 = TimeSpan::fromHours(24);
        
        TimeSpan diff = ts1 - ts2;
        CHECK(diff.totalDays() == 1);
    }
    
    SECTION("Multiplication") {
        TimeSpan ts = TimeSpan::fromHours(2);
        
        TimeSpan product = ts * 3;
        CHECK(product.totalHours() == 6);
    }
    
    SECTION("Division") {
        TimeSpan ts = TimeSpan::fromHours(6);
        
        TimeSpan quotient = ts / 2;
        CHECK(quotient.totalHours() == 3);
    }
    
    SECTION("Chained operations") {
        TimeSpan result = TimeSpan::fromHours(6) + TimeSpan::fromMinutes(30) - TimeSpan::fromMinutes(15);
        CHECK(result.totalMinutes() == 6 * 60 + 15);
    }
}

TEST_CASE("TimeSpan comparison operators", "[timespan]") {
    TimeSpan ts1 = TimeSpan::fromHours(1);
    TimeSpan ts2 = TimeSpan::fromHours(2);
    TimeSpan ts3 = TimeSpan::fromHours(1);
    
    SECTION("Equality") {
        CHECK(ts1 == ts3);
        CHECK_FALSE(ts1 == ts2);
    }
    
    SECTION("Inequality") {
        CHECK(ts1 != ts2);
        CHECK_FALSE(ts1 != ts3);
    }
    
    SECTION("Less than") {
        CHECK(ts1 < ts2);
        CHECK_FALSE(ts2 < ts1);
        CHECK_FALSE(ts1 < ts3);
    }
    
    SECTION("Greater than") {
        CHECK(ts2 > ts1);
        CHECK_FALSE(ts1 > ts2);
        CHECK_FALSE(ts1 > ts3);
    }
    
    SECTION("Less than or equal") {
        CHECK(ts1 <= ts2);
        CHECK(ts1 <= ts3);
        CHECK_FALSE(ts2 <= ts1);
    }
    
    SECTION("Greater than or equal") {
        CHECK(ts2 >= ts1);
        CHECK(ts1 >= ts3);
        CHECK_FALSE(ts1 >= ts2);
    }
}

TEST_CASE("TimeSpan string representation", "[timespan]") {
    SECTION("Basic format") {
        TimeSpan ts(1, 2, 3, 4, 5);
        CHECK(ts.toString() == "1d 02:03:04.005");
    }
    
    SECTION("Without days") {
        TimeSpan ts(0, 2, 3, 4, 5);
        CHECK(ts.toString() == "02:03:04.005");
    }
    
    SECTION("Without milliseconds") {
        TimeSpan ts(1, 2, 3, 4, 0);
        CHECK(ts.toString() == "1d 02:03:04");
    }
    
    SECTION("Only hours, minutes, seconds") {
        TimeSpan ts(0, 2, 3, 4, 0);
        CHECK(ts.toString() == "02:03:04");
    }
}

TEST_CASE("DateTime constructors", "[datetime]") {
    SECTION("Default constructor") {
        DateTime dt;
        CHECK(dt.timestamp() == 0);
    }
    
    SECTION("Components constructor") {
        DateTime dt(2022, 1, 31, 12, 34, 56, 789);
        CHECK(dt.year() == 2022);
        CHECK(dt.month() == 1);
        CHECK(dt.day() == 31);
        CHECK(dt.hour() == 12);
        CHECK(dt.minute() == 34);
        CHECK(dt.second() == 56);
        CHECK(dt.millisecond() == 789);
    }
    
    SECTION("Timestamp constructor") {
        // Construct a known datetime
        DateTime refDt(2022, 1, 31, 12, 34, 56, 789);
        int64_t ts = refDt.timestamp();
        
        // Create from timestamp
        DateTime dt(ts);
        
        CHECK(dt.year() == 2022);
        CHECK(dt.month() == 1);
        CHECK(dt.day() == 31);
        CHECK(dt.hour() == 12);
        CHECK(dt.minute() == 34);
        CHECK(dt.second() == 56);
        CHECK(dt.millisecond() == 789);
    }
    
    SECTION("Copy constructor") {
        DateTime dt1(2022, 1, 31, 12, 34, 56, 789);
        DateTime dt2(dt1);
        
        CHECK(dt2.year() == 2022);
        CHECK(dt2.month() == 1);
        CHECK(dt2.day() == 31);
        CHECK(dt2.hour() == 12);
        CHECK(dt2.minute() == 34);
        CHECK(dt2.second() == 56);
        CHECK(dt2.millisecond() == 789);
    }
    
    SECTION("Move constructor") {
        DateTime dt1(2022, 1, 31, 12, 34, 56, 789);
        DateTime dt2(std::move(dt1));
        
        CHECK(dt2.year() == 2022);
        CHECK(dt2.month() == 1);
        CHECK(dt2.day() == 31);
        CHECK(dt2.hour() == 12);
        CHECK(dt2.minute() == 34);
        CHECK(dt2.second() == 56);
        CHECK(dt2.millisecond() == 789);
    }
    
    SECTION("Assignment operator") {
        DateTime dt1(2022, 1, 31, 12, 34, 56, 789);
        DateTime dt2;
        dt2 = dt1;
        
        CHECK(dt2.year() == 2022);
        CHECK(dt2.month() == 1);
        CHECK(dt2.day() == 31);
        CHECK(dt2.hour() == 12);
        CHECK(dt2.minute() == 34);
        CHECK(dt2.second() == 56);
        CHECK(dt2.millisecond() == 789);
    }
    
    SECTION("Move assignment operator") {
        DateTime dt1(2022, 1, 31, 12, 34, 56, 789);
        DateTime dt2;
        dt2 = std::move(dt1);
        
        CHECK(dt2.year() == 2022);
        CHECK(dt2.month() == 1);
        CHECK(dt2.day() == 31);
        CHECK(dt2.hour() == 12);
        CHECK(dt2.minute() == 34);
        CHECK(dt2.second() == 56);
        CHECK(dt2.millisecond() == 789);
    }
}

TEST_CASE("DateTime parse method", "[datetime]") {
    SECTION("Default format") {
        auto dt = DateTime::parse("2022-01-31 12:34:56");
        
        CHECK(dt.has_value());
        CHECK(dt->year() == 2022);
        CHECK(dt->month() == 1);
        CHECK(dt->day() == 31);
        CHECK(dt->hour() == 12);
        CHECK(dt->minute() == 34);
        CHECK(dt->second() == 56);
        CHECK(dt->millisecond() == 0);
    }
    
    SECTION("Custom format") {
        auto dt = DateTime::parse("31/01/2022 12:34:56", "%d/%m/%Y %H:%M:%S");
        
        CHECK(dt.has_value());
        CHECK(dt->year() == 2022);
        CHECK(dt->month() == 1);
        CHECK(dt->day() == 31);
        CHECK(dt->hour() == 12);
        CHECK(dt->minute() == 34);
        CHECK(dt->second() == 56);
        CHECK(dt->millisecond() == 0);
    }
    
    SECTION("With milliseconds in string") {
        auto dt = DateTime::parse("2022-01-31 12:34:56.789");
        
        CHECK(dt.has_value());
        CHECK(dt->year() == 2022);
        CHECK(dt->month() == 1);
        CHECK(dt->day() == 31);
        CHECK(dt->hour() == 12);
        CHECK(dt->minute() == 34);
        CHECK(dt->second() == 56);
        CHECK(dt->millisecond() == 789);
    }
    
    SECTION("With %f format specifier") {
        auto dt = DateTime::parse("2022-01-31 12:34:56.789", "%Y-%m-%d %H:%M:%S");
        
        CHECK(dt.has_value());
        CHECK(dt->year() == 2022);
        CHECK(dt->month() == 1);
        CHECK(dt->day() == 31);
        CHECK(dt->hour() == 12);
        CHECK(dt->minute() == 34);
        CHECK(dt->second() == 56);
        CHECK(dt->millisecond() == 789);
    }
}

TEST_CASE("DateTime format and toISOString methods", "[datetime]") {
    DateTime dt(2022, 1, 31, 12, 34, 56, 789);
    
    SECTION("Default format") {
        CHECK(dt.format() == "2022-01-31 12:34:56");
    }
    
    SECTION("Custom format") {
        CHECK(dt.format("%d/%m/%Y %H:%M:%S") == "31/01/2022 12:34:56");
    }
    
    SECTION("Format with milliseconds") {
        CHECK(dt.format_w_milliseconds("%Y-%m-%d %H:%M:%S") == "2022-01-31 12:34:56.789");
    }
    
    SECTION("ISO string format") {
        CHECK(dt.toISOString() == "2022-01-31T12:34:56");
        CHECK(dt.toISOStringMsec() == "2022-01-31T12:34:56.789");
    }
    
    SECTION("Format with various specifiers") {
        CHECK(dt.format("%Y") == "2022");
        CHECK(dt.format("%m") == "01");
        CHECK(dt.format("%d") == "31");
        CHECK(dt.format("%H") == "12");
        CHECK(dt.format("%M") == "34");
        CHECK(dt.format("%S") == "56");
        CHECK(dt.format("%Y-%m") == "2022-01");
    }
}

TEST_CASE("DateTime timestamp method", "[datetime]") {
    DateTime dt(2022, 1, 31, 12, 34, 56, 789);
    int64_t ts = dt.timestamp();
    
    // Create a new DateTime from the timestamp
    DateTime dt2(ts);
    
    // Both should represent the same point in time
    CHECK(dt.year() == dt2.year());
    CHECK(dt.month() == dt2.month());
    CHECK(dt.day() == dt2.day());
    CHECK(dt.hour() == dt2.hour());
    CHECK(dt.minute() == dt2.minute());
    CHECK(dt.second() == dt2.second());
    CHECK(dt.millisecond() == dt2.millisecond());
    CHECK(dt.timestamp() == dt2.timestamp());
}

TEST_CASE("DateTime arithmetic with TimeSpan", "[datetime]") {
    DateTime dt(2022, 1, 15, 12, 0, 0, 0);
    
    SECTION("Addition") {
        DateTime result = dt + TimeSpan::fromDays(10);
        CHECK(result.year() == 2022);
        CHECK(result.month() == 1);
        CHECK(result.day() == 25);
        CHECK(result.hour() == 12);
        
        result = dt + TimeSpan::fromHours(12);
        CHECK(result.day() == 16);
        CHECK(result.hour() == 0);
    }
    
    SECTION("Subtraction") {
        DateTime result = dt - TimeSpan::fromDays(10);
        CHECK(result.year() == 2022);
        CHECK(result.month() == 1);
        CHECK(result.day() == 5);
        CHECK(result.hour() == 12);
        
        result = dt - TimeSpan::fromHours(13);
        CHECK(result.day() == 14);
        CHECK(result.hour() == 23);
    }
    
    SECTION("Difference between DateTimes") {
        DateTime dt1(2022, 1, 15, 12, 0, 0, 0);
        DateTime dt2(2022, 1, 20, 18, 30, 0, 0);
        
        TimeSpan diff = dt2 - dt1;
        CHECK(diff.totalDays() == 5);
        CHECK(diff.totalHours() == 5 * 24 + 6);
        CHECK(diff.totalMinutes() == (5 * 24 + 6) * 60 + 30);
    }
}

TEST_CASE("DateTime comparison operators", "[datetime]") {
    DateTime dt1(2022, 1, 15);
    DateTime dt2(2022, 1, 20);
    DateTime dt3(2022, 1, 15);
    
    SECTION("Equality") {
        CHECK(dt1 == dt3);
        CHECK_FALSE(dt1 == dt2);
    }
    
    SECTION("Less than") {
        CHECK(dt1 < dt2);
        CHECK_FALSE(dt2 < dt1);
        CHECK_FALSE(dt1 < dt3);
    }
    
    SECTION("Greater than") {
        CHECK(dt2 > dt1);
        CHECK_FALSE(dt1 > dt2);
        CHECK_FALSE(dt1 > dt3);
    }
    
    SECTION("Less than or equal") {
        CHECK(dt1 <= dt2);
        CHECK(dt1 <= dt3);
        CHECK_FALSE(dt2 <= dt1);
    }
    
    SECTION("Greater than or equal") {
        CHECK(dt2 >= dt1);
        CHECK(dt1 >= dt3);
        CHECK_FALSE(dt1 >= dt2);
    }
}

TEST_CASE("DateTime now method", "[datetime]") {
    // Get current time
    DateTime now = DateTime::now();
    
    // Get current time using standard library
    const auto stdNow = std::chrono::system_clock::now();
    
    // We can't use std::gmtime in thread-safe way across all platforms, so 
    // we just check that now() returns a recent timestamp
    auto nowTs = now.timestamp();
    auto stdTs = std::chrono::duration_cast<std::chrono::milliseconds>(
        stdNow.time_since_epoch()).count();
    
    // Timestamps should be within 5 seconds of each other
    CHECK(std::abs(nowTs - stdTs) < 5000);
    
    // Basic validation that now() returns a sensible date (not 1970)
    CHECK(now.year() >= 2022);
}

TEST_CASE("TimeSpan and DateTime edge cases", "[edge]") {
    SECTION("DateTime with extreme values") {
        // Far past date
        DateTime ancient(1, 1, 1);
        
        // Very far future date
        DateTime farFuture(9999, 12, 31, 23, 59, 59, 999);
        
        CHECK(ancient < farFuture);
    }
    
    SECTION("TimeSpan with large values") {
        // Very large TimeSpan
        TimeSpan largeSpan(TimeSpan::s_TimespanComponents{10000,0,0,0,0}); // 10000 days
        
        CHECK(largeSpan.totalDays() == 10000);
        CHECK(largeSpan.days() == 10000);
    }
    
    SECTION("Date wrapping") {
        // Create a date and add enough time to wrap to the next month
        DateTime dt(2022, 1, 31);
        DateTime result = dt + TimeSpan::fromDays(1);
        
        CHECK(result.year() == 2022);
        CHECK(result.month() == 2);
        CHECK(result.day() == 1);
    }
    
    SECTION("Leap year handling") {
        // Test February 29 in leap year
        DateTime leapDay(2020, 2, 29);
        
        // Add one year
        DateTime nextYear = leapDay + TimeSpan::fromDays(366);
        
        // Should be March 1, 2021 (2020 was a leap year)
        CHECK(nextYear.year() == 2021);
        CHECK(nextYear.month() == 3);
        CHECK(nextYear.day() == 1);
    }
}

TEST_CASE("TimeSpan arithmetic with different units", "[timespan]") {
    SECTION("Mixed units addition") {
        TimeSpan days = TimeSpan::fromDays(1);
        TimeSpan hours = TimeSpan::fromHours(12);
        TimeSpan minutes = TimeSpan::fromMinutes(30);
        
        TimeSpan total = days + hours + minutes;
        
        CHECK(total.totalHours() == 36);
        CHECK(total.totalMinutes() == 36 * 60 + 30);
    }
    
    SECTION("Overflow handling") {
        TimeSpan hours = TimeSpan::fromHours(25);
        
        CHECK(hours.days() == 1);
        CHECK(hours.hours() == 1);
    }
}

TEST_CASE("DateTime parsing edge cases", "[datetime]") {
    SECTION("Invalid date string handling") {
        // This tests that the code doesn't crash, not specific results
        auto dt = DateTime::parse("not a date");
        
        // Just verify we can access methods without crashing
        CHECK_FALSE(dt.has_value());

    }
    
    SECTION("Partially valid date string") {
        auto dt = DateTime::parse("2022-01-XX");

        // This should not crash, but dt should be invalid
        CHECK_FALSE(dt.has_value());

    }
}

TEST_CASE("DateTime serialization roundtrip", "[datetime]") {
    DateTime original(2022, 3, 15, 14, 30, 45, 500);
    
    // Convert to string
    std::string iso = original.toISOStringMsec();
\
    // Parse back with appropriate format
    auto parsed = DateTime::parse(iso, "%Y-%m-%dT%H:%M:%S");
    
    // Should be the same timestamp
    CHECK(parsed.has_value());
    CHECK(parsed->timestamp() == original.timestamp());
}

TEST_CASE("TimeSpan for measuring elapsed time", "[timespan]") {
    auto startTime = DateTime::now();
    
    // Sleep for a short duration
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    auto endTime = DateTime::now();
    TimeSpan elapsed = endTime - startTime;
    
    // Should be at least our sleep duration
    CHECK(elapsed.totalMilliseconds() >= 100);
    
    // Shouldn't be too much longer (adjust tolerance as needed)
    CHECK(elapsed.totalMilliseconds() < 1000);
}

TEST_CASE("DateTime stream insertion operator", "[datetime]") {
    DateTime dt(2022, 1, 15, 12, 30, 45);
    std::ostringstream oss;
    
    oss << dt;
    
    CHECK(oss.str() == "2022-01-15 12:30:45");
}

TEST_CASE("DateTime get_time_point method", "[datetime]") {
    DateTime dt(2022, 1, 15, 12, 30, 45);
    auto tp = dt.get_time_point();
    
    // Create a new DateTime from time_point
    DateTime dt2(tp);
    
    CHECK(dt.timestamp() == dt2.timestamp());
    CHECK(dt.year() == dt2.year());
    CHECK(dt.month() == dt2.month());
    CHECK(dt.day() == dt2.day());
    CHECK(dt.hour() == dt2.hour());
    CHECK(dt.minute() == dt2.minute());
    CHECK(dt.second() == dt2.second());
}
