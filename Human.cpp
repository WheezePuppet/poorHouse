//This will be the Human class definitions
#include<cstdlib>
#include<iostream>
#include"Human.h"
#include"Model.h"
#include"Commodity.h"
#include<Random.h>
#include<AgentId.h>

using namespace std; 
int Human::nextAgentNum = 0;

void Human::step() {
    std::cout << "my agent number is " << myId << 
        " and by the way my salary is $" << salary << std::endl;
}

Human::Human()
{
cout << "human constructor a" << endl;
    myId = repast::AgentId(nextAgentNum++,0,0); 
cout << "human constructor b" << endl;

	salary=Model::instance()->generateSalary();
cout << "human constructor c" << endl;
	int temp=rand()%100;
cout << "human constructor d" << endl;
	mps=(temp/100.0)-.1;
cout << "human constructor e" << endl;

	//Random age for first generation, not for children
	age=0;//(rand()%20)+20;
cout << "human constructor f" << endl;
	//Random initial savings?
	for(int i=0; i<10; i++)
	{
cout << "human constructor g" << endl;
		minThreshold[i]=(rand()%5)+1.0;	
cout << "human constructor h" << endl;
		maxThreshold[i]=(rand()%5)+5.0;
cout << "human constructor i" << endl;
		commoditiesHeld[i]=0;
	}
cout << "human constructor j" << endl;
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
