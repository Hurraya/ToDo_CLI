#define NOMINMAX    // 把windows定义的宏干掉
#include <iostream>
#include <windows.h> 
#include <string>
#include <limits>
#include <functional>
#include <map>
#include "TodoList.h"


void printMenu();
void init();
int getSafeInt();
std::chrono::system_clock::time_point parsedate(const std::string& dateStr);
void handleAddTasks(TodoList& list);
void handleListTasks(TodoList& list);
void handleMarkDone(TodoList& list);
void handleRemove(TodoList& list);
void handleSearch(TodoList& list);

int main() {
    SetConsoleOutputCP(CP_UTF8);
    init(); 

    return 0;
}

void printMenu() {
    std::cout << "\n=== Todo List ===\n";
    std::cout << "1.添加任务\n"; //add
    std::cout << "2.列出任务\n"; //list
    std::cout << "3.标记完成\n"; //markDone
    std::cout << "4.删除任务\n"; //remove
    std::cout << "5.搜索任务\n"; //search
    std::cout << "6.退出\n";  
    std::cout << "请输入指令(1-6)：";
}

void init() {
    TodoList list; 
    list.loadFromFile("todo.txt"); // 启动时加载

    // 核心重构：建立字典命令！
    // key是数字，value是不需要返回值的 lambda
    std::map<int, std::function<void()>> commands;

    // 把函数和lambda绑定在数字上
    commands[1] = [&list]() {handleAddTasks(list);};
    commands[2] = [&list]() {handleListTasks(list);};
    commands[3] = [&list]() {handleMarkDone(list);};
    commands[4] = [&list]() {handleRemove(list);};
    commands[5] = [&list]() {handleSearch(list);};
    commands[6] = [&list]() {
        list.saveToFile("todo.txt"); // 退出时保存
        std::cout << "再见！\n";
        exit(0);
        };

    while (true) {
        printMenu();
        
        int choice;
        choice = getSafeInt();

        // 重构精髓：查表
        auto it = commands.find(choice);
        if (it != commands.end()) {
            it->second();
        }
        else {
            std::cout << "无效输入，请重新输入。\n";
        }
        
    }
}

// 安全读入整形，防止用户输入字母导致死循环
int getSafeInt() {
    int input;
    while (true) {
        std::cin >> input;
        if (std::cin.fail()) {
            std::cin.clear();   // 清除错误标志
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');         //清空缓冲区
            std::cout << "输入格式错误，请输入数字：";
        }
        else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 丢弃回车，不要妨碍下次getline
            return input;
        }
    }
}
// 解析 “YYYY-MM-DD”字符串，返回time_pointer
std::chrono::system_clock::time_point parsedate(const std::string& dateStr) {
    std::tm tm = {};
    std::istringstream ss(dateStr);
    ss >> std::get_time(&tm, "%Y-%m-%d");       // 解析格式

    if (ss.fail()) {
        return std::chrono::system_clock::time_point(); // 解析失败，返回默认值

    }
    // mktime 将tm 转为 time_t,再转为 time_pointer
    std::time_t t = std::mktime(&tm);
    return std::chrono::system_clock::from_time_t(t);
}

// 抽离出独立的命令函数

void handleAddTasks(TodoList& list) {

    std::string titile;
    int priority;
    std::chrono::system_clock::time_point deadline;

    std::cout << "请输入任务的名称：";
    // std::cin.ignore();  // 清除输入缓冲区的换行符.非常重要，因为输入1按回车后，回车还在缓冲区，如果不ignore，后面的getline会直接读到一个空字符串
    std::getline(std::cin, titile);

    std::cout << "请输入优先级（1高，2中，3低）：";
    priority = getSafeInt();

    std::cout << "请输入截日期（格式：YYYY-mm-dd）";
    std::string tmp;
    std::cin >> tmp;
    deadline = parsedate(tmp);

    list.add(titile, priority, deadline);
}

void handleListTasks(TodoList& list) {
    list.list();
}

void handleMarkDone(TodoList& list) {
    int id;
    std::cout << "请输入要完成的ID：";
    id = getSafeInt();
    list.markDone(id);

}

void handleRemove(TodoList& list) {
    int id;
    std::cout << "请输入要删除的ID：";

    id = getSafeInt();
    list.remove(id);
}

void handleSearch(TodoList& list) {
    std::string keyword;
    std::cout << "请输出要搜索的关键词：";
    // std::cin >> keyword;

    std::getline(std::cin, keyword);
    list.search(keyword);

}