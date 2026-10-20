#include"Task.h"

Task::Task(int id, const std::string& title, int priority) 
	: id(id), title(title), priority(priority), completed(false) {}// 默认未完成

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

std::ostream& operator<<(std::ostream& os, const Task& task) {
	os << "[" << task.getID() << "] ";
	if (task.isCompleted()) {
		os << "[√]";
	}
	else
	{
		os << "[×]";
	}
	
	os << task.getTitle() << "(优先级：" << task.getPriority() << ")";

	return os;
}

