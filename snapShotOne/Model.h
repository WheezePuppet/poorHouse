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
	double getTick();
    static const int NUM_INITIAL_AGENTS = 100;
    static const int NUM_YEARS = 100;
	static int INTROVERT_DIAL;// = 50;
	static int SEED;
	static const int LEMMINGNESS = 100;
	static const int COMMUNITIES = 10;
    static Model * instance();
	void fillCommunities();
    double generateNeedCommodityThreshold();
    double generateWantCommodityThreshold();
    double generateSalary();
	int generateMake();
	double generateMps();
	double generateLifeProb();
	double generateConsume();	
	int generateNumTraders();
	int generateOutsideTrade();
	int generateCommunity(Human *);
	int generateChild();
    void startSimulation();
	repast::SharedContext<Human>& getActors();
    void printCommodityStats(std::ostream & os) const;
	void setNUM_INITIAL_AGENTS(int);
	void setNUM_YEARS(int);
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
	void inter(Human *);

virtual repast::AgentId & getId();
virtual const repast::AgentId & getId() const;

private:
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
	repast::NumberGenerator *deathChildDistro;
	repast::NumberGenerator *mpsDistro;
	repast::NumberGenerator *consumeDistro;
	repast::NumberGenerator *tradeDistro;
	repast::NumberGenerator *outsideTrade;
	repast::NumberGenerator *communityDistro;
	repast::NumberGenerator *childDistro;
    static Model * theInstance;
	Model();
	std::vector<std::vector<Human *> > communities;
	std::vector<Human *> graveyard;
};
#endif
