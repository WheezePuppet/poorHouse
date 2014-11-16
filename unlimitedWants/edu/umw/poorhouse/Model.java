package edu.umw.poorhouse;

import sim.util.distribution.*;
import sim.engine.*;
import java.util.*;
import java.io.PrintWriter;
import java.io.FileWriter;
import ec.util.MersenneTwisterFast;

public class Model extends SimState implements Steppable
{
        //Create singleton model
        private static Model theInstance;

        //Global constants
        public static int NUM_INITIAL_AGENTS = 100;
        public static int NUM_YEARS;// = 200;
        public static int INTROVERT_DIAL;
        public static long SEED = 0;
        public static final int COMMUNITIES = 10;
        public static int SWITCH_PROZ=0;
        public static int NUM_TRADERS = 0;
        public static int COMM_CONSUME_MEAN = 3;
        public static double PRODUCTION_MEAN = 30;
        public static double MONEY = 100;
        public static long SIM_TAG;

        //Output control
        static PrintWriter commoditiesFile;
        static final String COMM_FILE_NAME = "/tmp/comm_statsSIMTAG.csv";
        static PrintWriter simStatsFile;
        static final String SIM_STATS_FILE_NAME = "/tmp/sim_statsSIMTAG.csv";


        //Random number generator next functions
        public double generateNeedCommodityThreshold() {
                return commodityNeedThresholdDistro.nextDouble(); }
        public double generateAmountProduced() {
                double thing=amountProducedDistro.nextDouble();
                while(thing<0) {
                        thing=amountProducedDistro.nextDouble();
                }
                return thing;
        }
        //public int generateMake() { return makeDistro.nextInt(); }
        public int generateMake(Human toAdd) {
                int make = makeDistro.nextInt();
                //make = make%Commodity.;//TODO
                //producers.get(make).add(toAdd);
                return make;
        }
        public double generateMps() { return mpsDistro.nextDouble(); }
        public int generateLifeProb() { 
                return (years < NUM_YEARS) ? probDistro.nextInt() : 101;
        }//Make this a double?
        public double generateConsume() { return consumeDistro.nextDouble(); }
        public double generateExpPrice() { return priceDistro.nextDouble(); }

        public double generateChokeQuant() { return chokeQuantDistro.nextDouble(); }
        public double generateDemandSlope() { return demandSlopeDistro.nextDouble(); }

        public double generateAlpha() { return alphaDistro.nextDouble(); }
        public double generateBeta() { return betaDistro.nextDouble(); }

        public int generateNumTraders() { return probDistro.nextInt(); }
        public int generateOutsideTrade() { return probDistro.nextInt(); }//Change to double TODO On second thought, it's the introvert dial
        public int generateCommunity(Human toAdd) {
                int randomCommunity = 0;//communityDistro.nextInt();
                randomCommunity = randomCommunity%COMMUNITIES;
                communities.get(randomCommunity).add(toAdd);
                return randomCommunity;
        }
        public int generateChild() { return probDistro.nextInt(); }//Changed to double
        public int generateAge() { return ageDistro.nextInt(); }
        public int generateSwitch() { return probDistro.nextInt(); }

        //Trade facilitation functions
        //--------------------------------------------------------------------------
        /* Give the trading agent a random member of their own community to
           trade with */
        public Human getRandomCommunityMember(int communityNum) {
                int indexOfRandomMember = probDistro.nextInt();
                indexOfRandomMember = indexOfRandomMember%(communities.get(communityNum).size());
                return communities.get(communityNum).get(indexOfRandomMember);
        }

