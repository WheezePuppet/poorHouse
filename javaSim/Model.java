//package jsim;

import sim.util.distribution.*;
import sim.engine.*;
import java.util.*;
import ec.util.MersenneTwisterFast;

public class Model extends SimState implements Steppable
{
        //Create singleton model
        private static Model theInstance = instance();

        //Global constants
        public static final int NUM_INITIAL_AGENTS = 100;
        public static final int NUM_YEARS = 100;
        public static int INTROVERT_DIAL;// = 50;
        public static long SEED = 0;
        public static int LEMMINGNESS = 100;
        public static final int COMMUNITIES = 10;

        //Random number generator next functions
        public double generateNeedCommodityThreshold() { return commodityNeedThresholdDistro.nextDouble(); }
        public double generateWantCommodityThreshold() { return commodityWantThresholdDistro.nextDouble(); }
        public double generateSalary() {
                double thing=salaryDistro.nextDouble();
                while(thing<0)
                {
                        thing=salaryDistro.nextDouble();
                }
                return thing;
        }
        public int generateMake() { return makeDistro.nextInt(); }
        public double generateMps() { return mpsDistro.nextDouble(); }
        public int generateLifeProb() { 
            return (years < NUM_YEARS) ? deathDistro.nextInt() : 101;
             }
        public double generateConsume() { return consumeDistro.nextDouble(); }
        public int generateNumTraders() { return tradeDistro.nextInt(); }
        public int generateOutsideTrade() { return outsideTrade.nextInt(); }
        public int generateCommunity(Human toAdd) {
                int randomCommunity = communityDistro.nextInt();
                randomCommunity = randomCommunity%COMMUNITIES;
                communities.get(randomCommunity).add(toAdd);
                return randomCommunity;
        }
        public int generateChild() { return childDistro.nextInt(); }
        public int generateAge() { return ageDistro.nextInt(); }

