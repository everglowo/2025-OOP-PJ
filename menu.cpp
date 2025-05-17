#include "menu.h"
#include <iostream>

Menu::Menu(const std::string& title, bool isSubMenu) 
    : title(title), isSubMenu(isSubMenu) {}

void Menu::addOption(const std::string& option, std::function<void()> action) {
    options.push_back(option);
    actions.push_back(action);
}

void Menu::display() const {
    std::cout << "\n=== " << title << " ===" << std::endl;
    for (size_t i = 0; i < options.size(); ++i) {
        std::cout << (i + 1) << ". " << options[i] << std::endl;
    }
    if (isSubMenu) {
        std::cout << (options.size() + 1) << ". 返回上级菜单" << std::endl;
    }
    std::cout << "请选择操作 (1-" << (isSubMenu ? options.size() + 1 : options.size()) << ")：";
}

void Menu::handleChoice() const {
    std::string choice;
    std::getline(std::cin, choice);
    
    try {
        int choiceNum = std::stoi(choice);
        if (choiceNum > 0 && choiceNum <= static_cast<int>(options.size())) {
            actions[choiceNum - 1]();
        }
        else if (isSubMenu && choiceNum == static_cast<int>(options.size() + 1)) {
            return; // 返回上级菜单
        }
        else {
            std::cout << "\n无效的选择，请重试。" << std::endl;
        }
    }
    catch (...) {
        std::cout << "\n无效的输入，请输入数字。" << std::endl;
    }
}

void Menu::clear() {
    options.clear();
    actions.clear();
}

void Menu::showMainMenu() {
    std::cout << "1. Configure Profile  2. Log Meals  3. Nutritional Insights  4. View Meal History  5. Exit" << std::endl;
    std::cout << "Enter command (1-5): ";
} 
void Menu::runMainLoop(User& user, std::vector<Food>& foods, std::vector<DietRecord>& records) {
    while (true) {
        std::cout << std::endl;
        showMainMenu();
        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            user.configureProfile();
        }
        else if (choice == "2") {
            DietRecord::addNewRecord(records, foods);
        }
        else if (choice == "3") {
            DietRecord::generateDailyReport(user, records);
        }
        else if (choice == "4") {
            DietRecord::queryRecordsByDate(records);
        }
        else if (choice == "5") {
            std::cout << "====================================================" << std::endl;
            std::cout << std::endl;
            std::cout << "Start Tracking ➞ Start Shining. Goodbye, Uncertainty ^_^" << std::endl;
            std::cout << std::endl;
            std::cout << "====================================================" << std::endl;
            break;
        }
        else {
            std::cout << "Invalid choice." << std::endl;
        }
    }
}