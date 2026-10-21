#include<iostream>
#include<cassert>	// C++自带的断言宏
#include<vector>
#include<string>
#include"Task.h"
#include"TodoList.h"

// 用一个简单的宏来模拟测试框架

#define TEST_ASSERT(condition, message)\
	if(!(condition)){ \
		std::cerr<< "❌ 测试失败：" << message << "\n";\
		return;\
	}


void testTaskCreation() {
	Task t(1, "Test Task", 1);
	TEST_ASSERT(t.getID() == 1, "ID 应该是1");
	TEST_ASSERT(t.getTitle() == "Test Task", "标题不匹配");
	TEST_ASSERT(t.isCompleted() == false, "默认应该是未完成");

	std::cout << "✅ testTaskCreation 通过！\n";
}

void testTaskDone() {
	Task t{ 1, "Test Task", 1 };
	t.markDone();
	TEST_ASSERT(t.isCompleted(), "标记完应该是已完成了");
	std::cout << "✅ testTaskDone 通过！\n";
}

void testTodoListAddAndRemove() {
	TodoList list;
	list.add("Task 1", 1, std::chrono::system_clock::now());
	list.add("Task 2", 2, std::chrono::system_clock::now());

	TEST_ASSERT(list.getSize() == 2, "添加完，应该要有两个task");

	list.remove(1);
	TEST_ASSERT(list.getSize() == 1, "删除完，应该只有1个task");

	std::cout << "✅ testAddandRemove 通过！";
}

void runAllTests() {
	std::cout << "\n--- 开始单元测试 ---\n";
	testTaskCreation();
	testTaskDone();
	testTodoListAddAndRemove();
	std::cout << "\n--- 所有测试完成 ---\n";
}