//package jsim;
package edu.umw.poorhouse;

import java.util.*;
import sim.util.*;
import sim.engine.*;
import static java.lang.Math.pow;

public class Human implements Steppable {
        /*What is the relation of the agent's held commodities to their
          need level*/
        public enum CommodityStatus { DEFICIENT, SATISFIED };
        //What stage of the year an agent is in
        public enum LifeStage { EARNING, TRADING, CONSUMING, BIRTHING, DYING }; 
        private boolean ALL = false;
        private boolean TRADE = false;
        private boolean PRICE = false;
        private int GOOD = 2;
        public static double totalMoney = 0;
        public static double totalSpent = 0;
        private double changeProp = 1.01;
        private int buyerChange = 10;
        public static int buyers = 0;

        private void debug(String message, boolean cond){
                if(cond || ALL){
                        System.out.println(message);
                }
        }

        public String commodityStatusString(CommodityStatus cs) {
                switch (cs) {
                        case DEFICIENT:
                                return "DEFICIENT";
                        case SATISFIED:
                                return "SATISFIED";
                }
                return "SOMETHING IS DREADFULLY WRONG!!!!!";
        }

        //Global static values
        public static int BEQ;

        //Scheduled events
        //--------------------------------------------------------------------------
        //Increase the agent's salary commodity by his amountProduced and inform commodities

        public void earnIncome() {
                //If they didn't empty their inventory, lower price
                //Model.instance().showNumOfProducers();
                String message;
                if(Model.instance().findProducer(producedCommodity, this) && Model.instance().getTick() > 1){
                        //cph Decrease price by changeProp to represent unsold inventory
                        expPrice[producedCommodity]/=changeProp;
                        if(producedCommodity == GOOD){
                                message = "Due to surplus inventory, price of " + Integer.toString(GOOD) + " falls to " + Double.toString(expPrice[producedCommodity]);
                                debug(message, PRICE);
                        }
                }
                /*if((producedCommodity != 5) && (Model.instance().getNumProducers(producedCommodity)>15)){
                  producedCommodity = 5;
                  }*/
                if(Model.instance().generateSwitch() < Model.SWITCH_PROZ && Model.instance().getTick() > 1){
                        //Remove from current production arrayList
                        int com = 0;
                        double max = 0;
                        for(int i=0; i<Commodity.NUM_COMM; i++){
                                if(expPrice[i] > max){
                                        com = i;
                                        max = expPrice[i];
                                }
                        }
                        producedCommodity = com;
                }
                if(Model.instance().findProducer(producedCommodity, this)){

                }else{
                        //Add to new production arrayList
                        Model.instance().addToProducers(producedCommodity, this);
                }

                if(producedCommodity == 0){
                        //System.out.println("I produce A!");
                }else{
                        //System.out.println("I produce something else!");
                }
                //if(age > 3 && producedCommodity == 1){

                //}else{
                commoditiesHeld[producedCommodity]+=amountProduced;//Units?
                Commodity.getCommNum(producedCommodity).produce(amountProduced);
                totalMoney += money;
                makeBudget();
                //}
        }

        //Form a budget
        public void makeBudget(){
                double totalUtilsDollar = 0;
                for(int i=0; i<Commodity.NUM_COMM; i++){
                        goodUtils[i] = alpha[i] * pow(2.718, (-beta[i] * commoditiesHeld[i]));
                        utilsDollar[i] = goodUtils[i]/expPrice[i];
                        totalUtilsDollar += utilsDollar[i];
                }
                //System.out.printf("I have %f money and I am %d\n", money, myId);
                double shouldBudget = 0;
                for(int i=0; i<Commodity.NUM_COMM; i++){
                        budgetExp[i] = money * (utilsDollar[i]/totalUtilsDollar);
                        shouldBudget += budgetExp[i];
                        //System.out.printf("%f, ", budgetExp[i]);
                }
                //System.out.printf("\n");
                for(int i=0; i<Commodity.NUM_COMM; i++){
                        //System.out.printf("%f, ", commoditiesHeld[i]);
                }
                //System.out.printf("I'm spending %f money\n", shouldBudget);
        }

