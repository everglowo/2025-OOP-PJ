#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>
#include <functional>
#include "user.h"
#include "food.h"
#include "diet_record.h"

class Menu {
private:
    std::string title;
    std::vector<std::string> options;
    std::vector<std::function<void()>> actions;
    bool isSubMenu;

public:
    Menu(const std::string& title, bool isSubMenu = false);
    
    void addOption(const std::string& option, std::function<void()> action);
    void display() const;
    void handleChoice() const;
    size_t getOptionCount() const { return options.size(); }
    void clear();
    static void showMainMenu();

    static void runMainLoop(User& user, std::vector<Food>& foods, std::vector<DietRecord>& records);
};

#endif // MENU_H