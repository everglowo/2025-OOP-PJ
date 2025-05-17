#include "user.h"
#include "food.h"
#include "diet_record.h"
#include "menu.h"
#include <iostream>
#include <vector>
#include <fstream>

int main() {
    // 判断是否首次启动
    bool isFirstRun = false;
    std::ifstream f("user.csv");
    if (!f.good()) isFirstRun = true;
    f.close();

    User user;
    
    if (isFirstRun) {
        user.initializeUser();
    } else {
        User::showWelcomeScreen(user.getName());
    }

    // 加载数据
    std::vector<Food> foods;
    Food::loadFromFile(foods);
    std::vector<DietRecord> records;
    DietRecord::loadFromFile(records);

    // 启动主循环
    Menu::runMainLoop(user, foods, records);
    return 0;
}