        //Talk to three producers for each good and select one to buy from
        //Lower the prices of the other two
        public void checkThreeProducers(){ //TODO lots of things REALLY
                //makeBudget();
                String message;
                //Model.instance().showNumOfProducers();
                tradingPartners = new ArrayList<Human>();
                ArrayList<Integer> random = new ArrayList<Integer>();
                for(int l=0; l<Commodity.NUM_COMM; l++){
                        random.add(l);
                }
                //Collections.shuffle(random);
                for(int p=0; p<Commodity.NUM_COMM; p++){
                        int i=random.get(p);
                        if(i==GOOD){
                                buyers++;
                                //System.out.printf("%d agents have tried to buy the good\n", buyers);
                        }
                        int numTrades = 0;
                        //System.out.printf("Human trade budget %f num prod %d numTrades %d\n", budgetExp[i], Model.instance().getNumProducers(i),numTrades);
                        while(Model.instance().getNumProducers(i)>0 && budgetExp[i] > 0.01 && numTrades < 3){
                                //System.out.printf("Human trade budget is %f and num prod is %d\n", budgetExp[i], Model.instance().getNumProducers(i));
                                numTrades++;
                                tradingPartners.clear();
                                //System.out.println("Maybe stuck in a loop\n");
                                //System.out.printf("Budget for good %d is %f\n", i, budgetExp[i]);
                                //int i = p;
                                int numt = 3;
                                int psize = Model.instance().getNumProducers(i);
                                if(i==GOOD){
                                        //System.out.printf("Num producers is %d\n",psize);
                                }
                                if(psize<3){
                                        numt = psize;
                                }
                                int cheapestProducer = 0;
                                double lowestPrice = 0;
                                double avgPrice = 0;
                                double sumPrice = 0;
                                //System.out.printf("psize is %d\n", psize);
                                for(int k=0; k<numt; k++){
                                        Human toAdd = Model.instance().getProducerOfGood(i);
                                        if(tradingPartners.contains(toAdd)){
                                                //k--;
                                        }else{
                                                tradingPartners.add(toAdd);
                                        }
                                }
                                //System.out.printf("tp size is %d\n",tradingPartners.size());
                                lowestPrice = tradingPartners.get(0).expPrice[i];
                                for(int k=0; k<tradingPartners.size(); k++){
                                        sumPrice += tradingPartners.get(k).expPrice[i];
                                        if(tradingPartners.get(k).expPrice[i]<lowestPrice){
                                                cheapestProducer = k;
                                                lowestPrice = tradingPartners.get(k).expPrice[i];
                                        }
                                }

                                avgPrice = sumPrice/tradingPartners.size();
                                double diff = avgPrice - expPrice[i];
                                diff/=buyerChange;
                                //cph change buyer's price to reflect prices they believe sellers are offering
                                expPrice[i]+=diff;

                                for(int k=0; k<tradingPartners.size(); k++){
                                        if(k!=cheapestProducer){
                                                //tradingPartners.get(k).expPrice[i]*=.99;
                                                if(i==2){
                                                        message = "no deal, price falls to " + Double.toString(expPrice[i]);
                                                        debug(message, TRADE);
                                                }
                                        }else{
                                                selectProducer(tradingPartners.get(k),i);
                                        }
                                }
                        }
                        //cph increase buyer's price by changeProp in the event of no producers to reflect scarcity of the good
                        //expPrice[i]*=changeProp;
                        if(i == GOOD){
                                message = "no producers, price of good " + Integer.toString(GOOD) + " rises to " + Double.toString(expPrice[i]);
                                //debug(message, PRICE);
                        }
                }
        }

        //Reduce each of the agent's commodities by the amount dictated
        //by the commodity class in question

        //Remove half of every commodity and send that info to the commodity
        public void consume() {
                for(int i=0; i<Commodity.NUM_COMM; i++) {
                        //double prop = commoditiesHeld[i]/2;
                        double prop = commoditiesHeld[i];
                        //double prop = 10;
                        Commodity.getCommNum(i).consumeProp(prop);
                        commoditiesHeld[i]-=prop;
                        //commoditiesHeld[i]-=10;
                        /*if(commoditiesHeld[i]-Commodity.getCommNum(i).getAmtCons()>=0) {	
                          commoditiesHeld[i]-=Commodity.getCommNum(i).getAmtCons();	
                          Commodity.getCommNum(i).consume();
                          } else {
                          Commodity.getCommNum(i).consFail(commoditiesHeld[i]);
                          commoditiesHeld[i]=0;
                        //considerDeath();
                        }*/
                }
        }

