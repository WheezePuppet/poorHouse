#ifndef MODEL_H
#define MODEL_H
#include<SharedContext.h>
#include"Human.h"
#include<Random.h>
#include<ostream>
//#include"Commodity.h"

class Model : public repast::Agent
{
public:
	double getTick();//Returns simulation time
	void incrementPopulation();
	void decrementPopulation();
	int getPopulation();//How many people are on the schedule right now
    static const int NUM_INITIAL_AGENTS = 100;
    static const int NUM_YEARS = 100;
	static int INTROVERT_DIAL;// = 50;
	static int SEED;
	static const int LEMMINGNESS = 100;
	static const int COMMUNITIES = 10;
    static Model * instance();//Give a reference to the singletone model class
    double generateNeedCommodityThreshold();//Next distribution
    double generateWantCommodityThreshold();//Next distribution
    double generateSalary();//Next distribution
	int generateMake();//Next distribution
	double generateMps();//Next distribution
	double generateConsume();//Next distribution
	int generateNumTraders();//Next distribution
	int generateOutsideTrade();//Next distribution
	int generateCommunity(Human *);//Assign community to agent and add agent to community
	int generateChild();
	int generateDeath();
    void startSimulation();
	repast::SharedContext<Human>& getActors();
    void printCommodityStats(std::ostream & os) const;//Return ostream with all agents' info by community
    std::vector<Human *> getCommunityMembers(int communityNum) const;
    std::vector<Human *> getGraveyard();
    int getCommunitySize(int communityNum) const;
    Human * getRandomCommunityMember(int communityNum) const;
    Human * getRandomGlobalMember();
	double getAvgDeficientCommComm(int) const;
    void printCommunityStats(std::ostream & os) const;
    double wealthGiniCoefficient() const;
    double satisfactionGiniCoefficient() const;
	void printGini();
	void incrementTrades();
	void resetTrades();
	void addToTradedAmount(double);
	void resetTradedAmount();
	void inter(Human *);//Remove agent to graveyard vector
	void addToActors(Human *);//Add new human to the context
	void addToCommunity(int, Human *);

virtual repast::AgentId & getId();
virtual const repast::AgentId & getId() const;

private:
	int population;
	void createCommunities();
    repast::AgentId myId;
    void createInitialAgents();
    void startYear();
    double computeGini(std::vector<double> values) const;
	int yearlyTrades;
	double yearlyAmountTraded;
	repast::SharedContext<Human> actors;
    repast::NumberGenerator *commodityNeedThresholdDistro;
    repast::NumberGenerator *commodityWantThresholdDistro;
    repast::NumberGenerator *salaryDistro;
	repast::NumberGenerator *makeDistro;	
	repast::NumberGenerator *mpsDistro;
	repast::NumberGenerator *consumeDistro;
	repast::NumberGenerator *tradeDistro;
	repast::NumberGenerator *outsideTrade;
	repast::NumberGenerator *communityDistro;
	repast::NumberGenerator *childDistro;
	repast::NumberGenerator *deathDistro;
    static Model * theInstance;
	Model();
	std::vector<std::vector<Human *> > communities;
	std::vector<Human *> graveyard;
};
#endif
