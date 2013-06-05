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
    os << "ID " << h.myId << 
        " Salary " << h.salary << " Make " << h.producedCommodity << std::endl;
    return os;
}

Human::Human()
{
    myId = repast::AgentId(nextAgentNum++,0,0); 
	producedCommodity=Model::instance()->generateMake();
	salary=Model::instance()->generateSalary();
	while(salary<0)
	{
		salary=Model::instance()->generateSalary();
	}
	int temp=rand()%100;
	mps=(temp/100.0)-.1;

	//Random age for first generation, not for children
	age=0;//(rand()%20)+20;
	//Random initial savings?
	for(int i=0; i<10; i++)
	{
		minThreshold[i]=(rand()%5)+1.0;	
		maxThreshold[i]=(rand()%5)+5.0;
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