        /* Give the trading agent a random member of the world to trade with */
        public Human getRandomGlobalMember() {
                int big = actors.size();
                Enumeration<Integer> allKeys = actors.keys();
                int randomHash = probDistro.nextInt()%big;
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

        public void inter(Human body) {
                graveyard.add(body);
                int h=communities.get(body.getCommunity()).size();
                //System.out.println("comm size is " + h);
                for(int i=0; i<h-1; i++) {
                        if(communities.get(body.getCommunity()).get(i)==body) {
                                communities.get(body.getCommunity()).remove(i);
                        }
                }
        }
        public void addToActors(Human man) {
                actors.put(
                                man.getId(),man);
        }
        public void addToCommunity(int i, Human man) { communities.get(i).add(man); }
        public void addToProducers(int good, Human man) { producers.get(good).add(man); }
        public void removeFromProducers(int good, Human man) { producers.get(good).remove(man); }
        public int getNumProducers(int good) {return producers.get(good).size(); }
        public void showNumOfProducers(){
                for(int i=0; i<Commodity.NUM_COMM; i++){
                        System.out.printf("%d,", producers.get(i).size());
                }
                System.out.println(" ");
        }

        public boolean findProducer(int good, Human man) { return producers.get(good).contains(man); }
        public boolean noProducers(int good) {
                return !(producers.get(good).size()>0);
        }
        public Human getProducerOfGood(int good){ return producers.get(good).get(probDistro.nextInt()%(producers.get(good).size())); }//TODO if none, raise price

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

        //Returns the average price of a commodity
        public double avgPrice(int comm){
                int sizeOfMap = actors.size();
                double avg = 0;
                for(Map.Entry<Integer, Human> entry : actors.entrySet()){
                        Integer key = entry.getKey();
                        Human value = entry.getValue();
                        avg += value.getPrice(comm);
                }
                avg /= sizeOfMap;
                return avg;
        }

        //Returns the standard deviation of the price of a commodity
        public double sdPrice(int comm){
                double avg = avgPrice(comm);
                double sd=0;
                for(Map.Entry<Integer, Human> entry : actors.entrySet()){
                        Integer key = entry.getKey();
                        Human value = entry.getValue();
                        double diff = (value.getPrice(comm) - avg);
                        sd += (diff * diff);
                }
                sd /= actors.size();
                sd = Math.sqrt(sd);
                double normalized = sd/avgPrice(comm);
                normalized*=20;
                return normalized;
        }

        public void resetTotalWealth() { totalWealth=0; }
        public void addToWealthRedistributed(double value) { wealthRedistributed+=value; }
        public void resetWealthRedistributed() { wealthRedistributed=0; }

        //Modeling software things
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
        }

        public void fillCommunities() {
                for(int i=0; i<COMMUNITIES; i++) {
                        communities.add(new ArrayList<Human>());
                }
        } 

