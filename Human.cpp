//This will be the Human class definitions
#include<cstdlib>
#include<iostream>
#include<ostream>
#include"Human.h"
#include"Model.h"
#include"Commodity.h"
#include<Random.h>
#include<AgentId.h>

//using namespace std; 
int Human::nextAgentNum = 0;

void Human::step() {
	earnIncome();
	//transact
	consume();
    std::cout << *this;
}

std::ostream & operator<<(std::ostream & os, const Human &h) {
    os << h.myId << 
        " Salary " << h.salary << " Make " << h.producedCommodity << " mps " << h.mps << std::endl << "[";
	for(int i=0; i<Commodity::NUM_COMM; i++)
		{
			os << h.commoditiesHeld[i] << ", ";
		}
		os << "]" << std::endl;
	for(int i=0; i<Commodity::NUM_COMM; i++)
		{
			os << h.minThreshold[i] << ", ";
		}
		os << "]" << std::endl;
	for(int i=0; i<Commodity::NUM_COMM; i++)
		{
			os << h.maxThreshold[i] << ", ";
		}
		os << "]" << std::endl << std::endl;
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
		if(commoditiesHeld[i]-Commodity::getCommNum(i).getAmtCons()>=0)
		{	
			commoditiesHeld[i]-=Commodity::getCommNum(i).getAmtCons();	
			Commodity::getCommNum(i).consume();
		}
		else
		{
			Commodity::getCommNum(i).consFail(Commodity::getCommNum(i).getAmtCons()-commoditiesHeld[i]);
			commoditiesHeld[i]=0;
		}
		
		//Commodity::getCommNum(i).consume();
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
