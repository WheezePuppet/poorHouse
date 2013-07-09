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

	//Global constants
    static const int NUM_INITIAL_AGENTS = 100;
    static const int NUM_YEARS = 200;
	static int INTROVERT_DIAL;// = 50;
	static int SEED;
	static const int LEMMINGNESS = 100;
	static const int COMMUNITIES = 10;

	//Miscellaneous
    double wealthGiniCoefficient() const;
    double satisfactionGiniCoefficient() const;
    Human * getRandomCommunityMember(int communityNum) const;
    Human * getRandomGlobalMember();
	double getAvgDeficientCommComm(int) const;
	void inter(Human *);
	void addToActors(Human *);
	void addToCommunity(int, Human *);

	//Keep track of Model variables
	void incrementTrades();
	void resetTrades();
	void addToTradedAmount(double);
	void resetTradedAmount();
	void incrementPopulation();
	void decrementPopulation();

	//Repast things
	virtual repast::AgentId & getId();
	virtual const repast::AgentId & getId() const;
    static Model * instance();
    void startSimulation();
	void fillCommunities();

	//Return model things
	repast::SharedContext<Human>& getActors();
    std::vector<Human *> getCommunityMembers(int communityNum) const;
    std::vector<Human *> getGraveyard();
    int getCommunitySize(int communityNum) const;
	double getTick();
	int getPopulation();

	//Print things out
    void printCommodityStats(std::ostream & os) const;
	void printGini();
    void printCommunityStats(std::ostream & os) const;

	//Random number generator next functions
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

	//Obsolete
	//void setNUM_INITIAL_AGENTS(int);
	//void setNUM_YEARS(int);

private:
    repast::AgentId myId;
    static Model * theInstance;

	//Trackers
	int population;
	double yearlyAmountTraded;
	int yearlyTrades;

	//Functions
	Model();
    void createInitialAgents();
    void startYear();
    double computeGini(std::vector<double> values) const;

	//Agent containers
	repast::SharedContext<Human> actors;
	std::vector<std::vector<Human *> > communities;
	std::vector<Human *> graveyard;

	//Random number generator declarations
    repast::NumberGenerator *commodityNeedThresholdDistro;
    repast::NumberGenerator *commodityWantThresholdDistro;
    repast::NumberGenerator *salaryDistro;
	repast::NumberGenerator *makeDistro;	
	repast::NumberGenerator *deathDistro;
	repast::NumberGenerator *mpsDistro;
	repast::NumberGenerator *consumeDistro;
	repast::NumberGenerator *tradeDistro;
	repast::NumberGenerator *outsideTrade;
	repast::NumberGenerator *communityDistro;
	repast::NumberGenerator *childDistro;
};
#endif
