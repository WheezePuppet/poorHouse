#ifndef HUMAN_H
#define HUMAN_H
#include<iostream>
#include<ostream>
#include<AgentId.h>
#include<Random.h>
#include"Commodity.h"


enum CommodityStatus { DEFICIENT, SATISFIED, BLOATED };

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
private:
    int getNumCommoditiesWithStatus(CommodityStatus status) const;
    void trade(int comm1Num, int comm2Num,
        double amtAWillingToBuyOf1, double amtBwillingToBuyOf2,
        double amtAWillingToSellOf2, double amtBwillingToSellOf1,
        Human & B);
    void makeSuperSatisfiableTradesWith(Human& other, CommodityStatus otherState, double thresh[Commodity::NUM_COMM]);
    void makeHalfSuperSatisfiableTradesWith(Human& other);
    void makeOrdinarySatisfiableTradesWith(Human& other);
    void transactWith(Human& other);
    void swap(double x, Human& other, int alow, int blow);
    CommodityStatus checkStatus(int commodityNum) const;
    int findNextDeficientCommodityStartingAt(int x);
    repast::AgentId myId;
    static int nextAgentNum;
	double mps;//Float less than one
	double salary;//Between 3 and 7
	int producedCommodity;//Between 0 and 10
	//vector<double> cons;
	//double savings;
	double commoditiesHeld [Commodity::NUM_COMM];
	double minThreshold [Commodity::NUM_COMM];//Between 0 and 5
	double maxThreshold [Commodity::NUM_COMM];//Between 6 and 10
	int age;
	Human *parent;
    std::vector<Human *> children;
};


// std::ostream & operator<<(const Human & h, std::ostream & os) const;
#endif
