#include "diet_record.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <map>
#include <cctype>

DietRecord::DietRecord() : weight(0), calories(0), category(FoodCategory::StapleFood) {
    date = Date::getCurrentDate();
}

DietRecord::DietRecord(const Date& date, const std::string& foodName, 
                      double weight, double calories, FoodCategory category)
    : date(date), foodName(foodName), weight(weight), 
      calories(calories), category(category) {}

std::string DietRecord::toTitleCase(const std::string& str) {
    if (str.empty()) return str;
    
    std::string result = str;
    bool newWord = true;
    
    for (char& c : result) {
        if (std::isspace(c)) {
            newWord = true;
        } else if (newWord) {
            c = std::toupper(c);
            newWord = false;
        } else {
            c = std::tolower(c);
        }
    }
    
    return result;
}

bool DietRecord::saveToFile(const std::vector<DietRecord>& records) {
    // 创建临时文件
    std::string tempFile = "diet_records_temp.csv";
    std::ofstream file(tempFile);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to create temporary file" << std::endl;
        return false;
    }
    
    // 写入表头
    file << "Date,Category,Name,Weight(g),Calories(kcal)\n";
    
    // 写入记录数据
    for (const auto& record : records) {
        file << record.date.toString() << ","
             << Food::categoryToString(record.category) << ","
             << record.foodName << ","
             << record.weight << ","
             << record.calories << "\n";
    }
    
    file.close();
    
    // 如果原文件存在，先删除
    std::remove("diet_records.csv");
    
    // 重命名临时文件
    if (std::rename(tempFile.c_str(), "diet_records.csv") != 0) {
        std::cerr << "Error: Unable to save file" << std::endl;
        return false;
    }
    
    return true;
}

bool DietRecord::loadFromFile(std::vector<DietRecord>& records) {
    std::ifstream file("diet_records.csv");
    if (!file.is_open()) {
        return false;
    }
    
    records.clear(); // 清空现有数据
    std::string line;
    
    // 跳过表头
    std::getline(file, line);
    
    // 读取记录数据
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string date_str, category_str, name, weight_str, calories_str;
        
        std::getline(ss, date_str, ',');
        std::getline(ss, category_str, ',');
        std::getline(ss, name, ',');
        std::getline(ss, weight_str, ',');
        std::getline(ss, calories_str, ',');
        
        try {
            DietRecord record;
            record.date = Date(date_str);
            record.foodName = name;
            record.weight = std::stod(weight_str);
            record.calories = std::stod(calories_str);
            record.category = Food::stringToCategory(category_str);
            
            records.push_back(record);
        } catch (const std::exception& e) {
            std::cerr << "Error: loading records" << e.what() << std::endl;
            continue;
        }
    }
    
    file.close();
    return true;
}

bool DietRecord::addNewRecord(std::vector<DietRecord>& records, std::vector<Food>& foods) {
    std::cout << "Select a category (S/A/V/F/B: Staple Food/Animal Proteins/Vegetables/Fruits/Beverages): ";
    std::string catInput;
    std::getline(std::cin, catInput);
    FoodCategory category = FoodCategory::StapleFood;
    if (catInput == "S" || catInput == "s") category = FoodCategory::StapleFood;
    else if (catInput == "A" || catInput == "a") category = FoodCategory::AnimalProteins;
    else if (catInput == "V" || catInput == "v") category = FoodCategory::Vegetables;
    else if (catInput == "F" || catInput == "f") category = FoodCategory::Fruits;
    else if (catInput == "B" || catInput == "b") category = FoodCategory::Beverages;
    else {
        std::cout << "Invalid input." << std::endl;
        return false;
    }

    std::cout << "Food name: ";
    std::string foodName;
    std::getline(std::cin, foodName);
    foodName = toTitleCase(foodName);  // 转换为首字母大写格式

    auto it = std::find_if(foods.begin(), foods.end(),
        [&](const Food& f) { return f.getName() == foodName; });

    double calories = 0;
    if (it == foods.end()) {
        std::cout << foodName << " is not in the food database." << std::endl;
        std::cout << "Enter calories per 100g (kcal/100g): ";
        std::string calStr;
        std::getline(std::cin, calStr);
        calories = std::stod(calStr);
        foods.push_back(Food(foodName, calories, category));
        Food::saveToFile(foods);
        std::cout<<"Food record saved."<<std::endl;
    } else {
        calories = it->getCalories();
        category = it->getCategory();
    }

    std::cout << "Food weight (g): ";
    std::string weightStr;
    std::getline(std::cin, weightStr);
    double weight = std::stod(weightStr);

    double totalCalories = calories * weight / 100.0;
    records.push_back(DietRecord(Date::getCurrentDate(), foodName, weight, totalCalories, category));
    DietRecord::saveToFile(records);
    std::cout<<"Diet record saved."<<std::endl;
    return true;
}

