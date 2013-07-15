#include"Model.h"
#include<Properties.h>
#include<Random.h>
#include<Schedule.h>
#include<RepastProcess.h>
#include<iterator>
#include<initialize_random.h>
#include<fstream>

using namespace std;

// For weird C++ reasons, "define" the static variables. (??)
Model * Model::theInstance = NULL;
int Model::INTROVERT_DIAL;
int Model::SEED;
//int Model::LEMMINGNESS;

//Miscellaneous
//============================================================
void Model::addToCommunity(int i, Human * man)
{
	communities[i].push_back(man);
}

void Model::addToActors(Human * man)
{
	actors.addAgent(man);
}

void Model::inter(Human * body)
{
	graveyard.push_back(body);
	//std::vector<Human *> grieving=communities[(*body).getCommunity()];
	int h=communities[(*body).getCommunity()].size();
	int cmp=(*body).getCommunity();
	//int h=grieving.size();
	//std::cout<<cmp<<','<<h<<std::endl;
	for(int i=0; i<h; i++)
	{
		if(communities[(*body).getCommunity()][i]==body)
		//if(grieving[i]==body)
		{
			communities[(*body).getCommunity()].erase((communities[(*body).getCommunity()].begin())+i);
			//grieving.erase((grieving.begin()+i));
		}
	}
	//grieving.erase(std::find(grieving.begin(),grieving.end(),body));
}

int Model::getCommunitySize(int communityNum) const
{
	return communities[communityNum].size();
}

Human * Model::getRandomCommunityMember(int communityNum) const
{
	int indexOfRandomMember = tradeDistro->next();
	indexOfRandomMember = indexOfRandomMember%(communities[communityNum].size());
	return communities[communityNum][indexOfRandomMember];
}

Human * Model::getRandomGlobalMember()
{
	std::vector<Human *> oneAgent;
	actors.getRandomAgents(1,oneAgent);
	return oneAgent[0];
}

double Model::getAvgDeficientCommComm(int commuNum) const
{
	int i=getCommunitySize(commuNum);
	double total=0;
	for(int j=0; j<i; j++)
	{
		total+=((getCommunityMembers(commuNum))[j])->getNumDeficientCommodities();
	}
	total/=i;
	return total;
}

/*
 * wealthGiniCoefficient -- compute "Gini coefficient".
 */
double Model::adultWealthGiniCoefficient() const {

    vector<double> wealths;
/*
    for (repast::SharedContext<Human>::const_local_iterator actorIter = 
        actors.localBegin();
        actorIter != actors.localEnd(); actorIter++) {
        
		wealths.push_back((*actorIter)->getWealth());
    }
*/
	for(int i=0; i<Model::COMMUNITIES; i++)
	{
		for(int j=0; j<Model::instance()->getCommunityMembers(i).size(); j++)
		{
			Human * man = Model::instance()->getCommunityMembers(i)[j];
			if(((*man).getAge())>20)
			{
				wealths.push_back((*man).getWealth());
			}
		}
	}
    return computeGini(wealths);
}

double Model::wealthGiniCoefficient() const {

	vector<double> wealths;

	for(int i=0; i<Model::COMMUNITIES; i++)
	{
		for(int j=0; j<Model::instance()->getCommunityMembers(i).size(); j++)
		{
			Human * man = Model::instance()->getCommunityMembers(i)[j];
			wealths.push_back((*man).getWealth());
		}
	}
    return computeGini(wealths);
}

/*
 * satisfactionGiniCoefficient -- compute "Russell coefficient".
 */
double Model::satisfactionGiniCoefficient() const {

    vector<double> satisfactions;
/*
    for (repast::SharedContext<Human>::const_local_iterator actorIter = 
        actors.localBegin();
        actorIter != actors.localEnd(); actorIter++) {
        
		satisfactions.push_back((*actorIter)->getSatisfaction());
    }
*/
	for(int i=0; i<Model::COMMUNITIES; i++)
	{
		for(int j=0; j<Model::instance()->getCommunityMembers(i).size(); j++)
		{
			Human * man = Model::instance()->getCommunityMembers(i)[j];
			satisfactions.push_back((*man).getSatisfaction());
		}
	}
    return computeGini(satisfactions);
}

//Keep track of Model variables
//============================================================

void Model::incrementTrades()
{
	yearlyTrades++;
}

void Model::resetTrades()
{
	yearlyTrades=0;
}

void Model::addToTradedAmount(double change)
{
	yearlyAmountTraded+=change;
}

void Model::resetTradedAmount()
{
	yearlyAmountTraded=0;
}

void Model::incrementPopulation()
{
	population++;
}

void Model::decrementPopulation()
{
	population--;
}

void Model::incrementOmniEvent()
{
	numOmniEvents++;
}

void Model::resetOmniEvent()
{
	numOmniEvents=0;
}

void Model::calculateTotalWealth()
{
	for(int i=0; i<Model::COMMUNITIES; i++)
	{
		for(int j=0; j<communities[i].size(); j++)
		{
			totalWealth+=(communities[i][j])->getWealth();
		}
	}
}

