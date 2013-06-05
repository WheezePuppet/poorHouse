#ifndef MODEL_H
#define MODEL_H
#include<SharedContext.h>
#include"Human.h"
#include<Random.h>
//#include"Commodity.h"

class Model
{
public:
    static const int NUM_INITIAL_AGENTS = 100;
    static Model * instance();
    double generateNeedCommodityThreshold();
    double generateWantCommodityThreshold();
    double generateSalary();
	int generateMake();
    void startSimulation();

private:
    void createInitialAgents();
	repast::SharedContext<Human> actors;
    repast::NumberGenerator *commodityNeedThresholdDistro;
    repast::NumberGenerator *commodityWantThresholdDistro;
    repast::NumberGenerator *salaryDistro;
	repast::NumberGenerator *makeDistro;
    static Model * theInstance;
	Model();
};
#endif



/*
    ...somewhere, Russell needs to generate a random commodity threshold!

    he writes this code!

    Model::instance()->generateCommodityThreshold();
*/
