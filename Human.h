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
	void step();
	Human();
	//Human(int);
	void earnIncome();
	void consume();
	void considerHavingAChild();
	void considerDeath();
	~Human();
    virtual repast::AgentId & getId();
    virtual const repast::AgentId & getId() const;
	void tradeWithRandomAgents();
    int getNumDeficientCommodities() const;
    int getNumSatisfiedCommodities() const;
    int getNumBloatedCommodities() const;
	double getSalary();
	double getWealth();
	double getSatisfaction();
	double amtCommodity(int x);
	int getMake();
	int getNumTraders();
	void incrementTrades();
	int getTimesTraded();
	int getCommunity();
private:
	double mps;//Float less than one
	double salary;//Between 3 and 7
	int producedCommodity;//Between 0 and 10
	int numTraders;//Between 5 and 100
	Human *parent;
    repast::AgentId myId;
	int age;
	int residentCommunity;
	int timesTraded;

    double commoditiesHeld [Commodity::NUM_COMM];
	double minThreshold [Commodity::NUM_COMM];//Between 0 and 5
	double maxThreshold [Commodity::NUM_COMM];//Between 6 and 10

    std::vector<Human *> children;
    static int nextAgentNum;

    void makeTradesSuchThat(Human& other, 
        CommodityStatus aTooLowStatus_C1,
        CommodityStatus aWantToPreserveStatus_C2,
        CommodityStatus otherWantToPreserveStatus_C1,
        CommodityStatus otherTooLowStatus_C2);
    void transactWith(Human& other);
    void swap(double x, Human& other, int alow, int blow);
    CommodityStatus checkStatus(int commodityNum) const;
	int findStatusCommodityStartingAt(int x, CommodityStatus sepcifiedCommStatus);
    int getNumCommoditiesWithStatus(CommodityStatus status) const;
    void trade(int comm1Num, int comm2Num,
        double amtAWillingToBuyOf1, double amtBwillingToBuyOf2,
        double amtAWillingToSellOf2, double amtBwillingToSellOf1,
        Human & B);

    //int findNextDeficientCommodityStartingAt(int x);
	//vector<double> cons;
	//double savings;
	//int probOutTrade;
};


// std::ostream & operator<<(const Human & h, std::ostream & os) const;
#endif
