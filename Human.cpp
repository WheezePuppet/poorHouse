//This will be the Human class definitions
#include<cstdlib>
#include<iostream>
#include"Human.h"
#include"Commodity.h"
#include<Random.h>
using namespace repast;

Human::Human()
{
	srand(time(0));
	salary=(rand()%7);
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

/*Human::Human(int gero)
{
	
}*/

void Human::earnIncome()
{
	//savings+=(salary+(savings*rr))*mps;
	commoditiesHeld[producedCommodity]+=salary;
}

void Human::consume(float cons)
{
	//cons.push_back(salary+(savings*rr))*(1-mps);
	for(int i=0; i<10; i++)
	{
		commoditiesHeld[i]-=getCommNum(i).getAmtCons();
		getCommNum(i).consume();
	}
}

void considerDeath()
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
