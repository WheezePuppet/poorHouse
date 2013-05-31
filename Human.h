#ifndef HUMAN_H
#define HUMAN_H
using namespace repast;
#include<iostream>
#include<Random.h>

class Human
{
public:
	void step();
	Human();
	//Human(int);
	void earnIncome();
	void consume(float *);
	void considerHavingAChild();
	void considerDeath();
	~Human();
private:
	float mps;//Float less than one
	float salary;//Between 3 and 7
	int producedCommodity;//Between 0 and 10
	//vector<float> cons;
	//float savings;
	float commoditiesHeld [10];
	float minThreshold [10];//Between 0 and 5
	float maxThreshold [10];//Between 6 and 10
	int age;
	Human *parent;
	vector<Human *> children;
};
#endif
