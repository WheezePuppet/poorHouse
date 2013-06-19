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
#include<RepastProcess.h>
#include<fstream>

using namespace std; 

int Human::getNumTraders()
{
	return numTraders;
}

int Human::getTimesTraded()
{
	return timesTraded;
}

double Human::getSalary()
{
	return salary;
}

void Human::tradeWithRandomAgents()
{
/*
	Ok, new plan. Trading with communities in play.
	Generate a number every time, if you are below, trade in community.
	If you're above, you may trade with the world at large.
*/
/*
	std::vector<Human*> tradingPartners;
	Model::instance()->getActors().selectAgents(
        repast::SharedContext<Human>::LOCAL,
		numTraders,//Model::TRADING_PARTNERS_PER_YEAR, 
        tradingPartners, 
        false);
	for(int i=0; i<numTraders; i++)//Model::TRADING_PARTNERS_PER_YEAR; i++)
	{
		transactWith(*tradingPartners[i]);
	}
*/
	for(int i=0; i<numTraders; i++)
	{
		int roll=(Model::instance()->generateNumTraders());
		if(roll>Model::EXTROVERT_DIAL)
		{
			//std::cout<<"Traded outside\n";
			transactWith(*(Model::instance()->getRandomGlobalMember()));
		}
		else
		{
			//std::cout<<"Traded inside\n";
			transactWith(*(Model::instance()->getRandomCommunityMember(residentCommunity)));
		}
	}
}

int Human::nextAgentNum = 0;

std::ostream & operator<<(std::ostream & os, const Human &h) {
double totalNeeds = 0;
for (int i=0; i<Commodity::NUM_COMM; i++) {
totalNeeds += h.minThreshold[i];
}
    os << h.myId << 
        " Salary " << h.salary << " Make " << h.producedCommodity << " mps "
<< h.mps << " total needs " << totalNeeds << " Trades with " 
<< h.numTraders << " Traded " << h.timesTraded << " times " << std::endl << "[";
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
            case SATISFIED:
                os << "=";
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
	numTraders=Model::instance()->generateNumTraders();
	residentCommunity=Model::instance()->generateCommunity(this);
	//probOutTrade=Model::instance()->generateOutsideTrade();

	//Random age for first generation, not for children
	age=0;//(rand()%20)+20;
	timesTraded=0;
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
	//commoditiesHeld[producedCommodity]=salary;
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
//cout << "earning!!" << endl;
	//savings+=(salary+(savings*rr))*mps;
	commoditiesHeld[producedCommodity]+=salary;
}

