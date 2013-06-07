//This will be the Human class definitions
#include<cstdlib>
#include<iostream>
#include<ostream>
#include<algorithm>
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


int Human::findNextDeficientCommodityStartingAt(int x)
{
    for(int i=x; i<Commodity::NUM_COMM; i++)
    {
        if(commoditiesHeld[i]<minThreshold[i])
        {
            return i;
        }
    }
    return -1;
}

CommodityStatus Human::checkStatus(int commodityNum)
{
    if(commoditiesHeld[commodityNum]<minThreshold[commodityNum])
    {
        return DEFICIENT;
    }
    if(commoditiesHeld[commodityNum]>maxThreshold[commodityNum])
    {
        return BLOATED;
    }
    return SATISFIED;
}

void Human::swap(double x, Human& other, int alow, int blow)
{
    commoditiesHeld[alow]+=x;
    other.commoditiesHeld[alow]-=x;
    commoditiesHeld[blow]-=x;
    other.commoditiesHeld[blow]+=x;
}


void Human::trade(int comm1Num, int comm2Num,
    double amtAWillingToBuyOf1, double amtBWillingToBuyOf2,
    double amtAWillingToSellOf2, double amtBWillingToSellOf1,
    Human & B) {

    double coms[4];
    coms[0] = amtAWillingToBuyOf1;
    coms[1] = amtBWillingToSellOf1;
    coms[2] = amtBWillingToBuyOf2;
    coms[3] = amtAWillingToSellOf2;

    double* x = std::min_element(coms, coms+4);
    double change=*x;
    swap(change, B, comm1Num, comm2Num);
}

void Human::transactWith(Human& other)
{
    // 1. Make super-satisfiable trades where possible.
    for(int i=0; i<Commodity::NUM_COMM; i++)
    {
        int alow= findNextDeficientCommodityStartingAt(i);
        if(other.checkStatus(alow)==BLOATED)
        {
            for(int j=0; j<Commodity::NUM_COMM; j++)
            {
                int blow=other.findNextDeficientCommodityStartingAt(j);
                if( checkStatus(blow)==BLOATED)
                {
                    // Do actual trade! A has an excess of blow and B has an
                    // excess of alow, so trade those.
                    trade(alow,
                          blow,
                          minThreshold[alow]- commoditiesHeld[alow],
                          other.commoditiesHeld[alow]-other.maxThreshold[alow],
                          other.minThreshold[blow]-other.commoditiesHeld[blow],
                          commoditiesHeld[blow]- maxThreshold[blow],
                          other);
                }
                j=blow;
            }
        }
        i=alow;
    }

    // 2. Make half-super-satisfiable trades where possible.
    for(int i=0; i<Commodity::NUM_COMM; i++)
    {
        int alow= findNextDeficientCommodityStartingAt(i);
        
        if(other.checkStatus(alow)==SATISFIED)
        {
            for(int j=0; j<Commodity::NUM_COMM; j++)
            {
                int blow=other.findNextDeficientCommodityStartingAt(j);
                
                if(checkStatus(blow)==SATISFIED)
                {
                    trade(alow,
                          blow,
                          minThreshold[alow]- commoditiesHeld[alow],
                          other.commoditiesHeld[alow]-other.minThreshold[alow],
                          other.minThreshold[blow]-other.commoditiesHeld[blow],
                          commoditiesHeld[blow]- minThreshold[blow],
                          other);
                }
                j=blow;
            }
        }
        i=alow;
    }

    // 3. Future: make other kinds of half-super-satisfiable trades where
    // possible.
    // Future: make ordinary-satisfiable trades happen.
}

