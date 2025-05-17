#ifndef DIET_RECORD_H
#define DIET_RECORD_H

#include <string>
#include <vector>
#include <ctime>
#include <map>
#include "food.h"
#include "user.h"
#include "date.h"

class DietRecord {
private:
    Date date;
    std::string foodName;
    double weight;         // 单位：g
    double calories;       // 单位：kcal
    FoodCategory category;

    // 私有辅助方法
    static double calculateCalories(const Food& food, double weight);
    
    // 健康报告相关方法
    static std::map<FoodCategory, double> calculateDailyIntake(const std::vector<DietRecord>& records, const Date& date);
    static double calculateTotalCalories(const std::vector<DietRecord>& records, const Date& date);
    static void generateReport(const User& user, const std::vector<DietRecord>& records, const Date& date);

    // 辅助函数：将字符串转换为首字母大写格式
    static std::string toTitleCase(const std::string& str);

public:
    DietRecord();
    DietRecord(const Date& date, const std::string& foodName, 
               double weight, double calories, FoodCategory category);
    
    // 获取器
    Date getDate() const { return date; }
    std::string getFoodName() const { return foodName; }
    double getWeight() const { return weight; }
    double getCalories() const { return calories; }
    FoodCategory getCategory() const { return category; }

    // 文件操作
    static bool saveToFile(const std::vector<DietRecord>& records);
    static bool loadFromFile(std::vector<DietRecord>& records);
    
    // 添加新记录
    static bool addNewRecord(std::vector<DietRecord>& records, std::vector<Food>& foods);
    
    // 生成健康报告
    static void generateDailyReport(const User& user, const std::vector<DietRecord>& records);
    
    // 查询功能
    static void queryRecordsByDate(const std::vector<DietRecord>& records);
};

#endif // DIET_RECORD_H 