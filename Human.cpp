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

using namespace std; 

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
            CommodityStatus cs = h.checkStatus(i);
            switch (cs) {
            case BLOATED:
                os << "*";
                break;
            case DEFICIENT:
                os << "?";
                break;
            }
			os << h.commoditiesHeld[i];
            if (i<Commodity::NUM_COMM-1) {
                os  << ", ";
            }
		}
		os << "]" << std::endl;
/*
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
*/
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


// Return the number of the commodity greater than or equal to x that we are
// deficient in. If we are not deficient in any, return NUM_COMM (which is an
// illegal commodity number.)
int Human::findNextDeficientCommodityStartingAt(int x)
{
    for(int i=x; i<Commodity::NUM_COMM; i++)
    {
        if(commoditiesHeld[i]<minThreshold[i])
        {
            return i;
        }
    }
    return Commodity::NUM_COMM;
}

CommodityStatus Human::checkStatus(int commodityNum) const
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
	std::cout<<"Traded "<<change<<" "<<std::endl;
    swap(change, B, comm1Num, comm2Num);
}

/*
bool Human::middle()
{
	for(int i=0; i<NUM_COMM; i++)
	{
		if(commoditiesHeld[i]<minThreshold[i])
		{
			return false;
		}
	}
	return true;
}
*/

void Human::transactWith(Human& other)
{
    // 1. Make super-satisfiable trades where possible.
    makeSuperSatisfiableTradesWith(other);

    // 2. Make half-super-satisfiable trades where possible.
    makeHalfSuperSatisfiableTradesWith(other);

    // 3. Future: make other kinds of half-super-satisfiable trades where
    // possible.
    // Future: make ordinary-satisfiable trades happen.
    makeOrdinarySatisfiableTradesWith(other);
}


void Human::makeSuperSatisfiableTradesWith(Human& other)
{
cout << "Looking for super trades between " << getId() << " and " <<
other.getId() << "..." << endl;
	int check=0;

    // Go through all my commodities, searching for ones I'm deficient in.
    for(int i=0; i<Commodity::NUM_COMM; i++)
    {
        i= findNextDeficientCommodityStartingAt(i);
cout << "My next deficient commodity is " << i << "..." << endl;
        if(i!=Commodity::NUM_COMM && other.checkStatus(i)==BLOATED)
        {
            // Okay, this is now true: I am deficient in commodity i, and 
            // the RHS is bloated in that commodity. So we've "halfway" found 
            // a possible super-satisfiable trade.
cout << "Verify: I am deficient in commodity " << i << ", and RHS is bloated in commodity " <<  i << ":" << endl;
cout << "Me: " << *this << endl;
cout << "RHS: " << other << endl;
        
			//std::cout<<"Found compatible low\n";
            for(int j=0; j<Commodity::NUM_COMM; j++)
            {
                j=other.findNextDeficientCommodityStartingAt(j);
				if(j==Commodity::NUM_COMM)
				{
                    // Well, bummer. The RHS is not deficient in anything,
                    // so there are no super-satisfiable trades possible.
cout << "Verify: the RHS is not deficient in anything:" << endl;
cout << "RHS: " << other << endl;
        
cout << "done!" << endl;
					return;
				}
                if(checkStatus(j)==BLOATED && checkStatus(i)==DEFICIENT)
                {
                    // Do actual trade! A has an excess of j and B has an
                    // excess of i, so trade those.
					std::cout<<"Perfect: Sold good "<< j <<" and bought good "<<i<<" ";
                    trade(i,
                          j,
                          minThreshold[i]- commoditiesHeld[i],
                          other.commoditiesHeld[i]-other.maxThreshold[i],
                          other.minThreshold[j]-other.commoditiesHeld[j],
                          commoditiesHeld[j]- maxThreshold[j],
                          other);

                    // We have now traded. However, I may still be deficient
                    // in commodity i. If so, I need to continue to look 
                    // for other j's to continue to super satisfy my i.
cout << "Verify: I may still be deficient in commodity " << i << ":" << endl;
cout << "Me: " << *this << endl;
                }
            }

            // At this point, one of two things is true:
            // (a) I am no longer deficient in commodity i. Yay!  ...or...
            // (b) I am still deficient in commodity i, but I've exhausted
            // all possibilities of trading with RHS for it.
            // So now, continue looking for other i's I may be deficient in.
if (checkStatus(i) == DEFICIENT) {
cout << "This is the case Stephen wanted to check: I'm still deficient in commodity " << i << " but have run out of trading options with this guy." << endl;
cout << "Me: " << *this << endl;
cout << "RHS: " << other << endl;
}
        }
    }
cout << "done 2!" << endl;
}


void Human::makeHalfSuperSatisfiableTradesWith(Human& other)
{
/*
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
*/
}

void Human::makeOrdinarySatisfiableTradesWith(Human& other) {

}