        /*Pobabilistically determine whether an agent will die and inter
          them and distribute their wealth*/
        public void considerDeath() {
                int BD=0;
                int prob=Model.instance().generateLifeProb();
                //Death at end of model
                if(prob == 101) {

                        //Death if black death is active
                } else if(Model.instance().getTick()>80 && Model.instance().getTick()<82 && BD != 0) {
                        if(prob<10) {
                                Model.instance().inter(this);
                                Model.instance().decrementPopulation();
                                if(BEQ==0) {
                                        omniBequeath(this);
                                }
                                if(BEQ==1) {
                                        primoBequeath(this);
                                }
                        } else {
                                Model.instance().schedule.scheduleOnceIn(.7,this);
                        }
                        //Normal death
                } else if(age>=25 && age<100) {
                        int getAbove=6;
                        /*if(Model::instance()->getTick()>80 && Model::instance()->getTick()<85)
                          {
                          getAbove=90;
                          }*/
                        if(prob<getAbove) {
                                Model.instance().inter(this);
                                Model.instance().decrementPopulation();
                                if(BEQ==0) {
                                        omniBequeath(this);
                                }
                                if(BEQ==1) {
                                        primoBequeath(this);
                                }
                        } else {
                                Model.instance().schedule.scheduleOnceIn(.7,this);
                        }
                        //No one lives over 100 years
                } else if(age>=100 ) {
                        Model.instance().inter(this);
                        Model.instance().decrementPopulation();
                        if(BEQ==0) {
                                omniBequeath(this);
                        }
                        if(BEQ==1) {
                                primoBequeath(this);
                        }
                        //Person survived
                } else{
                        Model.instance().schedule.scheduleOnceIn(.7,this);
                }
                age++;
        }

        /*Execute each of the agent's yearly actions in turn*/
        public void step(SimState state) {
                if(Model.instance().generateLifeProb()==101){
                        mode = LifeStage.DYING;
                }
                //earn
                if(mode == LifeStage.EARNING){
                        this.earnIncome();
                        mode = LifeStage.TRADING;
                        Model.instance().schedule.scheduleOnceIn(.1,this);
                        //trade
                }else if(mode == LifeStage.TRADING){
                        //this.tradeWithRandomAgents();
                        this.checkThreeProducers();
                        mode = LifeStage.CONSUMING;
                        Model.instance().schedule.scheduleOnceIn(.1,this);
                        //consume
                }else if(mode == LifeStage.CONSUMING){
                        this.consume();
                        //System.out.printf("food price: %f, id: %d, Make: %d\n",expPrice[1],myId,producedCommodity);
                        //mode = LifeStage.BIRTHING;
                        mode = LifeStage.EARNING;
                        Model.instance().schedule.scheduleOnceIn(.8,this);
                        age++;
                        //child
                }/*else if(mode == LifeStage.BIRTHING){
                   this.considerHavingAChild();
                   mode = LifeStage.DYING;
                   Model.instance().schedule.scheduleOnceIn(.1,this);
                //death
                }*/else if(mode == LifeStage.DYING){
                        this.considerDeath();
                        //mode = LifeStage.EARNING;
                }
        }

