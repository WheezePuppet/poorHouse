//This will be the Human class definitions
#include<cstdlib>
#include<iostream>
#include<ostream>
#include"Human.h"
#include"Model.h"
#include"Commodity.h"
#include<Random.h>
#include<AgentId.h>

using namespace std; 
int Human::nextAgentNum = 0;

void Human::step() {
    cout << *this;
}

ostream & operator<<(ostream & os, const Human &h) {
    os << h.myId << 
        " Salary " << h.salary << " Make " << h.producedCommodity << std::endl;
	os << "[";
	for(int i=0; i<10; i++)
		{
			os << h.commoditiesHeld[i] << ", ";
		}
		os << "]" << endl;
    return os;
}

Human::Human()
{
    myId = repast::AgentId(nextAgentNum++,0,0); 
	producedCommodity=Model::instance()->generateMake();
	salary=Model::instance()->generateSalary();
	mps=Model::instance()->generateMps();

	//Random age for first generation, not for children
	age=0;//(rand()%20)+20;
	//Random initial savings?
	for(int i=0; i<10; i++)
	{
		minThreshold[i]=Model::instance()->generateNeedCommodityThreshold();	
		maxThreshold[i]=Model::instance()->generateWantCommodityThreshold();
		commoditiesHeld[i]=0;
	}
}

Human::~Human() {

}

repast::AgentId & Human::getId() { 
    return myId;
}   

const repast::AgentId & Human::getId() const {
    return myId;
}


/*Human::Human(int gero)
{
	
}*/

void Human::earnIncome()
{
	//savings+=(salary+(savings*rr))*mps;
	commoditiesHeld[producedCommodity]+=salary;
}

void Human::consume()
{
	//cons.push_back(salary+(savings*rr))*(1-mps);
	for(int i=0; i<Commodity::NUM_COMM; i++)
	{
		commoditiesHeld[i]-=Commodity::getCommNum(i).getAmtCons();
		Commodity::getCommNum(i).consume();
	}
}

void Human::considerDeath()
{
	srand(time(0));
	if(age>30)
	{
		int prob=rand()%100;
		if(prob<4)
		{
			
		}
	}
}

void Human::considerHavingAChild()
{
	srand(time(0));
	if(age>20 && age<30)
	{
		int prob=rand()%100;
		if(prob>40)
		{
			//Create children by calling child constructor
		}
	}
}
