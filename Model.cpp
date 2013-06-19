#include"Model.h"
#include<Properties.h>
#include<Random.h>
#include<Schedule.h>
#include<RepastProcess.h>
#include<iterator>
#include<initialize_random.h>
#include<fstream>

using namespace std;

Model * Model::theInstance = NULL;

repast::SharedContext<Human>& Model::getActors()
{
	return actors;
}

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
	makeDistro =
		repast::Random::instance()->getGenerator("make");
	mpsDistro =
		repast::Random::instance()->getGenerator("mps");
	deathChildDistro =		
		repast::Random::instance()->getGenerator("deathChild");
	consumeDistro =
		repast::Random::instance()->getGenerator("consume");
	tradeDistro =
		repast::Random::instance()->getGenerator("trade");
}

void Model::createInitialAgents() {

    // Create the initial generation of agents, and add them to the Context.
    // Schedule the startYear method to run at integer clock ticks (start
    //   of year, say.
    // While we're at it, schedule all agents to run on the following
    //   schedule:
    // - On integer + .1, earn income.
    // - At integer + .2, trade with other random agents.
    // - At integer + .3, consume commodities for the year.
    repast::ScheduleRunner &theScheduleRunner = 
        repast::RepastProcess::instance()->getScheduleRunner();
    theScheduleRunner.scheduleEvent(1, 1, repast::Schedule::FunctorPtr(
        new repast::MethodFunctor<Model>(this, &Model::startYear)));
    for (int i=0; i<NUM_INITIAL_AGENTS; i++) {
        Human *newHuman = new Human();
        actors.addAgent(newHuman);
        theScheduleRunner.scheduleEvent(1.1, 1, repast::Schedule::FunctorPtr(
            new repast::MethodFunctor<Human>(newHuman, &Human::earnIncome)));
        theScheduleRunner.scheduleEvent(1.2, 1, repast::Schedule::FunctorPtr(
            new repast::MethodFunctor<Human>(newHuman,
                &Human::tradeWithRandomAgents)));
		//Print the stats before consuming
	/*	ofstream statsBeforeConsume;
		statsBeforeConsume.open("statsBeforeConsume.txt");
        theScheduleRunner.scheduleEvent(1.25, 1, repast::Schedule::FunctorPtr(
			new repast::MethodFunctor<Model>(this, &Model::printCommodityStats(statsBeforeConsume))));
*/
        theScheduleRunner.scheduleEvent(1.3, 1, repast::Schedule::FunctorPtr(
            new repast::MethodFunctor<Human>(newHuman,
                &Human::consume)));
    }
}

void Model::startYear() {
    repast::ScheduleRunner &theScheduleRunner = 
        repast::RepastProcess::instance()->getScheduleRunner();
    cout << "======================================================" << endl;
    cout << "Now starting year " <<  theScheduleRunner.currentTick() << "!"
        << endl;
}

void Model::startSimulation() {

    createInitialAgents();

    repast::ScheduleRunner &theScheduleRunner = 
        repast::RepastProcess::instance()->getScheduleRunner();

    // Schedule an end point.
    theScheduleRunner.scheduleStop(NUM_YEARS);

    // Let's DO THIS THING!!!
    theScheduleRunner.run();
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
    double thing=salaryDistro->next();
	while(thing<0)
	{
		thing=salaryDistro->next();
	}
	return thing;
}

double Model::generateMps() {
	return mpsDistro->next();
}

int Model::generateMake() {
	return makeDistro->next();
}

double Model::generateLifeProb() {
	return deathChildDistro->next();
}

double Model::generateConsume() {
//	return consumeDistro->next();
	double bob = consumeDistro->next();

return bob;
}

int Model::generateNumTraders() {
	return tradeDistro->next();
}

/*int Model::generateOutsideTrade()
{
	return outsideTrade->next();
}
*/
void Model::printCommodityStats(std::ostream & os) const {

cout << "printing all stats!" << endl;
    repast::SharedContext<Human>::const_local_iterator actorIter = 
        actors.localBegin();

    while (actorIter != actors.localEnd()) {
        os << (*actorIter)->getNumDeficientCommodities() << "," <<
              (*actorIter)->getNumSatisfiedCommodities() << "," <<
              (*actorIter)->getNumBloatedCommodities() << ","<<
			  (*actorIter)->getSalary()<<","<< 
			  (*actorIter)->amtCommodity((*actorIter)->getMake())<< "," <<
			  (*actorIter)->getNumTraders()<< endl;
		std::cout<<(*actorIter)->getId() << " has these totals: " <<
			  (*actorIter)->getNumDeficientCommodities() << "," <<
              (*actorIter)->getNumSatisfiedCommodities() << "," <<
              (*actorIter)->getNumBloatedCommodities() << endl;
cout << *(*actorIter) << endl;
        actorIter++;
    }
}


repast::AgentId & Model::getId() { 
    return myId;
}   

const repast::AgentId & Model::getId() const {
    return myId;
}

/*
void Model::setNUM_INITIAL_AGENTS(int num)
{
	NUM_INITIAL_AGENTS=num;
}

void Model::setNUM_YEARS(int years)
{
	NUM_YEARS=years;
}

void Model::setTRADING_PARTNERS_PER_YEAR(int partners)
{
	TRADING_PARTNERS_PER_YEAR=partners;
}
*/
