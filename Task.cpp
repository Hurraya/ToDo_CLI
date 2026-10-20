#define _CRT_SECURE_NO_WARNINGS // 让VS不报localtime的错
#include"Task.h"
#include<iomanip>  // 用于std::put_time
#include<sstream>  // 用于std::ostringstream
#include<ctime>		//用于std::localtime

Task::Task(int id, const std::string& title, int priority, std::chrono::system_clock::time_point deadline) 
	: id(id), title(title), priority(priority), completed(false), deadline(deadline) {}// 默认未完成

bool  Task::isCompleted() const {
	return completed;
}

int Task::getID() const {
	return id;
}

const std::string& Task::getTitle() const{
	return title;
}

int Task::getPriority() const {
	return priority;
}

void Task::markDone() {
	completed = true;
}

int Task::getDaysRemaining() const {
	// 如果没有设置截至日期默认最小值
	if (deadline == std::chrono::system_clock::time_point()) {
		return 9999;
	}

	auto now = std::chrono::system_clock::now();  // 静态函数now
	auto diff = deadline - now;
	// 将时长转化为天数（可能为负数表示逾期）
	return static_cast<int>(std::chrono::duration_cast<std::chrono::hours>(diff).count() / 24);
}

std::string Task::getDeadlineString() const {
	if (deadline == std::chrono::system_clock::time_point()) {
		return "无";
	}

	// time_point 转 time_t
	// 这里可能会出问题，localtime可能会溢出，返回一个空指针
	std::time_t t = std::chrono::system_clock::to_time_t(deadline);
	std::tm* tmPtr = std::localtime(&t);    // 将time_t转换成tm结构（返回指针）

	//防御
	if (tmPtr == nullptr) {
		return "未知";
	}

	std::ostringstream oss;
	oss << std::put_time(tmPtr, "%Y-%m-%d");
	return oss.str();
}

std::chrono::system_clock::time_point Task::getDeadline() const{
	return deadline;
}

std::ostream& operator<<(std::ostream& os, const Task& task) {
	int days = task.getDaysRemaining();
	// 先设置颜色
	if (task.isCompleted()) {
		os << "\033[90m"; // 灰色
	}
	else
	{
		if (days > -9999) {		// 将默认情况1970年隔离出去
			if (days < 0 ) {
				os << "\033[31m"; // 红色
				
			}
			else if (days <= 3) {
				os << "\33[33m"; // 黄色
				
			}
			else {
				os << "\033[32m";  // 绿色
			}
		}
		else {
			os << "\033[0m"; // 默认颜色
		}
	}
	
	// 打印任务主体
	os << "[" << task.getID() << "] ";

	if (task.isCompleted()) {
		os << "[√]";
	}
	else {
		os << "[ ]";
	}

	os << task.getTitle() << "(优先级：" << task.getPriority() << ")";

	// 打印时间
	if (days > -9999 && !task.isCompleted()) {
		os << " | 截至： " << task.getDeadlineString();
		if (days < 0) {
			os << "（⚠ 已逾期" << -days << "天）";
		}
		else if (days <= 3) {
			os << "（⚠ 仅剩 " << days << "天）";
		}
	}

	// 重置颜色，不影响后面的输出
	os << "\033[0m";
	
	

	return os;
}

