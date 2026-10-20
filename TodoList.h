#pragma once
#include<vector>
#include<string>
#include"Task.h"

class TodoList {
private:
	std::vector<Task> tasks;  
	int nextID = 1;		// 用于自动分配ID

public:
	void add(const std::string& title, int priority, std::chrono::system_clock::time_point deadline);
	void list() const;
	void markDone(int id);
	void remove(int id);

	// 文件读写
	void saveToFile(const std::string& filename) const;
	void loadFromFile(const std::string& filename);
};