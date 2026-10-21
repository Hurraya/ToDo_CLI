#pragma once
#include <string>
#include <iostream>
// #include<cstdint>	uint8_t
#include<chrono>

class Task {
	int id;  // 我考虑id不可能是一个负数，并且它不会超过2的16次方，(uint16_t)是微软内部的类型吗？
	std::string title;
	int priority;	// 1高，2中，3低
	bool completed;

	std::chrono::system_clock::time_point deadline;		//sys_clock的时间点表示

public:
	Task(int id, const std::string& title, int priority, std::chrono::system_clock::time_point deadline = std::chrono::system_clock::time_point());
	bool isCompleted() const;
	int getID() const;
	const std::string& getTitle() const;
	int getPriority() const;

	// 获取剩余天数
	int getDaysRemaining() const;
	// 获取格式化截至字符串
	std::string getDeadlineString() const;
	std::chrono::system_clock::time_point getDeadline() const;

	void markDone();



	// 方便打印Task
	friend std::ostream& operator<<(std::ostream& os, const Task& task);
};