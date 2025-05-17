#ifndef FOOD_H
#define FOOD_H

#include <string>
#include <vector>
#include <fstream>

// 食物类别枚举
enum class FoodCategory {
    StapleFood,      // 主食
    AnimalProteins,  // 肉禽蛋
    Vegetables,      // 蔬菜
    Fruits,          // 水果
    Beverages        // 饮料
};

class Food {
private:
    std::string name;
    double calories;  // kcal/100g
    FoodCategory category;

public:
    Food();
    Food(const std::string& name, double calories, FoodCategory category);
    
    // 获取器
    std::string getName() const { return name; }
    double getCalories() const { return calories; }
    FoodCategory getCategory() const { return category; }
    
    // 设置器
    void setName(const std::string& n) { name = n; }
    void setCalories(double c) { calories = c; }
    void setCategory(FoodCategory c) { category = c; }

    // 类别转换方法
    static std::string categoryToString(FoodCategory cat);
    static FoodCategory stringToCategory(const std::string& str);

    // 文件操作
    static bool saveToFile(const std::vector<Food>& foods);
    static bool loadFromFile(std::vector<Food>& foods);
    
};

#endif // FOOD_H