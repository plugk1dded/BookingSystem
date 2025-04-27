#include "datetime.h"
#include <cstdio>

DateTime::DateTime() : year(0), month(0), day(0), hour(0), minute(0), second(0) {}

DateTime::DateTime(const std::string& dateTimeStr) {
    // YYYY-MM-DD HH:MM:SS или YYYY-MM-DD
    if (dateTimeStr.length() >= 10) {
        try {
            year = std::stoi(dateTimeStr.substr(0, 4));
            month = std::stoi(dateTimeStr.substr(5, 2));
            day = std::stoi(dateTimeStr.substr(8, 2));

            if (dateTimeStr.length() >= 19) {
                hour = std::stoi(dateTimeStr.substr(11, 2));
                minute = std::stoi(dateTimeStr.substr(14, 2));
                second = std::stoi(dateTimeStr.substr(17, 2));
            }
            else {
                hour = 0;
                minute = 0;
                second = 0;
            }
        }
        catch (const std::exception& e) {
            year = 0; month = 0; day = 0;
            hour = 0; minute = 0; second = 0;
        }
    }
    else {
        year = 0; month = 0; day = 0;
        hour = 0; minute = 0; second = 0;
    }
}

DateTime DateTime::now() {
    DateTime dt;
    time_t t = time(nullptr);
    struct tm timeinfo;

    localtime_s(&timeinfo, &t);

    dt.year = timeinfo.tm_year + 1900;
    dt.month = timeinfo.tm_mon + 1;
    dt.day = timeinfo.tm_mday;
    dt.hour = timeinfo.tm_hour;
    dt.minute = timeinfo.tm_min;
    dt.second = timeinfo.tm_sec;

    return dt;
}

std::string DateTime::toString() const {
    char buffer[20];
    sprintf_s(buffer, sizeof(buffer), "%04d-%02d-%02d %02d:%02d:%02d",
        year, month, day, hour, minute, second);
    return std::string(buffer);
}

std::string DateTime::toDateString() const {
    char buffer[11];
    sprintf_s(buffer, sizeof(buffer), "%04d-%02d-%02d", year, month, day);
    return std::string(buffer);
}

bool DateTime::operator<(const DateTime& other) const {
    if (year != other.year) return year < other.year;
    if (month != other.month) return month < other.month;
    if (day != other.day) return day < other.day;
    if (hour != other.hour) return hour < other.hour;
    if (minute != other.minute) return minute < other.minute;
    return second < other.second;
}

bool DateTime::operator>(const DateTime& other) const {
    return other < *this;
}

bool DateTime::operator==(const DateTime& other) const {
    return year == other.year && month == other.month && day == other.day &&
        hour == other.hour && minute == other.minute && second == other.second;
}

bool DateTime::operator<=(const DateTime& other) const {
    return *this < other || *this == other;
}

bool DateTime::operator>=(const DateTime& other) const {
    return *this > other || *this == other;
}
