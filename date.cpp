#include <iostream>
#include <string>
#include <vector>

class Time {
private:
    int hour;
    int minute;
    int second;

public:

    Time(int h = 0, int m = 0, int s = 0) {
        if (h < 0 || h >= 24) throw "Hour must be in range [0, 23]";
        if (m < 0 || m >= 60) throw "Minute must be in range [0, 59]";
        if (s < 0 || s >= 60) throw "Second must be in range [0, 59]";
        hour = h;
        minute = m;
        second = s;
    }

    
    // Геттеры
    int getHour() const { return hour; }
    int getMinute() const { return minute; }
    int getSecond() const { return second; }

    // Сеттеры
    void setHour(int h) {
        if (h < 0 || h >= 24) throw "Invalid hour";
        hour = h;
    }
    void setMinute(int m) {
        if (m < 0 || m >= 60) throw "Invalid minute";
        minute = m;
    }
    void setSecond(int s) {
        if (s < 0 || s >= 60) throw "Invalid second";
        second = s;
    }

    void print() const {
        std::cout << (hour < 10 ? "0" : "") << hour << ":"
                  << (minute < 10 ? "0" : "") << minute << ":"
                  << (second < 10 ? "0" : "") << second;
    }

    bool operator==(const Time& other) const {
        return hour == other.hour && minute == other.minute && second == other.second;
    }

    bool operator!=(const Time& other) const {
        return !(*this == other);
    }
};


class Date {
private:
    int day;
    int month;
    int year; 
    Time time;

    static const std::vector<int> days_in_month_common;
    std::vector<std::string> month_names = {
        "января","февраля","марта","апреля","мая","июня",
        "июля","августа","сентября","октября","ноября","декабря"
    };

public:
    Date(int d = 1, int m = 1, int y = 1, const Time& t = Time()) {
        if (m < 1 || m > 12) throw "Month must be between 1 and 12";
        int dim = daysInMonth(m, y);
        if (d < 1 || d > dim) throw "Invalid day for given month";
        day = d;
        month = m;
        year = y;
        time = t;
    }

    static bool isLeapYear(int y) {
        if (y < 0) y = -y - 1; 
        return (y % 400 == 0) || (y % 4 == 0 && y % 100 != 0);
    }

    static int daysInMonth(int m, int y) {
        if (m == 2 && isLeapYear(y)) return 29;
        return days_in_month_common[m - 1];
    }

    // Геттеры
    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getYear() const { return year; }
    Time getTime() const { return time; }

    // Сеттеры
    void setDay(int d) {
        if (d < 1 || d > daysInMonth(month, year)) throw "Invalid day";
        day = d;
    }
    void setMonth(int m) {
        if (m < 1 || m > 12) throw "Invalid month";
        month = m;
    }
    void setYear(int y) { year = y; }
    void setTime(const Time& t) { time = t; }

    //вывод даты
    void print() const {
        std::cout << day << " " << month_names[month - 1] << " ";
        if (year < 0)
            std::cout << -year << " г. до н.э.";
        else
            std::cout << year << " г.";
        std::cout << " ";
        time.print();
        std::cout << std::endl;
    }

    // Перегрузка операторов сравнения
    bool operator==(const Date& other) const {
        return day == other.day && month == other.month && year == other.year && time == other.time;
    }

    bool operator!=(const Date& other) const { return !(*this == other); }

    
};

const std::vector<int> Date::days_in_month_common = {31,28,31,30,31,30,31,31,30,31,30,31};

int main()
{
    std::cout << "=== Time tests ===\n";
    try {
        Time t1(9, 5, 3);
        std::cout << "t1: "; t1.print(); std::cout << '\n';

        Time t2;                   
        std::cout << "t2 (default): "; t2.print(); std::cout << '\n';

        std::cout << "t1 == t2 ? " << (t1 == t2 ? "yes" : "no") << '\n';

        t2.setHour(9); t2.setMinute(5); t2.setSecond(3);
        std::cout << "t2 after set: "; t2.print(); std::cout << '\n';
        std::cout << "t1 == t2 ? " << (t1 == t2 ? "yes" : "no") << '\n';

        try {
            Time t_bad(24, 0, 0);
        }
        catch (const char* e) {
            std::cerr << "Expected Time error: " << e << '\n';
        }
    }
    catch (const char* e) {
        std::cerr << "Time error: " << e << '\n';
    }
    catch (...) {
        std::cerr << "Unknown Time error\n";
    }

    std::cout << "\n=== Date tests ===\n";
    try {
        Date d1(29, 2, 2025, Time(12, 0, 0));
        std::cout << "d1: "; d1.print();

        std::cout << "isLeapYear(2025): " << (Date::isLeapYear(2025) ? "true" : "false") << '\n';
        std::cout << "daysInMonth(2,2021): " << Date::daysInMonth(2, 2021) << '\n';

        Date d2;
        std::cout << "d2 (default): "; d2.print();
        d2.setYear(1999); d2.setMonth(12); d2.setDay(31); d2.setTime(Time(23,59,59));
        std::cout << "d2 after sets: "; d2.print();

        std::cout << "d1 != d2 ? " << (d1 != d2 ? "yes" : "no") << '\n';

        Date d3(15, 3, -44, Time(12, 0, 0));
        std::cout << "d3 (BC): "; d3.print();

        try {
            Date d_bad(31, 4, 2021); 
        }
        catch (const char* e) {
            std::cerr << "Expected Date error: " << e << '\n';
        }
    }
    catch (const char* e) {
        std::cerr << "Date error: " << e << '\n';
    }
    catch (...) {
        std::cerr << "Unknown Date error\n";
    }

    return 0;
}