        public void fillProducers() {
                for(int i=0; i<Commodity.NUM_COMM; i++) {
                        producers.add(new ArrayList<Human>());
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
                System.out.printf("%d,%d\n",
                                population,
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
                randomGenerator = new MersenneTwisterFast(50);
                commodityNeedThresholdDistro = 
                        new Uniform(1.0,5.0,randomGenerator);
                amountProducedDistro = new Normal(PRODUCTION_MEAN,15,randomGenerator);
                makeDistro = new Uniform(0,Commodity.NUM_COMM-1,randomGenerator);//TODO
                consumeDistro = new Uniform(COMM_CONSUME_MEAN-2,
                                COMM_CONSUME_MEAN+2,randomGenerator);
                priceDistro = new Uniform(1,5,randomGenerator);
                ageDistro = new Uniform(0,29,randomGenerator);
                communityDistro = new Uniform(1,100,randomGenerator);
                probDistro = new Uniform(0,100,randomGenerator);
                chokeQuantDistro = new Uniform(50,100,randomGenerator);
                demandSlopeDistro = new Uniform(1,5,randomGenerator);

                alphaDistro = new Uniform(5,10,randomGenerator);
                betaDistro = new Uniform(1,5,randomGenerator);
                /*deathDistro = new Uniform(0,100,randomGenerator);
                  outsideTrade = new Uniform(0,100,randomGenerator);
                  tradeDistro = new Uniform(0,100, randomGenerator);
                  childDistro = new Uniform(0,100,randomGenerator);*/

                communities = new ArrayList<ArrayList<Human>>();
                producers = new ArrayList<ArrayList<Human>>();
                fillCommunities();
                fillProducers();
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
                for (int i=0; i<NUM_INITIAL_AGENTS; i++) {
                        //add agent to hash table
                        Human newHuman = new Human();
                        schedule.scheduleOnce(.1,newHuman);
                }
                System.out.println("Total amount produced: " + Human.totalAmountProduced);
        }

        public void step(SimState model) {
                //printGini();
                System.out.printf("The total money in the system is %f.\n",Human.totalMoney);
                System.out.printf("The total money spent this round was %f.\n",Human.totalSpent);
                //System.out.printf("%f\n",generateConsume());
                Human.totalMoney = 0;
                Human.totalSpent = 0;
                Human.buyers = 0;
                resetTrades();
                resetTradedAmount();
                resetOmniEvent();
                resetTotalWealth();
                resetWealthRedistributed();
                years++;
                double foodAmt = Commodity.getCommNum(1).getTotalAmt();
                double foodAvgPrice = avgPrice(1);
                double foodPriceSd = sdPrice(1);
                double totalConsForAllCommoditiesThisRound = 0;
                for(int i=0; i<Commodity.NUM_COMM; i++){
                    commoditiesFile.printf("%d, ",years);
                    commoditiesFile.printf("%f, ", avgPrice(i));
                    commoditiesFile.printf("%f, ", sdPrice(i));
                    commoditiesFile.printf("%f, ", Commodity.getCommNum(i).getAmtCons());
                    commoditiesFile.printf("%c\n",i+65);
                    totalConsForAllCommoditiesThisRound += Commodity.getCommNum(i).getTotalCons();
                }
                System.out.println("TOTAL consumed: " + 
                    totalConsForAllCommoditiesThisRound + " of " + 
                    Commodity.theoreticalTotalOfAllConsumption*100);
                commoditiesFile.flush();
                simStatsFile.println("" + years + "," +
                                totalConsForAllCommoditiesThisRound + "," + 
                                Commodity.theoreticalTotalOfAllConsumption*100);
                simStatsFile.flush();
                for(int i=0; i<Commodity.NUM_COMM; i++){
                        Commodity.getCommNum(i).resetCons();
                }
                if(years > NUM_YEARS){
                        System.exit(0);
                }
                if(population >0 && years < NUM_YEARS){
                        schedule.scheduleOnceIn(1,this);
                }
        }

        //Agent containers
        private Hashtable<Integer,Human> actors;
        private ArrayList<ArrayList<Human>> communities;
        private ArrayList <Human> graveyard;
        private ArrayList<ArrayList<Human>> producers;

        //Random number generator declarations
        private MersenneTwisterFast randomGenerator;
        private Uniform commodityNeedThresholdDistro;
        private Normal amountProducedDistro;
        private Uniform makeDistro;
        private Uniform mpsDistro;
        private Uniform consumeDistro;
        private Uniform priceDistro;
        private Uniform communityDistro;
        private Uniform ageDistro;
        private Uniform probDistro;
        private Uniform chokeQuantDistro;
        private Uniform demandSlopeDistro;
        private Uniform alphaDistro;
        private Uniform betaDistro;
        //private Uniform deathDistro;
        //private Uniform tradeDistro;
        //private Uniform outsideTrade;
        //private Uniform childDistro;

        // Usage: java Model [-numYears numYears] [-simtag simtag] 
        // [-switchPerc switchPerc] 
        //
        // numYears: the number of periods to run the simulation.
        // simtag: an integer identifying a particular run of the simulation.
        //    This is used to keep separate simultaneous simulations separate.
        //    You may safely set it to 1 (or anything else). The number you
        //    specify will be present in the filenames of the output files the
        //    simulation produces in /tmp.
        // switchPerc: integer (0-100) indicating percent likelihood
        //    each time period that an agent will change what commodity they 
        //    produce.
        // 
        // Examples:
        //   java Model -numYears 200 -simtag 9999 -switchPerc 25
        public static void main(String args[]) {
                doLoop(new MakesSimState() {
                                public SimState newInstance(long seed, String[] args) {
                                try {
                                for (int i=0; i<args.length; i++) {
                                if (args[i].equals("-numYears")) {
                                NUM_YEARS = Integer.valueOf(args[++i]);
                                }
                                if (args[i].equals("-simtag")) {
                                SIM_TAG = Integer.valueOf(args[++i]);
                                }
                                if (args[i].equals("-switchPerc")) {
                                SWITCH_PROZ = Integer.valueOf(args[++i]);
                                }
                                if (args[i].equals("-avgProd")) {
                                PRODUCTION_MEAN=Double.parseDouble(args[++i]);
                                }
                                if (args[i].equals("-money")) {
                                MONEY=Double.parseDouble(args[++i]);
                                }
                                }

                                //Model.NUM_TRADERS=Integer.parseInt(args[1]);
                                //Model.COMM_CONSUME_MEAN=Integer.parseInt(args[2]);
                                //Model.SALARY_MEAN=Integer.parseInt(args[3]);
                                //Model.NUM_INITIAL_AGENTS=Integer.parseInt(args[4]);

                                commoditiesFile = new PrintWriter(new FileWriter(
                                                        COMM_FILE_NAME.replace("SIMTAG","" + SIM_TAG)));
                                commoditiesFile.println("\"year\",\"avg price\",\"sd\",\"totalCons\",\"commodity\"");
                                simStatsFile = new PrintWriter(new FileWriter(
                                                        SIM_STATS_FILE_NAME.replace("SIMTAG","" + 
                                                                SIM_TAG)));
                                simStatsFile.println(
                                                "\"year\",\"totalConsumed\",\"theoreticalMax\"");
                                } catch (Exception e) {
                                        e.printStackTrace();
                                        printUsage();
                                        System.exit(1);
                                }
                                return instance();
                                }
                                public Class simulationClass() {
                                        return Model.class;
                                }
                }, args);
        }

        private static void printUsage() {
                System.err.println("Usage: java Model [-numYears numYears]" +
                                "   [-simtag simtag]" +
                                "   [-switchPerc switchPerc].");
        }
}
