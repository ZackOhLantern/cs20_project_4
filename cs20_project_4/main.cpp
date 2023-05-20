// DEFINE YOUR RUN
#define RUN01 //RUN02

#ifdef RUN01
#include"studentinfo.h"
#include<string>
using std::string;
#include <iostream>
#include"stack.h"
#include"queue.h"

int main() {



	Stack<std::string> stack;
	Queue<std::string> queue;

	stack.push(StudentInfo::name());
	stack.push("Feynman");
	stack.push("Turing");
	stack.push("Einstein");
	stack.push("Bohr");
	stack.push("Sanchez");

	stack.print();
	std::cout << std::endl;

	queue.enqueue(stack.peek());
	stack.pop();
	std::cout << std::endl;

	queue.enqueue(stack.peek());
	stack.pop();
	std::cout << std::endl;

	queue.enqueue(stack.peek());
	stack.pop();
	std::cout << std::endl;

	queue.enqueue(stack.peek());
	stack.pop();
	std::cout << std::endl;

	queue.dequeue();
	queue.dequeue();
	std::cout << std::endl;

	queue.print();
	std::cout << std::endl;

	return 0;
}

#elif defined(RUN02) //Test the game

#include"game.h"

int main() {


	//Setup game
	Game g("settings.ini");

	//Exectute game loop
	g.run();

	return 0;
}



#endif
