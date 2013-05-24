//This will be the human class definitions
#include<iostream>
#include"human.h"
using namespace std;

human::human()
{
	srand(time(0));
	salary=(rand()%6)*100;
	mps=(rand()%100)/100;
	//Random age for first generation, not for children
	age=(rand()%20)+20;
	//Random initial savings?
}

human::human(int gero)
{
	
}

void human::earnIncome(float rr)
{
	savings+=(salary+(savings*rr))*mps;
}

void human::consume()
{
	cons.push_back(salary+(savings*rr))*(1-mps);
}

void die()
{
	
}

void haveChildren()
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
