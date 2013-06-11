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
    static const int NUM_YEARS = 50;
    static const int TRADING_PARTNERS_PER_YEAR = 50;
    static Model * instance();
    double generateNeedCommodityThreshold();
    double generateWantCommodityThreshold();
    double generateSalary();
	int generateMake();
	double generateMps();
	double generateLifeProb();
	double generateConsume();
    void startSimulation();
	repast::SharedContext<Human>& getActors();
    void printCommodityStats(std::ostream & os) const;

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
    static Model * theInstance;
	Model();
};
#endif
