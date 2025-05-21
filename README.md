# FDate Library

A modern datetime library for Fortran that provides comprehensive date and time manipulation capabilities through a clean Fortran interface built on top of Howard Hinnant's acclaimed date library.

## Background

The FDate library bridges the gap between modern C++ datetime functionality and Fortran applications. While Fortran excels in scientific computing, it has traditionally lacked robust datetime manipulation capabilities. This library provides:

- Millisecond precision datetime objects
- TimeSpan objects for duration calculations
- Comprehensive formatting and parsing with customizable format strings
- Full operator overloading for natural datetime arithmetic
- Memory-safe design using value types instead of pointers
- Cross-platform compatibility

The library uses a design where both `t_DateTime` and `t_TimeSpan` objects are represented internally as 64-bit integers (milliseconds since epoch for `t_DateTime`, total milliseconds for `t_TimeSpan`), avoiding complex memory management while maintaining full functionality.

## Features

### DateTime Operations
- Create dates from components (year, month, day, hour, minute, second, millisecond)
- Parse dates from strings with custom format specifiers
- Format dates to strings (including ISO 8601)
- Get current system time
- Extract individual components (year, month, day, etc.)
- Full comparison operators (`==`, `<`, `>`, `<=`, `>=`, `/=`)
- Add/subtract TimeSpans to/from DateTime objects

### TimeSpan Operations
- Create time spans from various units (days, hours, minutes, seconds, milliseconds)
- Extract components and total values
- Arithmetic operations (addition, subtraction, multiplication, division)
- Full comparison operators (`==`, `<`, `>`, `<=`, `>=`, `/=`)
- String representation

## Installation

### Prerequisites
- CMake 3.14 or higher
- Modern C++ compiler supporting at least C++17
- Fortran compiler (gfortran, ifort, etc.)

### Building
```bash
mkdir build && cd build
cmake ..
make
```

### CMake Options
- `FDATE_BUILD_SHARED`: Build shared library (default: OFF)
- `FDATE_ENABLE_TESTING`: Enable testing suite (default: OFF)

## Fortran Usage Examples

### Basic DateTime Creation and Manipulation

```fortran
program datetime_examples
   use mod_datetime, only: t_datetime, t_timespan, now
   implicit none
   
   type(t_datetime) :: dt1, dt2, current_time
   type(t_timespan) :: ts1, ts2, difference
   character(len=64) :: date_string
   
   ! Create a specific datetime: January 15, 2024, 14:30:45.123
   dt1 = t_datetime(2024, 1, 15, 14, 30, 45, 123)
   
   ! Create date-only (time defaults to 00:00:00.000)
   dt2 = t_datetime(2024, 12, 25)
   
   ! Get current system time
   current_time = now()
   
   ! Extract components
   write(*,*) 'Year:', dt1%year()
   write(*,*) 'Month:', dt1%month()
   write(*,*) 'Day:', dt1%day()
   write(*,*) 'Hour:', dt1%hour()
   write(*,*) 'Minute:', dt1%minute()
   write(*,*) 'Second:', dt1%second()
   write(*,*) 'Millisecond:', dt1%millisecond()
   
end program datetime_examples
```

### TimeSpan Operations

```fortran
program timespan_examples
   use mod_datetime
   implicit none
   
   type(t_timespan) :: ts1, ts2, ts_result
   type(t_datetime) :: dt_start, dt_end
   
   ! Create timespans from different units
   ts1 = t_timespan(days=2, hours=3, minutes=45, seconds=30, milliseconds=500)
   ts2 = t_timespan(hours=5)  ! 5 hours
   
   ! Arithmetic operations
   ts_result = ts1 + ts2      ! Addition
   ts_result = ts1 - ts2      ! Subtraction
   ts_result = ts1 * 2        ! Multiplication
   ts_result = ts1 / 2        ! Division
   
   ! Extract components
   write(*,*) 'Days:', ts1%days()
   write(*,*) 'Hours:', ts1%hours()
   write(*,*) 'Total hours:', ts1%total_hours()
   write(*,*) 'Total milliseconds:', ts1%total_milliseconds()
   
   ! Calculate difference between two dates
   dt_start = t_datetime(2024, 1, 1, 9, 0, 0)
   dt_end = t_datetime(2024, 1, 5, 17, 30, 0)
   ts_result = dt_end - dt_start
   
   write(*,*) 'Time difference:', ts_result%to_string()
   
end program timespan_examples
```

