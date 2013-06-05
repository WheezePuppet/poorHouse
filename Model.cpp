#include"Model.h"
#include<Properties.h>
#include<Random.h>
#include<Schedule.h>
#include<RepastProcess.h>
#include<initialize_random.h>

Model * Model::theInstance = NULL;

Model::Model()
{
std::cout<<"Began constructor\n";
    // Initialize random number distributions.
    repast::Properties theProperties("./distributions.txt");
std::cout<<"Accessed properties file for generators\n";
    repast::initializeRandom(theProperties);
std::cout<<"Initialized random number generator\n";
    commodityNeedThresholdDistro =
        repast::Random::instance()->getGenerator("commodityNeedThreshold");
    commodityWantThresholdDistro =
        repast::Random::instance()->getGenerator("commodityWantThreshold");
    salaryDistro =
        repast::Random::instance()->getGenerator("salary");

std::cout<<"I got to the middle of the constructor\n";

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
std::cout<<"I got to the end of the constructor\n";
}

Model * Model::instance() {
    if (theInstance == NULL) {
std::cout<<"Created new instance\n";
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