        //Gini functions
//--------------------------------------------------------------------------------
        /* Find the gini coefficient of a collection of doubles */
        private double computeGini(ArrayList<Double> vals) {
                int n = vals.size();
                double [] p = new double[n];
                double [] nu = new double[n];
                double [] wx = new double[n];
                double [] first = new double[n];
                double [] second = new double[n];
                java.util.Collections.sort(vals);
                for (int i=0; i<n; i++) {
                        p[i] = ((double)i+1)/n;
                }
                for (int i=0; i<n; i++) {
                        wx[i] = vals.get(i)/n;
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

        /* Return gini of the total commodity amounts held by all agents */
        public double wealthGiniCoefficient() {
                ArrayList<Double> wealths = new ArrayList<Double>();//Total amount of goods held by each agent
                for(int i=0; i<COMMUNITIES; i++) {
                        for(int j=0; j<Model.instance().getCommunityMembers(i).size(); j++) {
                                Human man = Model.instance().getCommunityMembers(i).get(j);
                                wealths.add(man.getWealth());
                        }
                }
                return computeGini(wealths);
        }

        /* Return gini of the total commodity amounts held by all agents over
        the age of twenty*/
        public double adultWealthGiniCoefficient() {
                ArrayList<Double> wealths = new ArrayList<Double>();
                for(int i=0; i<COMMUNITIES; i++) {
                        for(int j=0; j<Model.instance().getCommunityMembers(i).size(); j++) {
                                Human man = Model.instance().getCommunityMembers(i).get(j);
                                if((man.getAge())>20) {
                                        wealths.add(man.getWealth());
                                }
                        }
                }
                return computeGini(wealths);
        }

        /* Return gini of total number of goods each agent is satisfied or
        bloated in */
        public double satisfactionGiniCoefficient() {
                ArrayList<Double> satisfactions = new ArrayList<Double>();
                for(int i=0; i<COMMUNITIES; i++) {
                        for(int j=0; j<Model.instance().getCommunityMembers(i).size(); j++) {
                                Human man = Model.instance().getCommunityMembers(i).get(j);
                                satisfactions.add(man.getSatisfaction());
                        }
                }
                return computeGini(satisfactions);
        }

        //Trade facilitation functions
//--------------------------------------------------------------------------------
        /* Give the trading agent a random member of their own community to
        trade with */
        public Human getRandomCommunityMember(int communityNum) {
                int indexOfRandomMember = tradeDistro.nextInt();
                indexOfRandomMember = indexOfRandomMember%(communities.get(communityNum).size());
                return communities.get(communityNum).get(indexOfRandomMember);
        }

        /* Give the trading agent a random member of the world to trade with */
        public Human getRandomGlobalMember() {
                //std::vector<Human *> oneAgent;
                //ArrayList<Human> oneAgent = new ArrayList<Human>();
                //actors.getRandomAgents(1,oneAgent);
                int big = actors.size();
                Enumeration<Integer> allKeys = actors.keys();
                int randomHash = tradeDistro.nextInt()%big;
                int toTake = 0;
                for(int i=0; i<randomHash; i++) {
                    toTake = allKeys.nextElement();
                }
                Human oneAgent = actors.get(toTake);
                return oneAgent;
        }

        public double getAvgDeficientCommComm(int commuNum) {
                int i=getCommunitySize(commuNum);
                double total=0;
                for(int j=0; j<i; j++) {
                        total += ((getCommunityMembers(commuNum)).get(j)).getNumDeficientCommodities();
                }
                total/=i;
                return total;
        }
        //
        public void inter(Human body) {
                graveyard.add(body);
                int h=communities.get(body.getCommunity()).size();
                //System.out.println("comm size is " + h);
                for(int i=0; i<h-1; i++) {
                        if(communities.get(body.getCommunity()).get(i)==body) {
                                //communities.get(body.getCommunity()).remove((communities.get(body.getCommunity()).begin())+i);
                                communities.get(body.getCommunity()).remove(i);
                        }
                }
        }
        public void addToActors(Human man) {
                 actors.put(
                    man.getId(),man);
                 }
        public void addToCommunity(int i, Human man) { communities.get(i).add(man); }

        //Keep track of Model variables
        public void incrementTrades() { yearlyTrades++; }
        public void resetTrades() { yearlyTrades=0; }
        public void addToTradedAmount(double change) { yearlyAmountTraded+=change; }
        public void resetTradedAmount() { yearlyAmountTraded=0; }
        public void incrementPopulation() { population++; }
        public void decrementPopulation() { population--; }
        public void incrementOmniEvent() { numOmniEvents++; }
        public void resetOmniEvent() { numOmniEvents=0; }
        public void calculateTotalWealth() {
                for(int i=0; i<COMMUNITIES; i++) {
                        for(int j=0; j<communities.get(i).size(); j++) {
                                totalWealth+=communities.get(i).get(j).getWealth();
                        }
                }
        }

        public void resetTotalWealth() { totalWealth=0; }
        public void addToWealthRedistributed(double value) { wealthRedistributed+=value; }
        public void resetWealthRedistributed() { wealthRedistributed=0; }

        //Repast things
        //public virtual repast::AgentId & getId() { return myId; }
        public static Model instance(){
                if(theInstance == null){
                    theInstance = new Model(SEED);
                }
                return theInstance;
        }

        public void start() {
                Commodity.initCommodities();
                createInitialAgents();
                schedule.scheduleOnce(1,this);

                // Schedule an end point.
                //theScheduleRunner.scheduleStop(NUM_YEARS);

                // Let's DO THIS THING!!!
                //theScheduleRunner.run();
        }

        public void fillCommunities() {
                //System.out.println("filling com");
                if(communities != null){
                    //System.out.println("not com");
                    //System.out.println(COMMUNITIES);
                }
                for(int i=0; i<COMMUNITIES; i++) {
                        communities.add(new ArrayList<Human>());
                }
        } 

        //Return model things

        public ArrayList<Human> getCommunityMembers(int communityNum) {
                return communities.get(communityNum);
        }

        public ArrayList<Human> getGraveyard() {
                return graveyard;
        }
        public int getCommunitySize(int communityNum) { return communities.get(communityNum).size(); }
        public double getTick() {
                return schedule.getTime();
        }

        public int getPopulation() { return population; }
        public double getTotalWealth() { return totalWealth; }
        public double getWealthRedistributed() { return wealthRedistributed; }
        public double calculatePercentWealthRedistributed() {
                calculateTotalWealth();
                double percent=wealthRedistributed/totalWealth;
                return percent;
        }

        //Print things out
        public void printGini() {
                System.out.printf("%f,%f,%d,%d,%f,%f,%d\n",
                                adultWealthGiniCoefficient(),
                                satisfactionGiniCoefficient(),
                                population,
                                numOmniEvents,
                                calculatePercentWealthRedistributed(),
                                wealthGiniCoefficient(),
                                yearlyTrades);

        }

        public void printCommunityStats(/*std::ostream & os*/) {
                for(int i=0; i<Commodity.NUM_COMM; i++)
                {
                        /*os << "Community " << i << " has " << getCommunitySize(i) <<
                          " members and an average of " << getAvgDeficientCommComm(i) <<
                          " deficient commodities\n";*/
                }
        }

        //Private things
        //private repast::AgentId myId;

        //Trackers
        private int population;
        private double yearlyAmountTraded;
        private int yearlyTrades;
        private int numOmniEvents;
        private double totalWealth;
        private double wealthRedistributed;
        private int years;

        //Functions
        private Model(long seed) {
                // Initialize random number distributions.
                super(seed);
                randomGenerator = new MersenneTwisterFast();
                commodityNeedThresholdDistro = new Uniform(1.0,5.0,randomGenerator);
                commodityWantThresholdDistro = new Uniform(6.0,10.0,randomGenerator);
                salaryDistro = new Normal(35,15,randomGenerator);
                makeDistro = new Uniform(0,9,randomGenerator);
                deathDistro = new Uniform(0,100,randomGenerator);
                mpsDistro = new Uniform(.1,.7,randomGenerator);
                consumeDistro = new Uniform(1,4,randomGenerator);
                tradeDistro = new Uniform(0,100,randomGenerator);
                outsideTrade = new Uniform(0,100,randomGenerator);
                communityDistro = new Uniform(1,100,randomGenerator);
                childDistro = new Uniform(0,100,randomGenerator);
                ageDistro = new Uniform(0,29,randomGenerator);

                communities = new ArrayList<ArrayList<Human>>();
                fillCommunities();
                actors = new Hashtable<Integer,Human>();
                graveyard = new ArrayList<Human>();
                yearlyTrades=0;
                yearlyAmountTraded=0;
                numOmniEvents=0;
                totalWealth=0;
                wealthRedistributed=0;
                population=NUM_INITIAL_AGENTS;
                years = 0;
        }
        private void createInitialAgents() {

                // Create the initial generation of agents, and add them to the Context.
                // Schedule the startYear method to run at integer clock ticks (start
                //   of year, say.
                // While we're at it, schedule all agents to run on the following
                //   schedule:
                // - On integer + .1, earn income.
                // - At integer + .2, trade with other random agents.
                // - At integer + .3, consume commodities for the year.
                for (int i=0; i<NUM_INITIAL_AGENTS; i++) {
                        //add agent to hash table
                        Human newHuman = new Human();
                        schedule.scheduleOnce(.1,newHuman);
                }
        }

        public void step(SimState model) {
                printGini();
                resetTrades();
                resetTradedAmount();
                resetOmniEvent();
                resetTotalWealth();
                resetWealthRedistributed();
                years++;
                if(population >0 && years < NUM_YEARS){
                    schedule.scheduleOnceIn(1,this);
                }
        }

        private void startYear() {
                //repast::ScheduleRunner &theScheduleRunner = 
                //repast::RepastProcess::instance()->getScheduleRunner();
                //cout << "======================================================" << endl;
                //cout << "Now starting year " <<  theScheduleRunner.currentTick() << " with a population of " << population << "!" << endl;
        }

        //Agent containers
        private Hashtable<Integer,Human> actors;
        private ArrayList<ArrayList<Human> > communities;
        private ArrayList <Human> graveyard;

        //Random number generator declarations
        private MersenneTwisterFast randomGenerator;
        private Uniform commodityNeedThresholdDistro;
        private Uniform commodityWantThresholdDistro;
        private Normal salaryDistro;
        private Uniform makeDistro;
        private Uniform deathDistro;
        private Uniform mpsDistro;
        private Uniform consumeDistro;
        private Uniform tradeDistro;
        private Uniform outsideTrade;
        private Uniform communityDistro;
        private Uniform childDistro;
        private Uniform ageDistro;

        public static void main(String args[]) {
                if(args.length<3){
                    System.out.println("You need DIAL, SEED, and BEQ");
                    System.exit(1);
                }
                doLoop(new MakesSimState() {
                                public SimState newInstance(long seed, String[] args) {
                                return instance();
                                }
                                public Class simulationClass() {
                                return Model.class;
                                }
                                }, args);
        }
}
