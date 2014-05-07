//package jsim;

import java.util.*;
import sim.util.*;
import sim.engine.*;

public class Human implements Steppable {
        /*What is the relation of the agent's held commodities to their
          need level*/
        public enum CommodityStatus { DEFICIENT, SATISFIED };
        //What stage of the year an agent is in
        public enum LifeStage { EARNING, TRADING, CONSUMING, BIRTHING, DYING }; 

        public String commodityStatusString(CommodityStatus cs) {
                switch (cs) {
                        case DEFICIENT:
                                return "DEFICIENT";
                        case SATISFIED:
                                return "SATISFIED";
                }
                return "SOMETHING IS DREADFULLY WRONG!!!!!";
        }

        //Print the status of each of the agent's commodities
        public void print() {
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
        }

        //Global static values
        public static int BEQ;

        //Scheduled events
        //--------------------------------------------------------------------------
        //Increase the agent's salary commodity by his salary and inform commodities
        public void earnIncome() {
                //if(age > 3 && producedCommodity == 1){

                //}else{
                    commoditiesHeld[producedCommodity]+=salary;
                    Commodity.getCommNum(producedCommodity).produce(salary);
                //}
        }

        //Initiate random trades with other agents
        public void tradeWithRandomAgents() {
                /*
                   Ok, new plan. Trading with communities in play.
                   Generate a number every time, if you are below, trade in community.
                   If you're above, you may trade with the world at large.
                 */
                for(int i=0; i<50/*numTraders*/; i++) {
                        int roll=(Model.instance().generateOutsideTrade());
                        //if(roll>Model.INTROVERT_DIAL) {
                                buyFrom(Model.instance().getRandomGlobalMember());
                        /*} else {
                                buyFrom(Model.instance().getRandomCommunityMember(residentCommunity));
                        }*/
                }
        }

        //Reduce each of the agent's commodities by the amount dictated
        //by the commodity class in question
        public void consume() {
                int j = 0;
                for(int i=0; i<Commodity.NUM_COMM; i++) {
                        if(age == 5 && i == 1 && j ==0){
                            //System.out.println("EEK!");
                            //i--;
                            j++;
                        }
                        if(commoditiesHeld[i]-Commodity.getCommNum(i).getAmtCons()>=0) {	
                                commoditiesHeld[i]-=Commodity.getCommNum(i).getAmtCons();	
                                Commodity.getCommNum(i).consume();
                        } else {
                                Commodity.getCommNum(i).consFail(commoditiesHeld[i]);
                                commoditiesHeld[i]=0;
                                //considerDeath();
                        }
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
                        if(age == 5 && producedCommodity == 1){
                            this.earnIncome();
                        }
                        this.earnIncome();
                        mode = LifeStage.TRADING;
                        Model.instance().schedule.scheduleOnceIn(.1,this);
                        //trade
                }else if(mode == LifeStage.TRADING){
                        this.tradeWithRandomAgents();
                        mode = LifeStage.CONSUMING;
                        Model.instance().schedule.scheduleOnceIn(.1,this);
                        //consume
                }else if(mode == LifeStage.CONSUMING){
                        this.consume();
                        //System.out.printf("food price: %f, id: %d, Make: %d\n",expPrice[1],myId,producedCommodity);
                        //mode = LifeStage.BIRTHING;
                        mode = LifeStage.EARNING;
                        Model.instance().schedule.scheduleOnceIn(.9,this);
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
                producedCommodity=Model.instance().generateMake();
                mps=Model.instance().generateMps();
                numTraders=Model.instance().generateNumTraders();
                residentCommunity=Model.instance().generateCommunity(this);
                age=0;//Model.instance().generateAge();
                money=100;
                children = new ArrayList<Human>();
                minThreshold = new int [Commodity.NUM_COMM];//Between 0 and 5
                commoditiesHeld = new int [Commodity.NUM_COMM];
                expPrice = new double [Commodity.NUM_COMM];
                timesTraded=0;
                for(int i=0; i<Commodity.NUM_COMM; i++) {
                        minThreshold[i]=Model.instance().generateNeedCommodityThreshold();	
                        while( minThreshold[i]< Commodity.getCommNum(i).getAmtCons()) {
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
                salary=Model.instance().generateSalary();
                Commodity.getCommNum(producedCommodity).incMakerNum(salary);
                Model.instance().addToActors(this);
        }

        //This constructor is called for new children
        public Human(Human progenitor) {
                mode = LifeStage.EARNING;
                parent=progenitor;
                myId = nextAgentNum++; 
                producedCommodity=Model.instance().generateMake();
                mps=Model.instance().generateMps();
                numTraders=Model.instance().generateNumTraders();
                residentCommunity=parent.residentCommunity;
                age=0;
                money=100;
                children = new ArrayList<Human>();
                minThreshold = new int [Commodity.NUM_COMM];//Between 0 and 5
                commoditiesHeld = new int [Commodity.NUM_COMM];
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
                salary=Model.instance().generateSalary();
                Commodity.getCommNum(producedCommodity).incMakerNum(salary);
                parent.children.add(this);
                Model.instance().addToActors(this);
                Model.instance().addToCommunity(residentCommunity,this);
        }

        //Private trade functions
        //--------------------------------------------------------------------------

        private void buyFrom(Human other){
                incrementTrades();
                other.incrementTrades();
                for(int i=0; i<Commodity.NUM_COMM; i++){
                    if(checkStatus(i)==CommodityStatus.DEFICIENT){
                        if(other.checkStatus(i)==CommodityStatus.SATISFIED){
                            trade(i,other);
                        }else{
                            expPrice[i]*=1.01;
                            other.expPrice[i]*=1.01;
                        }
                    }else if(other.checkStatus(i)==CommodityStatus.SATISFIED){
                        
                    }
                }
        }

        private void trade(int x, Human other){
            int deficit = minThreshold[x]-commoditiesHeld[x];
            int surplus = other.commoditiesHeld[x]-other.minThreshold[x];
            if(expPrice[x]>other.expPrice[x]){
                double price = (expPrice[x]+other.expPrice[x])/2;
                expPrice[x] = price*.99;
                other.expPrice[x] = price*.99;
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
                Model.instance().incrementTrades();
            }else{
                other.expPrice[x] = other.expPrice[x]-((other.expPrice[x]-expPrice[x])/4);
                expPrice[x] = expPrice[x]+((other.expPrice[x]-expPrice[x])/4);
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
        public double getSalary() { return salary; }
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
        private double mps;//Float less than one (which seems never to be used)
        private double salary;//Between 3 and 7
        private int producedCommodity;//Between 0 and 10
        private int numTraders;//Between 5 and 100
        private Human parent;
        private int myId;
        private int residentCommunity;
        private int allNeeds;
        private int [] minThreshold;

        //Human data changing
        private int age;
        private double money;
        private int timesTraded;
        private int [] commoditiesHeld;
        private double [] expPrice;
        private ArrayList <Human> children;
        public LifeStage mode;

        //Static agent id creator reference
        private static int nextAgentNum = 0;
};
