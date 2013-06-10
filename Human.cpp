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
#include<SharedContext.h>

//using namespace std; 

void Human::tradeWithRandomAgents()
{
	std::vector<Human*> tradingPartners;
	Model::instance()->getActors().selectAgents(repast::SharedContext<Human>::LOCAL, 50, tradingPartners, false, 100);
	for(int i=0; i<50; i++)
	{
		transactWith(*tradingPartners[i]);
	//	std::cout<<"I have traded with "<<i<<" people\n";
	}
}

int Human::nextAgentNum = 0;

void Human::step() {
	earnIncome();
	tradeWithRandomAgents();
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
		while(minThreshold[i]<Commodity::getCommNum(i).getAmtCons())
		{
			minThreshold[i]=Model::instance()->generateNeedCommodityThreshold();	
		}
		maxThreshold[i]=Model::instance()->generateWantCommodityThreshold();
		commoditiesHeld[i]=0;
	}
	commoditiesHeld[producedCommodity]=salary;
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
    if(commoditiesHeld[commodityNum]<=minThreshold[commodityNum])
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
	std::cout<<"Traded "<<change<<" "<<std::endl;
    swap(change, B, comm1Num, comm2Num);
}

/*bool Human::middle()
{
	for(int i=0; i<NUM_COMM; i++)
	{
		if(commoditiesHeld[i]<minThreshold[i])
		{
			return false;
		}
	}
	return true;
}*/

void Human::transactWith(Human& other)
{
    // 1. Make super-satisfiable trades where possible.
	int check=0;
    for(int i=0; i<Commodity::NUM_COMM; i++)
    {
        int alow= findNextDeficientCommodityStartingAt(i);
        if(alow!=-1 && other.checkStatus(alow)==BLOATED)
        {
			//std::cout<<"Found compatible low\n";
            for(int j=0; j<Commodity::NUM_COMM; j++)
            {
                int blow=other.findNextDeficientCommodityStartingAt(j);
				if(blow==-1)
				{
					continue;
				}
                if( checkStatus(blow)==BLOATED)
                {
                    // Do actual trade! A has an excess of blow and B has an
                    // excess of alow, so trade those.
					std::cout<<"Perfect: Sold good "<< blow <<" and bought good "<<alow<<" ";
                    trade(alow,
                          blow,
                          minThreshold[alow]- commoditiesHeld[alow],
                          other.commoditiesHeld[alow]-other.maxThreshold[alow],
                          other.minThreshold[blow]-other.commoditiesHeld[blow],
                          commoditiesHeld[blow]- maxThreshold[blow],
                          other);
                }
				/*std::cout<<"j is "<<j<<std::endl;
				if(blow<9)
				{
					j=blow;
				}
				std::cout<<check<<std::endl;
				check++;
				if(check>12)
				{
					exit(1);
				}*/
            }
        }
		/*std::cout<<i<<std::endl;
		if(alow<9)
		{
			i=alow;
		}*/
		/*check++;
		std::cout<<check<<std::endl;
		if(check>12)
		{
			exit(1);
		}*/
    }

    // 2. Make half-super-satisfiable trades where possible.
    for(int y=0; y<Commodity::NUM_COMM; y++)
    {
        int alow= findNextDeficientCommodityStartingAt(y);
        if(alow!=-1 && other.checkStatus(alow)==SATISFIED)
        {
			//std::cout<<"Found half compatible low\n";
            for(int g=0; g<Commodity::NUM_COMM; g++)
            {
                int blow=other.findNextDeficientCommodityStartingAt(g);
                if(blow==-1)
				{
					continue;
				}
                if(checkStatus(blow)==SATISFIED)
                {
					std::cout<<"Satisfiable: Sold good "<< blow <<" and bought good "<<alow<<" ";
                    trade(alow,
                          blow,
                          minThreshold[alow]- commoditiesHeld[alow],
                          other.commoditiesHeld[alow]-other.minThreshold[alow],
                          other.minThreshold[blow]-other.commoditiesHeld[blow],
                          commoditiesHeld[blow]- minThreshold[blow],
                          other);
                }
                //g=blow;
            }
        }
        //y=alow;
    }
/*	if(middle() && other.middle())
	{
		
	}*/
    // 3. Future: make other kinds of half-super-satisfiable trades where
    // possible.
    // Future: make ordinary-satisfiable trades happen.
}