void Human::consume()
{
//cout << "consuming!!" << endl;
	//cons.push_back(salary+(savings*rr))*(1-mps);
	//cout<<*this<<endl;
	//ofstream statsBeforeConsume;
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
		//cout<<this;
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
int Human::findStatusCommodityStartingAt(int x, CommodityStatus specifiedCommStatus)
{
    for(int i=x; i<Commodity::NUM_COMM; i++)
    {
        if(checkStatus(i)==specifiedCommStatus)
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

//cout << "ACTUALLY trading between " << getId() << " and " << B.getId() << "!!" << endl;
    double coms[4];
    coms[0] = amtAWillingToBuyOf1;
    coms[1] = amtBWillingToSellOf1;
    coms[2] = amtBWillingToBuyOf2;
    coms[3] = amtAWillingToSellOf2;

	bool good=true;
	for(int i=0; i<4; i++)
	{
		if(coms[i]<0)
		{
			coms[i]=10000;//Make all negative lows really high to prevent their being chosen to be traded
		}
		else if(coms[i]==0)
		{
			good=false;//If at least one is not negative or zero, make it possible to trade 
		}
	}

	if(good)
	{
		double* x = std::min_element(coms, coms+4);//Choose the lowest of the options for trades so that you don't overstep any of the bounds
		double change=*x;
		//std::cout<<*this;
		//std::cout<<B;
		//std::cout<<"Exchanging "<<change<<" units of commodities " <<comm1Num<< " and " <<comm2Num <<std::endl;
		swap(change, B, comm1Num, comm2Num);
		//std::cout<<*this;
	}
}


void Human::incrementTrades()
{
	timesTraded++;
}

void Human::transactWith(Human& other)
{
	incrementTrades();
	other.incrementTrades();
//cout << "transacting from " << getId() << " to " << other.getId() << endl;

	/*
D,B,B,D  - SUPER

D,B,B,S  - HALF-SUPER
D,B,S,D  - HALF-SUPER
S,B,B,D  - HALF-SUPER
D,S,B,D  - HALF-SUPER

D,S,S,D  - ORDINARY
S,B,B,S  - ORDINARY
S,B,D,S  - ORDINARY
S,D,B,S  - ORDINARY

	*/

    // 1. Make super-satisfiable trades where possible.
    makeTradesSuchThat(other, DEFICIENT, BLOATED, BLOATED, DEFICIENT);

    // 2. Make half-super-satisfiable trades where possible.

    makeTradesSuchThat(other, DEFICIENT, BLOATED, BLOATED, SATISFIED);
    makeTradesSuchThat(other, DEFICIENT, BLOATED, SATISFIED, DEFICIENT);
    makeTradesSuchThat(other, SATISFIED, BLOATED, BLOATED, DEFICIENT);
    makeTradesSuchThat(other, DEFICIENT, SATISFIED, BLOATED, DEFICIENT);

    // 3. make ordinary-satisfiable trades happen.

    makeTradesSuchThat(other, DEFICIENT, SATISFIED, SATISFIED, DEFICIENT);
    makeTradesSuchThat(other, SATISFIED, BLOATED, BLOATED, SATISFIED);
    makeTradesSuchThat(other, SATISFIED, BLOATED, DEFICIENT, SATISFIED);
    makeTradesSuchThat(other, SATISFIED, DEFICIENT, BLOATED, SATISFIED);

}

void Human::makeTradesSuchThat(Human& other, 
	CommodityStatus aTooLowStatus_C1,//The level A wants to get above in good 1
    CommodityStatus aWantToPreserveStatus_C2,//The level A doesn't want to fall below in good 2
    CommodityStatus otherWantToPreserveStatus_C1,//The level B doesn't want to fall below in good 1
    CommodityStatus otherTooLowStatus_C2)//The level B wants to get above in good 2
{
//cout << "trades between " << getId() << " and " << other.getId() << "..." << endl;
int numTrades = 0;

    // Go through all my commodities, searching for ones I'm below my lower bound in.
    for(int i=0; i<Commodity::NUM_COMM; i++)
    {
        i= findStatusCommodityStartingAt(i, aTooLowStatus_C1);
        if(i!=Commodity::NUM_COMM &&
            other.checkStatus(i)==otherWantToPreserveStatus_C1) {
            // Okay, this is now true: I want more of commodity i, and the
            // RHS (possibly) has some to spare. (We say "possibly" because
            // they might be exactly at the threshold of their current
            // status, and so actually wouldn't want to trade any of it.)
            // So we've "halfway" found a possible super-satisfiable trade.
        
            for(int j=0; j<Commodity::NUM_COMM; j++)
            {
                j=other.findStatusCommodityStartingAt(j, otherTooLowStatus_C2);
				if(j==Commodity::NUM_COMM)
				{
                    // Well, bummer. The RHS doesn't want anything,
                    // so there are no trades of this kind possible.
//cout << "Made " << numTrades << " trades." << endl;
					return;
				}
                if(checkStatus(j) == aWantToPreserveStatus_C2) 
                {
                    // Do actual trade! This is because we now know:
                    //   - A wants more i
                    //   - B has an excess of i
                    //   - B wants more j
                    //   - A has an excess of j 
					double aWantToBuy_Ci,
					       aWillingToSell_Cj,
					       otherWantToBuy_Cj,
					       otherWillingToSell_Ci;
					switch(aTooLowStatus_C1) {
                        case DEFICIENT:
						aWantToBuy_Ci=minThreshold[i]-commoditiesHeld[i];
                        break;
                        case SATISFIED:
						aWantToBuy_Ci=maxThreshold[i]-commoditiesHeld[i];
                        break;
					}
					switch(otherTooLowStatus_C2) {
                        case DEFICIENT:
						otherWantToBuy_Cj=
                            other.minThreshold[j]-other.commoditiesHeld[j];
                        break;
                        case SATISFIED:
						otherWantToBuy_Cj=
                            other.maxThreshold[j]-other.commoditiesHeld[j];
                        break;
					}
					switch(aWantToPreserveStatus_C2) {
                        case BLOATED:
						aWillingToSell_Cj=commoditiesHeld[j]-maxThreshold[j];
                        break;
                        case SATISFIED:
						aWillingToSell_Cj=commoditiesHeld[j]-minThreshold[j];
                        break;
					}
					switch(otherWantToPreserveStatus_C1) {
                        case BLOATED:
						otherWillingToSell_Ci=
                            other.commoditiesHeld[i]-other.maxThreshold[i];
                        break;
                        case SATISFIED:
						otherWillingToSell_Ci=
                            other.commoditiesHeld[i]-other.minThreshold[i];
                        break;
					}

                    trade(i,
                          j,
                          aWantToBuy_Ci,
                          otherWillingToSell_Ci,
                          otherWantToBuy_Cj,
                          aWillingToSell_Cj,
                          other);
numTrades++;

                    // We have now traded. However, I may still be too low
                    // in commodity i. If so, I need to continue to look 
                    // for other j's to continue to satisfy my i.
                }
            }

            // At this point, one of two things is true:
            // (a) I am no longer lacking in commodity i. Yay!  ...or...
            // (b) I am still lacking in commodity i, but I've exhausted
            // all possibilities of trading with RHS for it.
            // So now, continue looking for other i's I may be lacking in.
        }
    }
//cout << "Made " << numTrades << " trades." << endl;
}

int Human::getNumCommoditiesWithStatus(CommodityStatus status) const {
    int i=0;
    for (int j=0; j<Commodity::NUM_COMM; j++) {
        if (checkStatus(j) == status) {
            i++;
        }
    }
    return i;
}

int Human::getNumDeficientCommodities() const { 
    return getNumCommoditiesWithStatus(DEFICIENT);
}

int Human::getNumSatisfiedCommodities() const {
    return getNumCommoditiesWithStatus(SATISFIED);
}

int Human::getNumBloatedCommodities() const {
    return getNumCommoditiesWithStatus(BLOATED);
}

double Human::amtCommodity(int x)
{
	return commoditiesHeld [x];
}

int Human::getMake()
{
	return producedCommodity;
}

std::string commodityStatusString(CommodityStatus cs) {
    switch (cs) {
    case DEFICIENT:
        return "DEFICIENT";
    case BLOATED:
        return "BLOATED";
    case SATISFIED:
        return "SATISFIED";
    }
    return "SOMETHING IS DREADFULLY WRONG!!!!!";
}
    
