#include"TodoList.h"
#include<algorithm>
#include<fstream>
#include <sstream>
#include<cctype>


void TodoList::add(const std::string& title, int priority, std::chrono::system_clock::time_point deadline) {
	tasks.emplace_back(nextID, title, priority, deadline);  // 直接在vector尾部构造

	nextID++;
	std::cout << "successfully added task\n";
}

void TodoList::list() const{
	// 1.如果list空，打印“NO task yet”
	// 2.如果不为空打印，遍历按照优先级打印
	
	if (tasks.empty()) {
		std::cout << "no task yet. \n";
		return;
	}
	// 拷贝一份用于排序（不破坏原顺序）
	std::vector<Task> sortTasks = tasks;
	
	// 按优先级排序
	std::sort(sortTasks.begin(), sortTasks.end(),
		[](const Task& a, const Task& b) {return a.getPriority() < b.getPriority();});

	
	for (const auto& t : sortTasks) {
		std::cout << t << "\n";
	}
}

void TodoList::markDone(int id) {

	for (auto& t : tasks) {
		if (t.getID() == id) {
			t.markDone();
			std::cout << "Task " << id << "marked as done.\n";
			return;
		}
	}

	std::cerr << "Task not found\n";
}

void TodoList::remove(int id) {

	// yong1 Erase-remove 惯用法（找出来再删掉）
	// std::remove_if(会将要删除的元素移到末尾，并返回末尾的迭代器)
	auto it = std::remove_if(tasks.begin(), tasks.end(),
		[id](const Task& t) {return t.getID() == id;});

	if (it != tasks.end()) {
		tasks.erase(it, tasks.end());  // it指向移到末尾要删除的那个元素
		std::cout << "Task " << id << "remove.\n";
		
	}
	else {
		std::cerr << "Task not Found\n";
	}
}

void TodoList::saveToFile(const std::string& filename) const {
	std::ofstream fount(filename);
	if (!fount) {
		std::cerr << "保存文件失败!\n";  //文件打开失败
		return;
	}

	for (const auto& t : tasks) {
		// 用逗号分隔写入：ID，标题，优先级，是否完成

		// 把time_point 转成 time_t（从1970年的秒数），存为整数
		std::time_t t_time = std::chrono::system_clock::to_time_t(t.getDeadline());

		fount << t.getID() << ","
			<< t.getTitle() << ","
			<< t.getPriority() << ","
			<< (t.isCompleted() ? 1 : 0) << ","
			<< t_time << "\n";
	}

	std::cout << "任务以及保存到" << filename << "\n";
	
}// 析构函数自动关闭文件

void TodoList::loadFromFile(const std::string& filename) {

	std::ifstream fin(filename);

	if (!fin) {
		// 文件不存在很正常（第一次运行），不需要报错
		return;
	}

	tasks.clear(); // 清空就数据，把内存中tasks的东西清除
	nextID = 1;	//重置ID计数.


	std::string line;
	while (std::getline(fin, line)) {
		// 使用stringstream解析line
		std::istringstream iss(line);

		int id = 0;
		std::string title;
		int priority = 1;
		bool isCompleted = false;
		std::chrono::system_clock::time_point deadline;

		std::string token;
		int round = 0;
		while (std::getline(iss, token, ',')) {
			round++;
			switch (round) {
			case 1:
				id = stoi(token);
				break;
			case 2:
				title = token;
				break;
			case 3:
				priority = stoi(token);
				break;
			case 4:
				isCompleted = stoi(token);
				break;
			case 5:
				std::time_t t = std::stoll(token);	// 时间轴可能很大，用 stoll(long long)
				deadline = std::chrono::system_clock::from_time_t(t);
				break;
			}

        }

		if (round == 5) {  // 确保读够了 5 个
			Task t(id, title, priority, deadline);
			if (isCompleted) {
				t.markDone();
			}
			tasks.emplace_back(t);

			// 维护 nextID
			if (id >= nextID) {
				nextID = id + 1;
			}

		}


	}
}

void TodoList::search(std::string& keyword)  const{
	if (tasks.empty()) {
		std::cout << "no task yet.\n";
		return;
	}

	bool found = false;
	std::cout << "\n--- 搜索结果: "<< keyword <<" ---\n";

	for (const auto& t : tasks) {
		if (constainsIgnoreCase(t.getTitle(), keyword)) {
			std::cout << t << "\n";
			found = true;
		}
	}

	if (!found) {
		std::cout << "没有找到包含 " << keyword << " 的任务。\n";
	}
}

bool constainsIgnoreCase(const std::string& str, const std::string& keyword) {

	auto it = std::search(
		str.begin(), str.end(),
		keyword.begin(), keyword.end(),
		[](char ch1, char ch2) {return std::tolower(ch1) == std::tolower(ch2);});

	return it != str.end();
}