        //Constructors & destructors
        //--------------------------------------------------------------------------
        //This constructor is called for initial agents
        public Human() {
                myId = nextAgentNum++; 
                mode = LifeStage.EARNING;
                /*if (myId == 0 || myId == 1) {
                  producedCommodity = myId;
                  } else {*/
                producedCommodity=Model.instance().generateMake(this);
                //}
                residentCommunity=Model.instance().generateCommunity(this);
                age=0;//Model.instance().generateAge();
                money=Model.MONEY;
                children = new ArrayList<Human>();
                minThreshold = new double [Commodity.NUM_COMM];//Between 0 and 5
                commoditiesHeld = new double [Commodity.NUM_COMM];
                expPrice = new double [Commodity.NUM_COMM];
                chokeQuant = new double [Commodity.NUM_COMM];
                demandSlope = new double [Commodity.NUM_COMM];
                budget = new double [Commodity.NUM_COMM];

                //New trade variables
                alpha = new double [Commodity.NUM_COMM];
                beta = new double [Commodity.NUM_COMM];
                goodUtils = new double [Commodity.NUM_COMM];
                utilsDollar = new double [Commodity.NUM_COMM];
                budgetExp = new double [Commodity.NUM_COMM];

                timesTraded=0;
                for(int i=0; i<Commodity.NUM_COMM; i++) {
                        minThreshold[i]=Commodity.getCommNum(i).getAmtCons();//Model.instance().generateNeedCommodityThreshold();	
                        /* while( minThreshold[i]< Commodity.getCommNum(i).getAmtCons()) {
                           minThreshold[i]=Model.instance().generateNeedCommodityThreshold();	
                           }*/
                        Commodity.getCommNum(i).incNeed(minThreshold[i]);
                        expPrice[i]=Model.instance().generateExpPrice();
                        chokeQuant[i]=Model.instance().generateChokeQuant();
                        demandSlope[i]=Model.instance().generateDemandSlope();
                        alpha[i] = Model.instance().generateAlpha();
                        beta[i] = Model.instance().generateBeta();
                        commoditiesHeld[i]=0;
                }
                allNeeds=0;
                for(int i=0; i<Commodity.NUM_COMM; i++) {
                        allNeeds+=minThreshold[i];
                }
                amountProduced=Model.instance().generateAmountProduced();
                totalAmountProduced += amountProduced;
                Commodity.getCommNum(producedCommodity).incMakerNum(amountProduced);
                Model.instance().addToActors(this);
                //Model.instance().addToProducers(producedCommodity, this);
        }

        //Private trade functions
        //--------------------------------------------------------------------------

        private void selectProducer(Human seller, int good){
                double price = seller.expPrice[good];
                double quantity = budgetExp[good]/price;
                double otherQuantity = seller.budgetExp[good]/price;//seller.chokeQuant[good];
                String message;
                /*double quantity = chokeQuant[good];
                  double otherQuantity = seller.chokeQuant[good];
                //How much is the buyer willing to buy at the price
                for(int i=0; i<Commodity.NUM_COMM; i++){
                if(i!=good){
                quantity -= demandSlope[i]*expPrice[i];
                }else{
                quantity -= demandSlope[good]*price;
                }
                }*/
                /*for(int k=0; k<Commodity.NUM_COMM; k++){
                  otherQuantity -= seller.demandSlope[k]*seller.expPrice[k];
                  }*/
                double firstQuantity = quantity;
                if(otherQuantity>(seller.commoditiesHeld[good]-quantity)){
                        quantity=(seller.commoditiesHeld[good]-quantity);
                        //Remove seller from producer arrayList
                        //cph Seller sold inventory, raises price to reflect scarcity of his good
                        seller.expPrice[good]*=changeProp;
                        if(good == GOOD){
                                message = "Producer of good " + Integer.toString(GOOD) + " ran out, price rises to " + Double.toString(seller.expPrice[good]) + " wanted to hold quantity " + Double.toString(quantity) + " not " + Double.toString(firstQuantity);
                                debug(message, PRICE);
                        }
                        Model.instance().removeFromProducers(seller.producedCommodity, seller);
                        debug("removing", false);
                }
                double diff = price - expPrice[good];
                diff/=10;
                double tempUnDiff = expPrice[good];
                //expPrice[good]+=diff;
                if(good==2){
                        message = "2 price changed from " + Double.toString(tempUnDiff) + " to " + Double.toString(expPrice[good]);
                        debug(message, TRADE);
                        message = "2 price changed by " + Double.toString(diff);
                        debug(message, TRADE);
                        //System.out.printf("2 price changed from %f to %f\n", tempUnDiff, expPrice[good]);
                        //System.out.printf("2 price changed by %f\n", diff);
                }
                //If they bought, raise seller's price
                if(quantity > 0){// && money >= price*quantity){
                        //System.out.printf("We bought %f at %f!\n",quantity, price);
                        seller.commoditiesHeld[good]-=quantity;
                        commoditiesHeld[good]+=quantity;

                        budgetExp[good]-= quantity*price;
                        seller.money += quantity*price;
                        money -= quantity*price;

                        Commodity.getCommNum(good).reportSale(quantity);
                        totalSpent+=quantity*price;
                        //seller.expPrice[good]*=1.01;
                }else{
                        //System.out.printf("We didn't buy at %f!\n", price);
                }
                }

                private void incrementTrades() { timesTraded++; }

