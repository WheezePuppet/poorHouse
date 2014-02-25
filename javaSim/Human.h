#ifndef HUMAN_H
#define HUMAN_H
#include<iostream>
#include<ostream>
#include<string>
#include<AgentId.h>
#include<Random.h>
#include"Commodity.h"


enum CommodityStatus { DEFICIENT, SATISFIED, BLOATED };

std::string commodityStatusString(CommodityStatus cs);

class Human : public repast::Agent
{

friend std::ostream & operator<<(std::ostream & os, const Human & h);

public:
	//Global static values
	static int BEQ;

    virtual repast::AgentId & getId() { return myId; }
    virtual const repast::AgentId & getId() const { return myId; }

	//Scheduled events
	void earnIncome();
	void tradeWithRandomAgents();
	void consume();
	void considerHavingAChild();
	void considerDeath();
	void reschedule();

	//Constructors & destructors
	Human();
	Human(Human *);
	~Human() { }

	//Return singular, unchanging data about the agent
	int getCommunity() { return residentCommunity; }
	int getTimesTraded() { return timesTraded; }
	int getMake() { return producedCommodity; }
	double getSalary() { return salary; }
	int getNumTraders() { return numTraders; }
	int getAge() const { return age; }

	//Return composite data about the agent
	double getWealth();
	double getSatisfaction();
	double getNeeds() { return allNeeds; }
	double getAmtOfCommodityX(int x) { return commoditiesHeld[x]; }
    int getNumDeficientCommodities() const { return getNumCommoditiesWithStatus(DEFICIENT); }
    int getNumSatisfiedCommodities() const { return getNumCommoditiesWithStatus(SATISFIED); }
    int getNumBloatedCommodities() const { return getNumCommoditiesWithStatus(BLOATED); }
	
private:
	//Human data unchanging
	double mps;//Float less than one
	double salary;//Between 3 and 7
	int producedCommodity;//Between 0 and 10
	int numTraders;//Between 5 and 100
	Human * parent;
    repast::AgentId myId;
	int residentCommunity;
	double allNeeds;
	double minThreshold [Commodity::NUM_COMM];//Between 0 and 5
	double maxThreshold [Commodity::NUM_COMM];//Between 6 and 10

	//Human data changing
	int age;
	int timesTraded;
    double commoditiesHeld [Commodity::NUM_COMM];
    std::vector<Human *> children;

	//Ummm...
    static int nextAgentNum;

	//Private trade functions
    void makeTradesSuchThat(Human& other, 
        CommodityStatus aTooLowStatus_C1,
        CommodityStatus aWantToPreserveStatus_C2,
        CommodityStatus otherWantToPreserveStatus_C1,
        CommodityStatus otherTooLowStatus_C2);
    void transactWith(Human& other);
    void swap(double x, Human& other, int alow, int blow);
    void trade(int comm1Num, int comm2Num,
        double amtAWillingToBuyOf1, double amtBwillingToBuyOf2,
        double amtAWillingToSellOf2, double amtBwillingToSellOf1,
        Human & B);
	void incrementTrades() { timesTraded++; }

	//Commodity checking utilties
    CommodityStatus checkStatus(int commodityNum) const;
	int findStatusCommodityStartingAt(int x, CommodityStatus sepcifiedCommStatus);
    int getNumCommoditiesWithStatus(CommodityStatus status) const;

	//Inheritance functions
	void omniBequeath(Human *);
	void primoBequeath(Human *);

    //int findNextDeficientCommodityStartingAt(int x);
	//vector<double> cons;
	//double savings;
	//int probOutTrade;
};


// std::ostream & operator<<(const Human & h, std::ostream & os) const;
#endif
