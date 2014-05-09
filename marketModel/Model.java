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
        public static final int NUM_YEARS = 15;
        public static int INTROVERT_DIAL;
        public static long SEED = 0;
        public static final int COMMUNITIES = 10;

        //Random number generator next functions
        public int generateNeedCommodityThreshold() {
            return commodityNeedThresholdDistro.nextDouble(); }
        public double generateSalary() {
                double thing=salaryDistro.nextDouble();
                while(thing<0) {
                        thing=salaryDistro.nextDouble();
                }
                return thing;
        }
        public int generateMake() { return makeDistro.nextInt(); }
        public double generateMps() { return mpsDistro.nextDouble(); }
        public int generateLifeProb() { 
            return (years < NUM_YEARS) ? deathDistro.nextInt() : 101;
             }//Make this a double?
        public double generateConsume() { return consumeDistro.nextDouble(); }
        public double generateExpPrice() { return priceDistro.nextDouble(); }
        public int generateNumTraders() { return tradeDistro.nextInt(); }
        public int generateOutsideTrade() { return outsideTrade.nextInt(); }//Change to double TODO On second thought, it's the introvert dial
        public int generateCommunity(Human toAdd) {
                int randomCommunity = communityDistro.nextInt();
                randomCommunity = randomCommunity%COMMUNITIES;
                communities.get(randomCommunity).add(toAdd);
                return randomCommunity;
        }
        public int generateChild() { return childDistro.nextInt(); }//Changed to double
        public int generateAge() { return ageDistro.nextInt(); }

        //Trade facilitation functions
//--------------------------------------------------------------------------
        /* Give the trading agent a random member of their own community to
        trade with */
        public Human getRandomCommunityMember(int communityNum) {
                int indexOfRandomMember = tradeDistro.nextInt();
                indexOfRandomMember = indexOfRandomMember%(communities.get(communityNum).size());
                return communities.get(communityNum).get(indexOfRandomMember);
        }

        /* Give the trading agent a random member of the world to trade with */
        public Human getRandomGlobalMember() {
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
            return sd;
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
                commodityNeedThresholdDistro = new Uniform(1.0,5.0,randomGenerator);
                salaryDistro = new Normal(35,15,randomGenerator);
                makeDistro = new Uniform(0,9,randomGenerator);
                deathDistro = new Uniform(0,100,randomGenerator);
                mpsDistro = new Uniform(.1,.7,randomGenerator);
                consumeDistro = new Uniform(1,5,randomGenerator);
                priceDistro = new Uniform(1,5,randomGenerator);
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
                for (int i=0; i<NUM_INITIAL_AGENTS; i++) {
                        //add agent to hash table
                        Human newHuman = new Human();
                        schedule.scheduleOnce(.1,newHuman);
                }
        }

        public void step(SimState model) {
                //printGini();
                resetTrades();
                resetTradedAmount();
                resetOmniEvent();
                resetTotalWealth();
                resetWealthRedistributed();
                years++;
                double foodAmt = Commodity.getCommNum(1).getTotalAmt();
                double foodAvgPrice = avgPrice(1);
                double foodPriceSd = sdPrice(1);
                System.out.printf("amount produced/need, avg price, sd, char\n year: %d\n", years);
                for(int i=0; i<Commodity.NUM_COMM; i++){
                    System.out.printf("%f, %f, %f, %c\n",(Commodity.getCommNum(i).getProducedQuantity()/Commodity.getCommNum(i).getAmtNeeded()), avgPrice(i), sdPrice(i), i+65);
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
        private ArrayList<ArrayList<Human> > communities;
        private ArrayList <Human> graveyard;

        //Random number generator declarations
        private MersenneTwisterFast randomGenerator;
        private Uniform commodityNeedThresholdDistro;
        private Normal salaryDistro;
        private Uniform makeDistro;
        private Uniform deathDistro;
        private Uniform mpsDistro;
        private Uniform consumeDistro;
        private Uniform priceDistro;
        private Uniform tradeDistro;
        private Uniform outsideTrade;
        private Uniform communityDistro;
        private Uniform childDistro;
        private Uniform ageDistro;

        public static void main(String args[]) {
                if(args.length<3){
                    System.out.println("You need DAL, SEED, and BEQ");
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