                //Commodity checking utilties
                //--------------------------------------------------------------------------
                /* Return whether the agent is DEFICIENT or SATISFIED in the
                   given commodity*/
                private CommodityStatus checkStatus(int commodityNum) {
                        if(commoditiesHeld[commodityNum]<minThreshold[commodityNum]) {
                                return CommodityStatus.DEFICIENT;
                        }
                        return CommodityStatus.SATISFIED;
                }

                /* Return the number of the commodity greater than or equal to x that we are
                   deficient in. If we are not deficient in any, return NUM_COMM (which is an
                   illegal commodity number.)*/
                private int findStatusCommodityStartingAt(int x, CommodityStatus specifiedCommStatus) {
                        for(int i=x; i<Commodity.NUM_COMM; i++) {
                                if(checkStatus(i)==specifiedCommStatus) {
                                        return i;
                                }
                        }
                        return Commodity.NUM_COMM;
                }

                /* Return how many commodities the agent is [CommodityStatus] in */
                private int getNumCommoditiesWithStatus(CommodityStatus status) {
                        int i=0;
                        for (int j=0; j<Commodity.NUM_COMM; j++) {
                                if (checkStatus(j) == status) {
                                        i++;
                                }
                        }
                        return i;
                }

                //Inheritance functions
                //--------------------------------------------------------------------------
                /*Give an even portion of the agent's goods to all the children*/
                private void omniBequeath(Human man) {
                        if(man.children.size()==0) {
                                //std::cout<<"Childless ";
                                /*TODO remove holdings from model*/
                        } else {
                                if(man.children.size()>1) {
                                        Model.instance().addToWealthRedistributed(man.getWealth());
                                        Model.instance().incrementOmniEvent();
                                }
                                for(int i=0; i<man.children.size(); i++) {
                                        for(int j=0; j<Commodity.NUM_COMM; j++) {
                                                double progeny=man.children.size();
                                                man.children.get(i).commoditiesHeld[j]+=man.commoditiesHeld[j]/progeny;
                                                man.children.get(i).money+=man.money/progeny;
                                        }
                                }
                        }
                }

                /*Give all of the dead agent's goods to first child agent*/
                private void primoBequeath(Human man) {
                        if(man.children.size()==0) {

                        } else {
                                for(int i=0; i<Commodity.NUM_COMM; i++) {
                                        man.children.get(0).commoditiesHeld[i]+=man.commoditiesHeld[i];
                                        man.children.get(0).money+=man.money;
                                }
                        }
                }

                //Human data retrieval
                //--------------------------------------------------------------------------
                //Return singular, unchanging data about the agent
                public int getCommunity() { return residentCommunity; }
                public int getTimesTraded() { return timesTraded; }
                public int getMake() { return producedCommodity; }
                public double getAmountProduced() { return amountProduced; }
                public int getNumTraders() { return numTraders; }
                public int getAge()  { return age; }
                public int getId()  { return myId; }

                //Return composite data about the agent
                //Return total amount of all commodities
                public double getWealth() {
                        double wealth = 0;
                        for (int i=0; i<Commodity.NUM_COMM; i++) {
                                wealth += commoditiesHeld[i];
                        }
                        return wealth;
                }
                //Return how many goods an agent is satisfied in
                public double getSatisfaction() { return getNumSatisfiedCommodities(); }
                //Return sum of all need levels
                public double getNeeds() { return allNeeds; }
                //Return how much of commodity x an agent has
                public double getAmtOfCommodityX(int x) { return commoditiesHeld[x]; }
                //Return the number of commmodities the agent is below need threshold on
                public int getNumDeficientCommodities()  { return getNumCommoditiesWithStatus(CommodityStatus.DEFICIENT); }
                //Return the number of goods an agent is above need and below want
                public int getNumSatisfiedCommodities()  { return getNumCommoditiesWithStatus(CommodityStatus.SATISFIED); }
                //Return one of their expected prices
                public double getPrice(int comm) { return expPrice[comm]; }

                //Human data
                //--------------------------------------------------------------------------
                //Human data unchanging
                private double amountProduced;//Between 3 and 7
                private int producedCommodity;//Between 0 and 10
                private int numTraders;//Between 5 and 100
                private Human parent;
                private int myId;
                private int residentCommunity;
                private double allNeeds;
                private double [] minThreshold;
                private double [] demandSlope;
                private double [] chokeQuant;
                private double [] budget;

