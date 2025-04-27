#ifndef DATETIME_H
#define DATETIME_H

#include <string>
#include <ctime>

class DateTime {
private:
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;

public:
    DateTime();
    DateTime(const std::string& dateTimeStr);

    static DateTime now();

    std::string toString() const;
    std::string toDateString() const;

    bool operator<(const DateTime& other) const;
    bool operator>(const DateTime& other) const;
    bool operator==(const DateTime& other) const;
    bool operator<=(const DateTime& other) const;
    bool operator>=(const DateTime& other) const;
};

#endif