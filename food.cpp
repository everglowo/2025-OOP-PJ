#include "food.h"
#include <iostream>
#include <iomanip>
#include <sstream>

Food::Food() : calories(0), category(FoodCategory::StapleFood) {}

Food::Food(const std::string& name, double calories, FoodCategory category)
    : name(name), calories(calories), category(category) {}

std::string Food::categoryToString(FoodCategory cat) {
    switch (cat) {
        case FoodCategory::StapleFood: return "StapleFood";
        case FoodCategory::AnimalProteins: return "AnimalProteins";
        case FoodCategory::Vegetables: return "Vegetables";
        case FoodCategory::Fruits: return "Fruits";
        case FoodCategory::Beverages: return "Beverages";
        default: return "Unknown";
    }
}

FoodCategory Food::stringToCategory(const std::string& str) {
    if (str == "StapleFood") return FoodCategory::StapleFood;
    if (str == "AnimalProteins") return FoodCategory::AnimalProteins;
    if (str == "Vegetables") return FoodCategory::Vegetables;
    if (str == "Fruits") return FoodCategory::Fruits;
    if (str == "Beverages") return FoodCategory::Beverages;
    return FoodCategory::StapleFood; // 默认返回主食类别
}

bool Food::saveToFile(const std::vector<Food>& foods) {
    // 创建临时文件
    std::string tempFile = "food_database_temp.csv";
    std::ofstream file(tempFile);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to create temporary file" << std::endl;
        return false;
    }
    
    // 写入表头
    file << "Food Name,Calories(kcal/100g),Category\n";
    
    // 写入食物数据
    for (const auto& food : foods) {
        file << food.name << ","
             << food.calories << ","
             << categoryToString(food.category) << "\n";
    }
    
    file.close();
    
    // 如果原文件存在，先删除
    std::remove("food_database.csv");
    
    // 重命名临时文件
    if (std::rename(tempFile.c_str(), "food_database.csv") != 0) {
        std::cerr << "Error: Unable to save file" << std::endl;
        return false;
    }
    
    return true;
}

bool Food::loadFromFile(std::vector<Food>& foods) {
    std::ifstream file("food_database.csv");
    if (!file.is_open()) {
        return false;
    }
    
    foods.clear(); // 清空现有数据
    std::string line;
    
    // 跳过表头
    std::getline(file, line);
    
    // 读取食物数据
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string name, calories_str, category_str;
        
        std::getline(ss, name, ',');
        std::getline(ss, calories_str, ',');
        std::getline(ss, category_str, ',');
        
        Food food;
        food.name = name;
        food.calories = std::stod(calories_str);
        food.category = stringToCategory(category_str);
        
        foods.push_back(food);
    }
    
    file.close();
    return true;
}

