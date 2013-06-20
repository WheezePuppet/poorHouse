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
    static const int NUM_INITIAL_AGENTS = 100;
    static const int NUM_YEARS = 100;
    static const int TRADING_PARTNERS_PER_YEAR = 110;
	static const int INTROVERT_DIAL = 50;
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
    void startSimulation();
	repast::SharedContext<Human>& getActors();
    void printCommodityStats(std::ostream & os) const;
	void setNUM_INITIAL_AGENTS(int);
	void setNUM_YEARS(int);
    std::vector<Human *> getCommunityMembers(int communityNum) const;
    int getCommunitySize(int communityNum) const;
    Human * getRandomCommunityMember(int communityNum) const;
    Human * getRandomGlobalMember();
	double getAvgDeficientCommComm(int) const;
    void printCommunityStats(std::ostream & os) const;

virtual repast::AgentId & getId();
virtual const repast::AgentId & getId() const;

private:
    repast::AgentId myId;
    void createInitialAgents();
    void startYear();
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
    static Model * theInstance;
	Model();
	std::vector<std::vector<Human *> > communities;
};
#endif