                //Try new demand function
                private double [] alpha; //utility derived from one of the good if I have none, "chokeUtil", if you will
                private double [] beta; //per ear, diminishing returns on good
                private double [] goodUtils;
                private double [] utilsDollar;
                private double [] budgetExp;

                //Human data changing
                private int age;
                private double money;
                private int timesTraded;
                private double [] commoditiesHeld;
                private double [] expPrice;
                private ArrayList <Human> children;
                public LifeStage mode;
                private ArrayList <Human> tradingPartners;

                //Static agent id creator reference
                private static int nextAgentNum = 0;

                public static double totalAmountProduced = 0;

                //Currently unused functions

                //Print the status of each of the agent's commodities
                /*public void print() {
                  double totalNeeds = 0;
                  for (int i=0; i<Commodity.NUM_COMM; i++) {
                  totalNeeds += this.minThreshold[i];
                  }
                //System.out.print(h.myId << " Salary " << h.salary << " Make " << h.producedCommodity << " mps "
                //        << h.mps << " total needs " << totalNeeds << " Trades with " 
                //        << h.numTraders << " Traded " << h.timesTraded << " times " << std::endl << "[";
                System.out.printf("Salary %d Make %d mps %d total needs %d Trades with %d Traded %d times \n["
                ,this.salary,this.producedCommodity,this.mps,totalNeeds,this.numTraders,this.timesTraded);
                for(int i=0; i<Commodity.NUM_COMM; i++)
                {
                CommodityStatus cs = this.checkStatus(i);
                switch (cs) {
                case DEFICIENT:
                System.out.printf("?");
                break;
                case SATISFIED:
                System.out.printf("=");
                break;
                }
                System.out.printf("%d",this.commoditiesHeld[i]);
                if (i<Commodity.NUM_COMM-1) {
                System.out.printf(", ");
                }
                }
                System.out.printf("]\n");
                }*/

                //Initiate random trades with other agents
                public void tradeWithRandomAgents() {
                        /*
                           Ok, new plan. Trading with communities in play.
                           Generate a number every time, if you are below, trade in community.
                           If you're above, you may trade with the world at large.
                         */
                        for(int i=0; i<Model.NUM_TRADERS;i++) {
                                int roll=(Model.instance().generateOutsideTrade());
                                //if(roll>Model.INTROVERT_DIAL) {
                                buyFrom(Model.instance().getRandomGlobalMember());
                                /*} else {
                                  buyFrom(Model.instance().getRandomCommunityMember(residentCommunity));
                                  }*/
                                /*int k=0;
                                  for(int j=0; j<Commodity.NUM_COMM; j++){
                                  if(checkStatus(j)==CommodityStatus.DEFICIENT){
                                  k=1;
                                  }
                                  }
                                  if(k==0){
                                  break;
                                  }*/
                        }
                }

                /*Probabilistically determine whether an agent will have a child
                  and add a new agent to the schedule if so*/
                public void considerHavingAChild() {
                        int prob=Model.instance().generateChild();
                        if(age>=20 && age<35) {
                                if(prob>90) {
                                        Human newchild = new Human(this);
                                        Model.instance().schedule.scheduleOnceIn(.6,newchild);
                                        Model.instance().incrementPopulation();
                                }
                        }
                }

                //Ill advised budgeting done in earnIncome function
                /*double [] tempQuants = new double [Commodity.NUM_COMM];
                  for(int i=0; i<Commodity.NUM_COMM; i++){
                  double quantity = chokeQuant[i];
                  for(int k=0; k<Commodity.NUM_COMM; k++){
                  quantity -= demandSlope[k]*expPrice[k];
                //System.out.printf("%f, ", quantity);
                }
                //System.out.printf(" End good\n");
                if(quantity >= 0){
                tempQuants[i] = quantity;
                }else{
                tempQuants[i] = 0;
                }
                }
                double totalBudget = 0;
                double adjBudget = 0;
                for(int i=0; i<Commodity.NUM_COMM; i++){
                budget[i] = tempQuants[i]*expPrice[i];
                totalBudget += tempQuants[i]*expPrice[i];
                }
                double budgetAdjustor = 0;
                if(totalBudget == 0){

                }else{
                budgetAdjustor = money/totalBudget;
                }
                for(int i=0; i<Commodity.NUM_COMM; i++){
                budget[i] = budget[i]*budgetAdjustor;
                adjBudget += budget[i];
                }
                //System.out.printf("My total budget is %f and I have %f money.\n", totalBudget, money);
                //System.out.printf("My adjusted budget is %f and I have %f money.\n", adjBudget, money);*/