void Model::resetTotalWealth()
{
	totalWealth=0;
}

void Model::addToWealthRedistributed(double value)
{
	wealthRedistributed+=value;
}

void Model::resetWealthRedistributed()
{
	wealthRedistributed=0;
}

//Repast things
//============================================================
repast::AgentId & Model::getId() { 
    return myId;
}   

const repast::AgentId & Model::getId() const {
    return myId;
}

Model * Model::instance() {
    if (theInstance == NULL) {
        theInstance = new Model();
    }
    return theInstance;
}

void Model::startSimulation() {

    createInitialAgents();
/*
	for(int i=0; i<COMMUNITIES; i++)
	{
		std::cout<<"Community "<< i <<" has "<< communities[i].size() <<" members\n";
	}
*/
    repast::ScheduleRunner &theScheduleRunner = 
        repast::RepastProcess::instance()->getScheduleRunner();

    // Schedule an end point.
    theScheduleRunner.scheduleStop(NUM_YEARS);

    // Let's DO THIS THING!!!
    theScheduleRunner.run();
}

void Model::fillCommunities()
{
	for(int i=0; i<COMMUNITIES; i++)
	{
		communities.push_back(*(new std::vector<Human *>));
		//std::cout<<"Added "<<i<<" communities\n";
	}
} 

//Return model things
//============================================================
repast::SharedContext<Human>& Model::getActors()
{
	return actors;
}

std::vector<Human *> Model::getCommunityMembers(int communityNum) const
{
	return communities[communityNum];
}

std::vector<Human *> Model::getGraveyard()
{
	return graveyard;
}

double Model::getTick()
{
    repast::ScheduleRunner &theScheduleRunner = 
        repast::RepastProcess::instance()->getScheduleRunner();
	return theScheduleRunner.currentTick();
}

int Model::getPopulation()
{
	return population;
}

double Model::getTotalWealth()
{
	return totalWealth;
}

double Model::getWealthRedistributed()
{
	return wealthRedistributed;
}

double Model::calculatePercentWealthRedistributed()
{
	calculateTotalWealth();
	double percent=wealthRedistributed/totalWealth;
	return percent;
}

//Print things out
//============================================================
void Model::printGini() {
	std::cout << adultWealthGiniCoefficient() << ',' <<
		satisfactionGiniCoefficient() << ',' <<
		population<< ',' <<
		numOmniEvents<< ',' <<
		calculatePercentWealthRedistributed() << ',' <<
		wealthGiniCoefficient() << ',' <<
		yearlyTrades<< std::endl;
}

void Model::printCommodityStats(std::ostream & os) const {

//cout << "printing all stats!" << endl;
    //repast::SharedContext<Human>::const_local_iterator actorIter = 
        //actors.localBegin();
		//os << "DEFICIENT,SATISFIED,BLOATED,COMMUNITY,SALARY,TIMES_TRADED,INTROVERSION" << std::endl;
    //while (actorIter != actors.localEnd()) {
	for(int k=0; k<Commodity::NUM_COMM; k++)
	{
		//os << Commodity::getCommNum(k).getTotalAmt() << std::endl;
	}
	for(int i=0; i<Model::COMMUNITIES; i++)
	{
		for(int j=0; j<communities[i].size()/*Model::instance()->getCommunityMembers(i).size()*/; j++)
		{
		Human * man = Model::instance()->getCommunityMembers(i)[j];
		os <</* (*actorIter)->getNumDeficientCommodities() << "," <<*/
              ((*man).getNumSatisfiedCommodities() +
              (*man).getNumBloatedCommodities()) << ","<<
			  (*man).getCommunity() << "," <<
			  (*man).getSalary()<< "," << 
		//	  (*man).getMake()<< "," <<
			  (*man).getTimesTraded()<< "," <<
			  (*man).getWealth() << std::endl;
		//	  (*actorIter)->getNeeds()<< "," <<
			  
		//	  Model::INTROVERT_DIAL<< std::endl;
/*
		std::cout<<(*actorIter)->getId() << " has these totals: " <<
			  (*actorIter)->getNumDeficientCommodities() << "," <<
              (*actorIter)->getNumSatisfiedCommodities() << "," <<
              (*actorIter)->getNumBloatedCommodities() << endl;
cout << *(*actorIter) << endl;
*/
        //actorIter++;
		}
    }
}

void Model::printCommunityStats(std::ostream & os) const
{
	for(int i=0; i<Commodity::NUM_COMM; i++)
	{
		os << "Community " << i << " has " << getCommunitySize(i) <<
			" members and an average of " << getAvgDeficientCommComm(i) <<
			" deficient commodities\n";
	}
}

//Random number generators
//============================================================
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
	return deathDistro->next();
}

double Model::generateConsume() {
	return consumeDistro->next();
}

int Model::generateNumTraders() {
	return tradeDistro->next();
}

int Model::generateOutsideTrade() {
	return outsideTrade->next();
}

int Model::generateCommunity(Human * toAdd) {
	int randomCommunity = communityDistro->next();
	randomCommunity = randomCommunity%Model::COMMUNITIES;
	communities[randomCommunity].push_back(toAdd);
	return randomCommunity;
}

