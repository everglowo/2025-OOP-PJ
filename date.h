#ifndef DATE_H
#define DATE_H

#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>

class Date {
private:
    int year;
    int month;
    int day;

public:
    // 构造函数
    Date();
    Date(int y, int m, int d);
    Date(const std::string& dateStr); // 格式：YYYY/MM/DD

    // 获取器
    int getYear() const { return year; }
    int getMonth() const { return month; }
    int getDay() const { return day; }

    // 获取当前日期
    static Date getCurrentDate();

    // 日期格式化
    std::string toString() const; // 格式：YYYY/MM/DD

    // 日期验证
    bool isValid() const;

    // 运算符重载
    bool operator==(const Date& other) const;
};

#endif // DATE_H 