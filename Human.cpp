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


int findNextDeficientCommodityStartingAt(int x)
{
    for(int i=x; i<COMM_NUM; i++)
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

void Human::transaction(Human& other)
{
    // 1. Make great trades where possible.
    for(int i=0; i<COMM_NUMM; i++)
    {
        int alow= findNextDeficientCommodityStartingAt(i);
        if(other.checkStatus(alow)==BLOATED)
        {
            for(int j=0; j<COMM_NUM; j++)
            {
                int blow=other.findNextDeficientCommodityStartingAt(j);
                if( checkStatus(blow)==BLOATED)
                {
                    // Do actual trade! A has an excess of blow and B has an
                    // excess of alow, so trade those.
                    double coms[4];
                    // coms[0] - the amount A is willing to buy of alow.
                    coms[0]= needThreshold[alow]- commoditiesHeld[alow];
                    // coms[1] - the amount B is willing to sell of alow.
                    coms[1]=other.commoditiesHeld[alow]-other.wantThreshold[alow];
                    // coms[2] - the amount B is willing to buy of blow.
                    coms[2]=other.needThreshold[blow]-other.commoditiesHeld[blow];
                    // coms[3] - the amount A is willint to sell of blow.
                    coms[3]= commoditiesHeld[blow]- wantThreshold[blow];
                    double* x = min_element(coms, coms+4);
                    double change=*x;
                    swap(change, other, alow, blow);
                }
                j=blow;
            }
        }
        i=alow;
    }

    for(int i=0; i<COMM_NUM; i++)
    {
        int alow= findNextDeficientCommodityStartingAt(i);
        if(other.checkStatus(alow)==1)
        {
            i=alow;
        }
        if(other.checkStatus(alow)==2)
        {
            for(int j=0; j<COMM_NUM; j++)
            {
                int blow=other.findNextDeficientCommodityStartingAt(j);
                if(checkStatus(blow)==1)
                {
                    j=blow;
                }
                if(checkStatus(blow)==3)
                {
                    double coms[4];
                    coms[0]= needThreshold[alow]- commoditiesHeld[alow];
                    coms[1]=other.commoditiesHeld[alow]-other.needThreshold[alow];
                    coms[2]=other.needThreshold[blow]-other.commoditiesHeld[blow];
                    coms[3]= commoditiesHeld[blow]- needThreshold[blow];
                    double* x = min_element(coms, coms+4);
                    double change=*x;
                    swap(change, other, alow, blow);
                    j=blow;
                }
            }
            i=alow;
        }
    }
}