### Date Arithmetic and Comparisons

```fortran
program date_arithmetic
   use mod_datetime
   implicit none
   
   type(t_datetime) :: meeting_time, deadline, reminder
   type(t_timespan) :: one_week, two_hours
   
   ! Set meeting time
   meeting_time = t_datetime(2024, 6, 15, 14, 0, 0)  ! June 15, 2PM
   
   ! Create timespan objects
   one_week = t_timespan(days=7)
   two_hours = t_timespan(hours=2)
   
   ! Calculate deadline (one week after meeting)
   deadline = meeting_time + one_week
   
   ! Set reminder (2 hours before meeting)
   reminder = meeting_time - two_hours
   
   ! Comparisons
   if (reminder < meeting_time) then
      write(*,*) 'Reminder is set before the meeting'
   end if
   
   if (deadline > meeting_time) then
      write(*,*) 'Deadline is after the meeting'
   end if
   
   ! Check if dates are equal
   if (meeting_time == meeting_time) then
      write(*,*) 'Dates are identical'
   end if
   
end program date_arithmetic
```

### String Formatting and Parsing

```fortran
program string_operations
   use mod_datetime
   implicit none
   
   type(t_datetime) :: dt, parsed_dt
   character(len=64) :: formatted_string
   
   ! Create a datetime
   dt = t_datetime(2024, 3, 14, 9, 26, 53, 589)
   
   ! Format to different string representations
   formatted_string = dt%format('%Y-%m-%d %H:%M:%S')
   write(*,*) 'Standard format: ', trim(formatted_string)
   
   formatted_string = dt%format('%B %d, %Y at %I:%M %p')
   write(*,*) 'Verbose format: ', trim(formatted_string)
   
   ! ISO 8601 format
   formatted_string = dt%to_iso_string()
   write(*,*) 'ISO format: ', trim(formatted_string)
   
   ! Parse from string (assuming you have the parse functionality exposed)
   ! This would require additional wrapper functions for string parsing
   ! parsed_dt = t_datetime('2024-03-14 09:26:53', '%Y-%m-%d %H:%M:%S')
   
end program string_operations
```

## Format Specifiers

The library supports standard strftime format specifiers for date formatting:

| Specifier | Description | Example |
|-----------|-------------|---------|
| `%Y` | 4-digit year | 2024 |
| `%m` | Month (01-12) | 03 |
| `%d` | Day of month (01-31) | 14 |
| `%H` | Hour (00-23) | 09 |
| `%M` | Minute (00-59) | 26 |
| `%S` | Second (00-59) | 53 |
| `%B` | Full month name | March |
| `%A` | Full weekday name | Thursday |

## Error Handling

The library handles errors gracefully:
- Invalid date parameters return special error timestamps
- Parsing failures are indicated by returned error values
- Division by zero in TimeSpan operations should be avoided

## Thread Safety

The library is thread-safe for read operations. For write operations in multi-threaded environments, appropriate synchronization should be implemented by the calling application.

## Contributing

Contributions are welcome! Please ensure that:
- All new features include appropriate test cases
- Code follows the existing style conventions
- Documentation is updated for new functionality

## License

This project is licensed under the GNU General Public License v3.0. See the [LICENSE](LICENSE) file for details.

## Support

For questions, bug reports, or feature requests, please create an issue in the project repository.
