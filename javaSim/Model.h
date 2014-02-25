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
    static const int NUM_YEARS = 100;
	static int INTROVERT_DIAL;// = 50;
	static int SEED;
	static const int LEMMINGNESS = 100;
	static const int COMMUNITIES = 10;

	//Random number generator next functions
    double generateNeedCommodityThreshold() { return commodityNeedThresholdDistro->next(); }
    double generateWantCommodityThreshold() { return commodityWantThresholdDistro->next(); }
    double generateSalary();
	int generateMake() { return makeDistro->next(); }
	double generateMps() { return mpsDistro->next(); }
	double generateLifeProb() { return deathDistro->next(); }
	double generateConsume() { return consumeDistro->next(); }
	int generateNumTraders() { return tradeDistro->next(); }
	int generateOutsideTrade() { return outsideTrade->next(); }
	int generateCommunity(Human *);
	int generateChild() { return childDistro->next(); }
	int generateAge() { return ageDistro->next(); }

	//Miscellaneous
    double wealthGiniCoefficient() const;
	double adultWealthGiniCoefficient() const;
    double satisfactionGiniCoefficient() const;
    Human * getRandomCommunityMember(int communityNum) const;
    Human * getRandomGlobalMember();
	double getAvgDeficientCommComm(int) const;
	void inter(Human *);
	void addToActors(Human * man) { actors.addAgent(man); }
	void addToCommunity(int i, Human * man) { communities[i].push_back(man); }

	//Keep track of Model variables
	void incrementTrades() { yearlyTrades++; }
	void resetTrades() { yearlyTrades=0; }
	void addToTradedAmount(double change) { yearlyAmountTraded+=change; }
	void resetTradedAmount() { yearlyAmountTraded=0; }
	void incrementPopulation() { population++; }
	void decrementPopulation() { population--; }
	void incrementOmniEvent() { numOmniEvents++; }
	void resetOmniEvent() { numOmniEvents=0; }
	void calculateTotalWealth();
	void resetTotalWealth() { totalWealth=0; }
	void addToWealthRedistributed(double value) { wealthRedistributed+=value; }
	void resetWealthRedistributed() { wealthRedistributed=0; }

	//Repast things
	virtual repast::AgentId & getId() { return myId; }
	virtual const repast::AgentId & getId() const { return myId; }
    static Model * instance();
    void startSimulation();
	void fillCommunities();

	//Return model things
	repast::SharedContext<Human>& getActors();
    std::vector<Human *> getCommunityMembers(int communityNum) const;
    std::vector<Human *> getGraveyard();
    int getCommunitySize(int communityNum) const { return communities[communityNum].size(); }
	double getTick();
	int getPopulation() { return population; }
	double getTotalWealth() { return totalWealth; }
	double getWealthRedistributed() { return wealthRedistributed; }
	double calculatePercentWealthRedistributed();

	//Print things out
    void printCommodityStats(std::ostream & os) const;
	void printGini();
    void printCommunityStats(std::ostream & os) const;

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
	int numOmniEvents;
	double totalWealth;
	double wealthRedistributed;

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
	repast::NumberGenerator *ageDistro;
};
#endif
