#include"Model.h"
#include<Properties.h>
#include<Random.h>
#include<Schedule.h>
#include<RepastProcess.h>
#include<initialize_random.h>

Model * Model::theInstance = NULL;

Model::Model()
{
    // Initialize random number distributions.
    repast::Properties theProperties("./distributions.txt");
    repast::initializeRandom(theProperties);
    commodityNeedThresholdDistro =
        repast::Random::instance()->getGenerator("commodityNeedThreshold");
    commodityWantThresholdDistro =
        repast::Random::instance()->getGenerator("commodityWantThreshold");
    salaryDistro =
        repast::Random::instance()->getGenerator("salary");


    // Create the initial generation of agents, and add them to the Context.
    // While we're at it, schedule all agents to run in 1 year's time.
    repast::ScheduleRunner &harryPotter = 
        repast::RepastProcess::instance()->getScheduleRunner();
    for (int i=0; i<NUM_INITIAL_AGENTS; i++) {
        Human *newHuman = new Human();
        actors.addAgent(newHuman);
        harryPotter.scheduleEvent(1, repast::Schedule::FunctorPtr(
            new repast::MethodFunctor<Human>(newHuman, &Human::step)));
    }

    // Schedule an end point.
    harryPotter.scheduleStop(2);

    // Let's DO THIS THING!!!
    harryPotter.run();
}

Model * Model::instance() {
    if (theInstance == NULL) {
        theInstance = new Model();
    }
    return theInstance;
}

double Model::generateNeedCommodityThreshold() {
    return commodityNeedThresholdDistro->next();
}

double Model::generateWantCommodityThreshold() {
    return commodityWantThresholdDistro->next();
}

double Model::generateSalary() {
    return salaryDistro->next();
}