int Model::generateChild() {
	return childDistro->next();
}

int Model::generateAge() {
	return ageDistro->next();
}

//Obsolete items
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

//Private functions
Model::Model()
{
    // Initialize random number distributions.
	repast::Random::instance()->initialize(SEED);
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
	deathDistro =		
		repast::Random::instance()->getGenerator("death");
	consumeDistro =
		repast::Random::instance()->getGenerator("consume");
	tradeDistro =
		repast::Random::instance()->getGenerator("trade");
	outsideTrade =
		repast::Random::instance()->getGenerator("outside");
	communityDistro =
		repast::Random::instance()->getGenerator("community");
	childDistro =
		repast::Random::instance()->getGenerator("children");
	ageDistro=
		repast::Random::instance()->getGenerator("initialAge");
	fillCommunities();
	yearlyTrades=0;
	yearlyAmountTraded=0;
	numOmniEvents=0;
	totalWealth=0;
	wealthRedistributed=0;
	population=NUM_INITIAL_AGENTS;
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
        theScheduleRunner.scheduleEvent(1.1, repast::Schedule::FunctorPtr(
            new repast::MethodFunctor<Human>(newHuman, &Human::earnIncome)));
        theScheduleRunner.scheduleEvent(1.2, repast::Schedule::FunctorPtr(
            new repast::MethodFunctor<Human>(newHuman, &Human::tradeWithRandomAgents)));
		//Print the stats before consuming
	/*	ofstream statsBeforeConsume;
		statsBeforeConsume.open("statsBeforeConsume.txt");
        theScheduleRunner.scheduleEvent(1.25, 1, repast::Schedule::FunctorPtr(
			new repast::MethodFunctor<Model>(this, &Model::printCommodityStats(statsBeforeConsume))));
*/
        theScheduleRunner.scheduleEvent(1.3, repast::Schedule::FunctorPtr(
            new repast::MethodFunctor<Human>(newHuman, &Human::consume)));
        theScheduleRunner.scheduleEvent(1.31, repast::Schedule::FunctorPtr(
            new repast::MethodFunctor<Human>(newHuman, &Human::considerHavingAChild)));
        theScheduleRunner.scheduleEvent(1.32, repast::Schedule::FunctorPtr(
            new repast::MethodFunctor<Human>(newHuman, &Human::considerDeath)));
		
    }
    theScheduleRunner.scheduleEvent(1.4, 1, repast::Schedule::FunctorPtr(
		new repast::MethodFunctor<Model>(this, &Model::printGini)));
    theScheduleRunner.scheduleEvent(1.5, 1, repast::Schedule::FunctorPtr(
		new repast::MethodFunctor<Model>(this, &Model::resetTrades)));	
    theScheduleRunner.scheduleEvent(1.6, 1, repast::Schedule::FunctorPtr(
		new repast::MethodFunctor<Model>(this, &Model::resetTradedAmount)));
    theScheduleRunner.scheduleEvent(1.7, 1, repast::Schedule::FunctorPtr(
		new repast::MethodFunctor<Model>(this, &Model::resetOmniEvent)));	
    theScheduleRunner.scheduleEvent(1.8, 1, repast::Schedule::FunctorPtr(
		new repast::MethodFunctor<Model>(this, &Model::resetTotalWealth)));	
    theScheduleRunner.scheduleEvent(1.9, 1, repast::Schedule::FunctorPtr(
		new repast::MethodFunctor<Model>(this, &Model::resetWealthRedistributed)));	
}

void Model::startYear() {
    repast::ScheduleRunner &theScheduleRunner = 
        repast::RepastProcess::instance()->getScheduleRunner();
    //cout << "======================================================" << endl;
    //cout << "Now starting year " <<  theScheduleRunner.currentTick() << " with a population of " << population << "!" << endl;
}

/*
 * Dumbly and mechanically translated to C++ from R function in reldist 
 *   library; see http://rss.acs.unt.edu/Rdoc/library/reldist/html/gini.html.
 *   - SD
 */
double Model::computeGini(vector<double> values) const {

    int n = values.size();
    vector<double> p(n);
    vector<double> nu(n);
    vector<double> wx(n);
    vector<double> first(n-1);
    vector<double> second(n-1);

    sort(values.begin(), values.end());
    
    for (int i=0; i<n; i++) {
        p[i] = ((double)i+1)/n;
    }
    for (int i=0; i<n; i++) {
        wx[i] = values[i]/n;
    }
    nu[0] = wx[0];
    for (int i=1; i<n; i++) {
        nu[i] = nu[i-1] + wx[i];
    }
    for (int i=0; i<n; i++) {
        nu[i] /= nu[n-1];
    }
    double sum_first = 0.0;
    for (int i=0; i<n-1; i++) {
        first[i] = nu[i+1] * p[i];
        sum_first += first[i];
    }
    double sum_second = 0.0;
    for (int i=0; i<n-1; i++) {
        second[i] = nu[i] * p[i+1];
        sum_second += second[i];
    }

    return sum_first - sum_second;
}
