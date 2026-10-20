#include"TodoList.h"
#include<algorithm>
#include<fstream>
#include <sstream>


void TodoList::add(const std::string& title, int priority) {
	tasks.emplace_back(nextID, title, priority);  // 直接在vector尾部构造

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
		fount << t.getID() << ","
			<< t.getTitle() << ","
			<< t.getPriority() << ","
			<< t.isCompleted() << "\n";
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

			}

		}

		if (round == 4) {  // 确保读够了 4 个
			Task t(id, title, priority);
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
