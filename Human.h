#ifndef HUMAN_H
#define HUMAN_H
#include<iostream>
#include<ostream>
#include<AgentId.h>
#include<Random.h>
#include"Commodity.h"

class Human : public repast::Agent
{

friend std::ostream & operator<<(std::ostream & os, const Human & h);

public:
	void step();
	Human();
	//Human(int);
	void earnIncome();
	void consume();
	void considerHavingAChild();
	void considerDeath();
	~Human();
    virtual repast::AgentId & getId();
    virtual const repast::AgentId & getId() const;

private:
    repast::AgentId myId;
    static int nextAgentNum;
	float mps;//Float less than one
	float salary;//Between 3 and 7
	int producedCommodity;//Between 0 and 10
	//vector<float> cons;
	//float savings;
	float commoditiesHeld [Commodity::NUM_COMM];
	float minThreshold [Commodity::NUM_COMM];//Between 0 and 5
	float maxThreshold [Commodity::NUM_COMM];//Between 6 and 10
	int age;
	Human *parent;
    std::vector<Human *> children;
};


// std::ostream & operator<<(const Human & h, std::ostream & os) const;
#endif