                //This constructor is called for new children
                public Human(Human progenitor) {
                        mode = LifeStage.EARNING;
                        parent=progenitor;
                        myId = nextAgentNum++; 
                        producedCommodity=Model.instance().generateMake(this);
                        residentCommunity=parent.residentCommunity;
                        age=0;
                        money=100;
                        children = new ArrayList<Human>();
                        minThreshold = new double [Commodity.NUM_COMM];//Between 0 and 5
                        commoditiesHeld = new double [Commodity.NUM_COMM];
                        timesTraded=0;
                        for(int i=0; i<Commodity.NUM_COMM; i++) {
                                minThreshold[i]=Model.instance().generateNeedCommodityThreshold();	
                                while(minThreshold[i]<Commodity.getCommNum(i).getAmtCons()) {
                                        minThreshold[i]=Model.instance().generateNeedCommodityThreshold();	
                                }
                                Commodity.getCommNum(i).incNeed(minThreshold[i]);
                                expPrice[i]=Model.instance().generateExpPrice();
                                commoditiesHeld[i]=0;
                        }
                        allNeeds=0;
                        for(int i=0; i<Commodity.NUM_COMM; i++) {
                                allNeeds+=minThreshold[i];
                        }
                        amountProduced=Model.instance().generateAmountProduced();
                        Commodity.getCommNum(producedCommodity).incMakerNum(amountProduced);
                        parent.children.add(this);
                        Model.instance().addToActors(this);
                        Model.instance().addToCommunity(residentCommunity,this);
                }

                private void buyFrom(Human other){
                        //Show that the agents have engaged in another trade
                        incrementTrades();
                        other.incrementTrades();
                        //Check each commodity for trading value
                        for(int i=0; i<Commodity.NUM_COMM; i++){
                                //If the buyer is low in the commodity...
                                if(checkStatus(i)==CommodityStatus.DEFICIENT){
                                        //And the seller is satisfied...
                                        if(other.checkStatus(i)==CommodityStatus.SATISFIED){
                                                //Let the trades begin!
                                                trade(i,other);
                                        }else{
                                                //If the seller is also low, the good is scarce and price expectations rise
                                                expPrice[i]*=1.01;
                                                other.expPrice[i]*=1.01;
                                        }
                                        //If both agents are satisfied, the good is not as scarce and price expectations fall
                                }else if(other.checkStatus(i)==CommodityStatus.SATISFIED){
                                        expPrice[i] *= .99;
                                        other.expPrice[i] *= .99;
                                }
                        }
                }

                private void trade(int x, Human other){
                        //Determine how low and high the agents are in the given good (not Giffen good)
                        double deficit = minThreshold[x]-commoditiesHeld[x];
                        double surplus = other.commoditiesHeld[x]-other.minThreshold[x];
                        //Sell only if the buyer is willing to pay more than the seller wants???
                        //if(expPrice[x]>other.expPrice[x]){//SELLER HAS MONOPOLISTIC CONTROL!!! MAYDAY!
                        //Haggle to the price being the average of the two expecteds
                        double price = (expPrice[x]+other.expPrice[x])/2;
                        double diff = price - expPrice[x];
                        //Bring both agents' expectations a tenth of the way to the average
                        diff/=10;
                        expPrice[x]+=diff;
                        diff = price - other.expPrice[x];
                        diff/=10;
                        other.expPrice[x]+=diff;
                        //Decide the amount to buy so as not to bring buyer above or seller below need level
                        if(deficit>surplus){
                                money -= surplus*price;
                                other.money += surplus*price;
                                other.commoditiesHeld[x]-=surplus;
                                commoditiesHeld[x]+=surplus;
                        }else{
                                money -= deficit*price;
                                other.money += deficit*price;
                                other.commoditiesHeld[x]-=deficit;
                                commoditiesHeld[x]+=deficit;
                        }
                        //Increment model-wide trades
                        Model.instance().incrementTrades();
                        /*}else{
                          other.expPrice[x] = other.expPrice[x]-((other.expPrice[x]-expPrice[x])/10);
                          expPrice[x] = expPrice[x]+((other.expPrice[x]-expPrice[x])/10);
                          }*/
                }
        };
