#ifndef USER_H
#define USER_H

#include <string>
#include <fstream>
#include <ctime>

class User {
private:
    std::string name;
    int age;
    std::string gender;
    double height;  // in cm
    double weight;  // in kg

    // 私有辅助方法
    bool saveToFile();
    bool loadFromFile();
    double calculateBMR() const;

public:
    User();
    ~User();

    // 获取器
    std::string getName() const { return name; }
    int getAge() const { return age; }
    std::string getGender() const { return gender; }
    double getHeight() const { return height; }
    double getWeight() const { return weight; }

    // 设置器
    void setName(const std::string& n) { name = n; }
    void setAge(int a) { age = a; }
    void setGender(const std::string& g) { gender = g; }
    void setHeight(double h) { height = h; }
    void setWeight(double w) { weight = w; }

    // 功能方法
    bool initializeUser();
    void greetUser();
    void displayUserInfo();
    std::string getBMICategory() const;
    double getRecommendedCalories() const;
    void configureProfile();
    
    // BMI计算
    double calculateBMI() const;

    static void showWelcomeScreen(const std::string& userName);
};

#endif // USER_H 