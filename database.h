#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>

template<typename T>
class Database {
private:
    std::vector<T> items;
    std::string filename;

public:
    Database(const std::string& filename) : filename(filename) {}
    
    // 添加项目
    void add(const T& item) {
        items.push_back(item);
    }
    
    // 获取所有项目
    const std::vector<T>& getAll() const {
        return items;
    }
    
    // 保存到文件
    bool save() const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        
        // 写入表头
        file << T::getHeader() << "\n";
        
        // 写入数据
        for (const auto& item : items) {
            file << item.toString() << "\n";
        }
        
        return true;
    }
    
    // 从文件加载
    bool load() {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        
        items.clear();
        std::string line;
        
        // 跳过表头
        std::getline(file, line);
        
        // 读取数据
        while (std::getline(file, line)) {
            try {
                T item(line);
                items.push_back(item);
            }
            catch (const std::exception& e) {
                std::cerr << "加载数据时出错：" << e.what() << std::endl;
                continue;
            }
        }
        
        return true;
    }
    
    // 清空数据
    void clear() {
        items.clear();
    }
    
    // 获取项目数量
    size_t size() const {
        return items.size();
    }
    
    // 检查是否为空
    bool empty() const {
        return items.empty();
    }
};

#endif // DATABASE_H 