std::map<FoodCategory, double> DietRecord::calculateDailyIntake(
    const std::vector<DietRecord>& records, const Date& date) {
    
    std::map<FoodCategory, double> intake;
    
    // 初始化所有类别的摄入量为0
    intake[FoodCategory::StapleFood] = 0;
    intake[FoodCategory::AnimalProteins] = 0;
    intake[FoodCategory::Vegetables] = 0;
    intake[FoodCategory::Fruits] = 0;
    intake[FoodCategory::Beverages] = 0;
    
    // 计算当日各类食物的摄入总量
    for (const auto& record : records) {
        if (record.date == date) {
            intake[record.category] += record.weight;
        }
    }
    
    return intake;
}

double DietRecord::calculateTotalCalories(
    const std::vector<DietRecord>& records, const Date& date) {
    
    double total = 0;
    for (const auto& record : records) {
        if (record.date == date) {
            total += record.calories;
        }
    }
    return total;
}

void DietRecord::generateReport(
    const User& user, const std::vector<DietRecord>& records, const Date& date) {
    std::ofstream report("diet_report.txt");
    if (!report.is_open()) {
        std::cerr << "Failed to create report file!" << std::endl;
        return;
    }
    auto print = [&report](const std::string& text) {
        std::cout << text;
        report << text;
    };

    print("============================================================\n");
    print("Daily Diet Analysis Report (" + std::to_string(date.getYear()) + "/" + std::to_string(date.getMonth()) + "/" + std::to_string(date.getDay()) + ")\n");
    print("------------------------------------------------------------\n");
    print("Name: " + user.getName() + "\n");
    print("Age: " + std::to_string(user.getAge()) + "\n");
    print("Gender(F/M):" + user.getGender() + "\n");
    print("Height(cm): " + std::to_string(int(user.getHeight())) + "\n");
    print("Weight(kg): " + std::to_string(int(user.getWeight())) + "\n");
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << user.calculateBMI();
    print("BMI: " + ss.str() + "\n");
    // 体重状态
    std::string status = "";
    double bmi = user.calculateBMI();
    if (bmi < 18.5) status = "Underweight";
    else if (bmi < 24) status = "Normal";
    else if (bmi < 28) status = "Overweight";
    else status = "Obese";
    print("Physical Status: " + status + "\n\n");
    // 营养摄入
    print("Nutritional Intake Profile (daily recommendation in parentheses) :\n");
    double totalCalories = calculateTotalCalories(records, date);
    double recommendedCalories = user.getRecommendedCalories();
    ss.str("");
    ss << int(totalCalories) << "(" << int(recommendedCalories) << "kcal)";
    print("Food Energy Intake: " + ss.str() + "\n");
    auto intake = calculateDailyIntake(records, date);
    // 主食
    print("Staple Food: " + std::to_string(int(intake[FoodCategory::StapleFood])) + " (200-300g)\n");
    // 肉禽蛋
    print("Animal Proteins: " + std::to_string(int(intake[FoodCategory::AnimalProteins])) + " (120-200g)\n");
    // 蔬菜
    print("Vegetables: " + std::to_string(int(intake[FoodCategory::Vegetables])) + " (300-500g)\n");
    // 水果
    print("Fruits: " + std::to_string(int(intake[FoodCategory::Fruits])) + " (200-350g)\n");
    // 奶制品 Dairy
    double dairy = 0;
    for (const auto& record : records) {
        if (record.date == date && record.category == FoodCategory::Beverages &&
            (record.foodName == "Milk" || record.foodName == "Yogurt" || record.foodName == "milk" || record.foodName == "yogurt")) {
            dairy += record.weight;
        }
    }
    print("Diary: " + std::to_string(int(dairy)) + " (300g)\n");
    print("============================================================\n");
    report.close();
}

void DietRecord::generateDailyReport(const User& user, const std::vector<DietRecord>& records) {
    generateReport(user, records, Date::getCurrentDate());
}

void DietRecord::queryRecordsByDate(const std::vector<DietRecord>& records) {
    std::cout << "Enter date (yyyy/mm/dd): ";
    std::string queryDateStr;
    std::getline(std::cin, queryDateStr);

    try {
        Date queryDate(queryDateStr);
        std::vector<const DietRecord*> dayRecords;
        for (const auto& record : records) {
            if (record.getDate() == queryDate) { // 直接比较日期对象更可靠
                dayRecords.push_back(&record);
            }
        }
        if (dayRecords.empty()) {
            std::cout << "No dietary records found for " << queryDateStr << ", in the database." << std::endl;
            return;
        }

        // 英文表头
        std::cout << std::left
                  << std::setw(12) << "Date"
                  << std::setw(18) << "Category"
                  << std::setw(18) << "Name"
                  << std::setw(10) << "Weight (g)"
                  << std::setw(15) << "Calories (kcal)" << std::endl;
        
        for (const auto* rec : dayRecords) {
            std::cout << std::left
                      << std::setw(12) << queryDateStr // 使用原始输入格式
                      << std::setw(18) << Food::categoryToString(rec->getCategory())
                      << std::setw(18) << rec->getFoodName()
                      << std::setw(10) << static_cast<int>(rec->getWeight())
                      << std::setw(15) << static_cast<int>(rec->getCalories())
                      << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "Invalid date format." << std::endl;
    }
}
