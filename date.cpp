#include "date.h"
#include <stdexcept>
#include <sstream>
#include <iomanip>

Date::Date() {
    time_t now = time(nullptr);
    tm* local_time = localtime(&now);
    year = local_time->tm_year + 1900;
    month = local_time->tm_mon + 1;
    day = local_time->tm_mday;
}

Date::Date(int y, int m, int d) : year(y), month(m), day(d) {
    if (!isValid()) {
        throw std::invalid_argument("Invalid date.");
    }
}

Date::Date(const std::string& dateStr) {
    std::stringstream ss(dateStr);
    std::string yearStr, monthStr, dayStr;
    
    // 尝试读取年月日
    if (!std::getline(ss, yearStr, '/') ||
        !std::getline(ss, monthStr, '/') ||
        !std::getline(ss, dayStr)) {
        throw std::invalid_argument("Incorrect date format. Expected YYYY/MM/DD or YYYY/M/D");
    }

    try {
        year = std::stoi(yearStr);
        month = std::stoi(monthStr);
        day = std::stoi(dayStr);
    } catch (...) {
        throw std::invalid_argument("Incorrect date format");
    }

    if (!isValid()) {
        throw std::invalid_argument("Invalid date");
    }
}

Date Date::getCurrentDate() {
    return Date();
}

std::string Date::toString() const {
    std::stringstream ss;
    ss << year << "/"
       << std::setw(2) << std::setfill('0') << month << "/"
       << std::setw(2) << std::setfill('0') << day;
    return ss.str();
}

bool Date::isValid() const {
    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    // 处理闰年
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)) {
        daysInMonth[1] = 29;
    }
    
    return day >= 1 && day <= daysInMonth[month - 1];
}

bool Date::operator==(const Date& other) const {
    return year == other.year && month == other.month && day == other.day;
} 