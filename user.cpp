#include "user.h"
#include <iostream>
#include <iomanip>
#include <sstream>

User::User() {
    loadFromFile();
}

User::~User() {
    saveToFile();
}

bool User::saveToFile() {
    std::ofstream file("user.csv");
    if (!file.is_open()) {
        return false;
    }
    file << "Name,Age,Gender,Height,Weight\n";
    file << name << "," << age << "," << gender << "," << height << "," << weight;
    file.close();
    return true;
}

bool User::loadFromFile() {
    std::ifstream file("user.csv");
    if (!file.is_open()) {
        return false;
    }
    std::string line;
    std::getline(file, line);
    if (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string item;
        
        std::getline(ss, name, ',');
        std::getline(ss, item, ','); age = std::stoi(item);
        std::getline(ss, gender, ',');
        std::getline(ss, item, ','); height = std::stod(item);
        std::getline(ss, item, ','); weight = std::stod(item);
        
        file.close();
        return true;
    }
    file.close();
    return false;
}

bool User::initializeUser() {
    std::cout << "**********************************" << std::endl;
    std::cout << "*                                *" << std::endl;
    std::cout << "* Welcome to Calorie Track Daily *" << std::endl;
    std::cout << "*                                *" << std::endl;
    std::cout << "**********************************" << std::endl;
    std::cout << "Enter your personal data:" << std::endl;

    std::cout << "Name: ";
    std::getline(std::cin, name);

    std::cout << "Age: ";
    std::cin >> age;
    std::cin.ignore();

    std::cout << "Gender(F/M): ";
    std::getline(std::cin, gender);

    std::cout << "Height(cm): ";
    std::cin >> height;
    std::cin.ignore();

    std::cout << "Weight(kg): ";
    std::cin >> weight;
    std::cin.ignore();

    std::cout << "User record saved." << std::endl;
    std::cout << std::endl;
    return saveToFile();
}

double User::calculateBMI() const {
    return weight / ((height / 100.0) * (height / 100.0));
}

std::string User::getBMICategory() const {
    double bmi = calculateBMI();
    if (bmi < 18.5) return "过轻(Underweight)";
    if (bmi < 24) return "正常(Normal)";
    if (bmi < 28) return "超重(Overweight)";
    return "肥胖(Obesity)";
}

double User::calculateBMR() const {
    double bmr;
    if (gender == "M") {
        bmr = 10 * weight + 6.25 * height - 5 * age + 5;
    } else {
        bmr = 10 * weight + 6.25 * height - 5 * age - 161;
    }
    return bmr * 1.2; // 轻度活动系数
}

double User::getRecommendedCalories() const {
    return calculateBMR();
}



void User::configureProfile() {
    while (true) {
        std::cout<<std::endl;
        std::cout << "1. Name  2. Age  3. Gender  4. Height  5. Weight  6. Exit" << std::endl;
        std::cout << "Enter command (1-6): ";
        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            std::cout << "Name: ";
            std::string n;
            std::getline(std::cin, n);
            setName(n);
        }
        else if (choice == "2") {
            std::cout << "Age: ";
            std::string ageStr;
            std::getline(std::cin, ageStr);
            try {
                int a = std::stoi(ageStr);
                setAge(a);
            } catch (...) {}
        }
        else if (choice == "3") {
            std::cout << "Gender(F/M): ";
            std::string g;
            std::getline(std::cin, g);
            setGender(g);
        }
        else if (choice == "4") {
            std::cout << "Height(cm): ";
            std::string hStr;
            std::getline(std::cin, hStr);
            try {
                double h = std::stod(hStr);
                setHeight(h);
            } catch (...) {}
        }
        else if (choice == "5") {
            std::cout << "Weight(kg): ";
            std::string wStr;
            std::getline(std::cin, wStr);
            try {
                double w = std::stod(wStr);
                setWeight(w);
            } catch (...) {}
        }
        else if (choice == "6") {
            std::cout << "User database saved." << std::endl;
            saveToFile();
            break;
        }
        else {
            std::cout << "Invalid choice." << std::endl;
            break;
        }
    }
}

void User::showWelcomeScreen(const std::string& userName) {
    std::cout << "**********************************" << std::endl;
    std::cout << "*                                *" << std::endl;
    std::cout << "* Welcome to Calorie Track Daily *" << std::endl;
    std::cout << "*                                *" << std::endl;
    std::cout << "**********************************" << std::endl;

    // 显示用户名和当前时间
    time_t now = time(nullptr);
    char buf[64];
    strftime(buf, sizeof(buf), "%a %b %d %H:%M:%S %Y", localtime(&now));
    std::cout << "Hi " << userName << "! Current time : " << buf << std::endl;
} 