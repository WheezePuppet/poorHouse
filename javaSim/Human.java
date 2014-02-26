import java.util.*;
import sim.util.*;
import sim.engine.*;

public class Human implements Steppable {

        //int Human::nextAgentNum = 0;
        //private static int nextAgentNum = 0;

        public enum CommodityStatus { DEFICIENT, SATISFIED, BLOATED };
        public enum LifeStage { EARNING, TRADING, CONSUMING, BIRTHING, DYING }; 

        public String commodityStatusString(CommodityStatus cs) {
                switch (cs) {
                        case DEFICIENT:
                                return "DEFICIENT";
                        case BLOATED:
                                return "BLOATED";
                        case SATISFIED:
                                return "SATISFIED";
                }
                return "SOMETHING IS DREADFULLY WRONG!!!!!";
        }

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
                                case BLOATED:
                                        System.out.printf("*");
                                        break;
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

        //public virtual repast::AgentId & getId() { return myId; }

        //Scheduled events
        public void earnIncome() {
                //savings+=(salary+(savings*rr))*mps;
                commoditiesHeld[producedCommodity]+=salary;
                Commodity.getCommNum(producedCommodity).produce(salary);
        }

        public void tradeWithRandomAgents() {
                /*
                   Ok, new plan. Trading with communities in play.
                   Generate a number every time, if you are below, trade in community.
                   If you're above, you may trade with the world at large.
                 */
                for(int i=0; i<numTraders; i++) {
                        int roll=(Model.instance().generateOutsideTrade());
                        if(roll>Model.INTROVERT_DIAL) {
                                transactWith(Model.instance().getRandomGlobalMember());
                        } else {
                                transactWith(Model.instance().getRandomCommunityMember(residentCommunity));
                        }
                }
        }

        public void consume() {
                //cout << "consuming!!" << endl;
                //cons.push_back(salary+(savings*rr))*(1-mps);
                //cout<<*this<<endl;
                //ofstream statsBeforeConsume;
                //if(Model::instance()->getTick()<Model::NUM_YEARS-1)
                for(int i=0; i<Commodity.NUM_COMM; i++) {
                        if(commoditiesHeld[i]-Commodity.getCommNum(i).getAmtCons()>=0) {	
                                commoditiesHeld[i]-=Commodity.getCommNum(i).getAmtCons();	
                                Commodity.getCommNum(i).consume();
                        } else {
                                Commodity.getCommNum(i).consFail(Commodity.getCommNum(i).getAmtCons()-commoditiesHeld[i]);
                                commoditiesHeld[i]=0;
                        }
                        //cout<<this;
                        //Commodity::getCommNum(i).consume();
                }
        }

        public void considerHavingAChild() {
                int prob=Model.instance().generateChild();
                if(age>=20 && age<35) {
                                //System.out.println("child possible!");
                        if(prob>90) {
                                //std::cout<<"Child!\n";
                                //System.out.println("child made!");
                                Human newchild = new Human(this);
                                Model.instance().schedule.scheduleOnceIn(.6,newchild);
                                Model.instance().incrementPopulation();
                        }
                }
        }

        public void considerDeath() {
                int BD=1;
                int prob=Model.instance().generateLifeProb();
                if(prob == 101) {

                } else if(Model.instance().getTick()>80 && Model.instance().getTick()<82 && BD != 0) {
                        if(prob<10) {
                                //std::cout<<"I'm dead!\n";
                                Model.instance().inter(this);
                                Model.instance().decrementPopulation();
                                if(BEQ==0) {
                                        omniBequeath(this);
                                }
                                if(BEQ==1) {
                                        primoBequeath(this);
                                }
                                //std::cout<<"death\n";
                        } else {
                                Model.instance().schedule.scheduleOnceIn(.7,this);
                        }
                } else if(age>=25 && age<100) {
                        //std::cout<<"Dying?\n";
                        int getAbove=6;
                        /*if(Model::instance()->getTick()>80 && Model::instance()->getTick()<85)
                          {
                          getAbove=90;
                          }*/
                        if(prob<getAbove) {
                                //std::cout<<"I'm dead!\n";
                                Model.instance().inter(this);
                                Model.instance().decrementPopulation();
                                if(BEQ==0) {
                                        omniBequeath(this);
                                }
                                if(BEQ==1) {
                                        primoBequeath(this);
                                }
                                //std::cout<<"death\n";
                        } else {
                                Model.instance().schedule.scheduleOnceIn(.7,this);
                        }
                } else if(age>=100 ) {
                        //std::cout<<"I'm dead!\n";
                        Model.instance().inter(this);
                        Model.instance().decrementPopulation();
                        if(BEQ==0) {
                                omniBequeath(this);
                        }
                        if(BEQ==1) {
                                primoBequeath(this);
                        }
                } else{
                        Model.instance().schedule.scheduleOnceIn(.7,this);
                }
                age++;
        }

        public void step(SimState state) {
            //earn
            if(mode == LifeStage.EARNING){
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
                mode = LifeStage.BIRTHING;
                Model.instance().schedule.scheduleOnceIn(.1,this);
            //child
            }else if(mode == LifeStage.BIRTHING){
                this.considerHavingAChild();
                mode = LifeStage.DYING;
                Model.instance().schedule.scheduleOnceIn(.1,this);
            //death
            }else if(mode == LifeStage.DYING){
                this.considerDeath();
                mode = LifeStage.EARNING;
            }
            //age++;
        }
/*
        public void reschedule() {
                repast::ScheduleRunner &theSchedule =
                        repast::RepastProcess::instance()->getScheduleRunner();
                double time = floor(theSchedule.currentTick());
                theSchedule.scheduleEvent(time+1.1, repast::Schedule::FunctorPtr(
                                        new repast::MethodFunctor<Human>(this, &Human::earnIncome)));
                theSchedule.scheduleEvent(time+1.2, repast::Schedule::FunctorPtr(
                                        new repast::MethodFunctor<Human>(this, &Human::tradeWithRandomAgents)));
                theSchedule.scheduleEvent(time+1.3, repast::Schedule::FunctorPtr(
                                        new repast::MethodFunctor<Human>(this, &Human::consume)));
                theSchedule.scheduleEvent(time+1.31, repast::Schedule::FunctorPtr(
                                        new repast::MethodFunctor<Human>(this, &Human::considerHavingAChild)));
                theSchedule.scheduleEvent(time+1.32, repast::Schedule::FunctorPtr(
                                        new repast::MethodFunctor<Human>(this, &Human::considerDeath)));
                age++;
        }*/

        //Constructors & destructors
        //This constructor is called for initial agents
        public Human() {
                //System.out.println("Creating a new human!");
                //myId = repast::AgentId(nextAgentNum++,0,0); 
                myId = nextAgentNum++; 
                mode = LifeStage.EARNING;
                producedCommodity=Model.instance().generateMake();
                mps=Model.instance().generateMps();
                numTraders=Model.instance().generateNumTraders();
                residentCommunity=Model.instance().generateCommunity(this);
                age=Model.instance().generateAge();
                children = new ArrayList<Human>();
                minThreshold = new double [Commodity.NUM_COMM];//Between 0 and 5
                maxThreshold = new double [Commodity.NUM_COMM];//Between 6 and 10
                commoditiesHeld = new double [Commodity.NUM_COMM];
                timesTraded=0;
                for(int i=0; i<Commodity.NUM_COMM; i++) {
                        minThreshold[i]=Model.instance().generateNeedCommodityThreshold();	
                        /*if(minThreshold == null){
                            System.out.println("minThresh null");
                        }
                        System.out.println("minThres i is " + minThreshold[i]);
                        while( minThreshold[i]< Commodity.getCommNum(i).getAmtCons()) {
                                minThreshold[i]=Model.instance().generateNeedCommodityThreshold();	
                        }*/
                        maxThreshold[i]=Model.instance().generateWantCommodityThreshold();
                        commoditiesHeld[i]=0;
                }
                allNeeds=0;
                for(int i=0; i<Commodity.NUM_COMM; i++) {
                        allNeeds+=minThreshold[i];
                }
                salary=Model.instance().generateSalary();
                Model.instance().addToActors(this);
        }

        //This constructor is called for new children
        public Human(Human progenitor) {
                mode = LifeStage.EARNING;
                parent=progenitor;
                //myId = repast::AgentId(nextAgentNum++,0,0); 
                myId = nextAgentNum++; 
                producedCommodity=Model.instance().generateMake();
                /*while((((salary-progenitor.salary)^2)/progenitor.salary)>Model::LEMMINGNESS)
                  {
                  salary=Model::instance()->generateSalary();
                  }*/
                mps=Model.instance().generateMps();
                numTraders=Model.instance().generateNumTraders();
                residentCommunity=parent.residentCommunity;
                age=0;
                children = new ArrayList<Human>();
                minThreshold = new double [Commodity.NUM_COMM];//Between 0 and 5
                maxThreshold = new double [Commodity.NUM_COMM];//Between 6 and 10
                commoditiesHeld = new double [Commodity.NUM_COMM];
                timesTraded=0;
                for(int i=0; i<Commodity.NUM_COMM; i++) {
                        minThreshold[i]=Model.instance().generateNeedCommodityThreshold();	
                        while(minThreshold[i]<Commodity.getCommNum(i).getAmtCons()) {
                                minThreshold[i]=Model.instance().generateNeedCommodityThreshold();	
                        }
                        maxThreshold[i]=Model.instance().generateWantCommodityThreshold();
                        commoditiesHeld[i]=0;
                }
                allNeeds=0;
                for(int i=0; i<Commodity.NUM_COMM; i++) {
                        allNeeds+=minThreshold[i];
                }
                salary=Model.instance().generateSalary();
                parent.children.add(this);
                Model.instance().addToActors(this);
                Model.instance().addToCommunity(residentCommunity,this);
                //reschedule();
        }

        //Return singular, unchanging data about the agent
        public int getCommunity() { return residentCommunity; }
        public int getTimesTraded() { return timesTraded; }
        public int getMake() { return producedCommodity; }
        public double getSalary() { return salary; }
        public int getNumTraders() { return numTraders; }
        public int getAge()  { return age; }
        public int getId()  { return myId; }

        //Return composite data about the agent
        public double getWealth() {
                double wealth = 0;
                for (int i=0; i<Commodity.NUM_COMM; i++) {
                        wealth += commoditiesHeld[i];
                }
                return wealth;
        }

        public double getSatisfaction() { return getNumSatisfiedCommodities() + getNumBloatedCommodities(); }
        public double getNeeds() { return allNeeds; }
        public double getAmtOfCommodityX(int x) { return commoditiesHeld[x]; }
        public int getNumDeficientCommodities()  { return getNumCommoditiesWithStatus(CommodityStatus.DEFICIENT); }
        public int getNumSatisfiedCommodities()  { return getNumCommoditiesWithStatus(CommodityStatus.SATISFIED); }
        public int getNumBloatedCommodities()  { return getNumCommoditiesWithStatus(CommodityStatus.BLOATED); }

        //Human data unchanging
        private double mps;//Float less than one
        private double salary;//Between 3 and 7
        private int producedCommodity;//Between 0 and 10
        private int numTraders;//Between 5 and 100
        private Human parent;
        //private repast::AgentId myId;
        private int myId;
        private int residentCommunity;
        private double allNeeds;
        private double [] minThreshold;// = new double [Commodity.NUM_COMM];//Between 0 and 5
        private double [] maxThreshold;// = new double [Commodity.NUM_COMM];//Between 6 and 10

        //Human data changing
        private int age;
        private int timesTraded;
        private double [] commoditiesHeld;// = new double [Commodity.NUM_COMM];
        private ArrayList <Human> children;
        public LifeStage mode;

        //Ummm...
        private static int nextAgentNum = 0;

        //Private trade functions
        private void makeTradesSuchThat(Human other, 
                        CommodityStatus aTooLowStatus_C1,//The level A wants to get above in good 1
                        CommodityStatus aWantToPreserveStatus_C2,//The level A doesn't want to fall below in good 2
                        CommodityStatus otherWantToPreserveStatus_C1,//The level B doesn't want to fall below in good 1
                        CommodityStatus otherTooLowStatus_C2)//The level B wants to get above in good 2
        {
                int numTrades = 0;

                // Go through all my commodities, searching for ones I'm below my lower bound in.
                for(int i=0; i<Commodity.NUM_COMM; i++) {
                        i= findStatusCommodityStartingAt(i, aTooLowStatus_C1);
                        if(i!=Commodity.NUM_COMM &&
                                        other.checkStatus(i)==otherWantToPreserveStatus_C1) {
                                // Okay, this is now true: I want more of commodity i, and the
                                // RHS (possibly) has some to spare. (We say "possibly" because
                                // they might be exactly at the threshold of their current
                                // status, and so actually wouldn't want to trade any of it.)
                                // So we've "halfway" found a possible super-satisfiable trade.

                                for(int j=0; j<Commodity.NUM_COMM; j++) {
                                        j=other.findStatusCommodityStartingAt(j, otherTooLowStatus_C2);
                                        if(j==Commodity.NUM_COMM) {
                                                // Well, bummer. The RHS doesn't want anything,
                                                // so there are no trades of this kind possible.
                                                return;
                                        }
                                        if(checkStatus(j) == aWantToPreserveStatus_C2) {
                                                // Do actual trade! This is because we now know:
                                                //   - A wants more i
                                                //   - B has an excess of i
                                                //   - B wants more j
                                                //   - A has an excess of j 
                                                double aWantToBuy_Ci=0.0,
                                                       aWillingToSell_Cj=0.0,
                                                       otherWantToBuy_Cj=0.0,
                                                       otherWillingToSell_Ci=0.0;
                                                switch(aTooLowStatus_C1) {
                                                        case DEFICIENT:
                                                                aWantToBuy_Ci=minThreshold[i]-commoditiesHeld[i];
                                                                break;
                                                        case SATISFIED:
                                                                aWantToBuy_Ci=maxThreshold[i]-commoditiesHeld[i];
                                                                break;
                                                }
                                                switch(otherTooLowStatus_C2) {
                                                        case DEFICIENT:
                                                                otherWantToBuy_Cj=
                                                                        other.minThreshold[j]-other.commoditiesHeld[j];
                                                                break;
                                                        case SATISFIED:
                                                                otherWantToBuy_Cj=
                                                                        other.maxThreshold[j]-other.commoditiesHeld[j];
                                                                break;
                                                }
                                                switch(aWantToPreserveStatus_C2) {
                                                        case BLOATED:
                                                                aWillingToSell_Cj=commoditiesHeld[j]-maxThreshold[j];
                                                                break;
                                                        case SATISFIED:
                                                                aWillingToSell_Cj=commoditiesHeld[j]-minThreshold[j];
                                                                break;
                                                }
                                                switch(otherWantToPreserveStatus_C1) {
                                                        case BLOATED:
                                                                otherWillingToSell_Ci=
                                                                        other.commoditiesHeld[i]-other.maxThreshold[i];
                                                                break;
                                                        case SATISFIED:
                                                                otherWillingToSell_Ci=
                                                                        other.commoditiesHeld[i]-other.minThreshold[i];
                                                                break;
                                                }

                                                trade(i,
                                                                j,
                                                                aWantToBuy_Ci,
                                                                otherWillingToSell_Ci,
                                                                otherWantToBuy_Cj,
                                                                aWillingToSell_Cj,
                                                                other);
                                                numTrades++;

                                                // We have now traded. However, I may still be too low
                                                // in commodity i. If so, I need to continue to look 
                                                // for other j's to continue to satisfy my i.
                                        }
                                }
                                // At this point, one of two things is true:
                                // (a) I am no longer lacking in commodity i. Yay!  ...or...
                                // (b) I am still lacking in commodity i, but I've exhausted
                                // all possibilities of trading with RHS for it.
                                // So now, continue looking for other i's I may be lacking in.
                        }
                }
                //cout << "Made " << numTrades << " trades." << endl;
        }

        private void transactWith(Human other) {
                incrementTrades();
                other.incrementTrades();
                //cout << "transacting from " << getId() << " to " << other.getId() << endl;

                /*
                   D,B,B,D  - SUPER

                   D,B,B,S  - HALF-SUPER
                   D,B,S,D  - HALF-SUPER
                   S,B,B,D  - HALF-SUPER
                   D,S,B,D  - HALF-SUPER

                   D,S,S,D  - ORDINARY
                   S,B,B,S  - ORDINARY
                   S,B,D,S  - ORDINARY
                   S,D,B,S  - ORDINARY

                 */

                // 1. Make super-satisfiable trades where possible.
                makeTradesSuchThat(other, CommodityStatus.DEFICIENT, CommodityStatus.BLOATED, CommodityStatus.BLOATED, CommodityStatus.DEFICIENT);

                // 2. Make half-super-satisfiable trades where possible.

                makeTradesSuchThat(other, CommodityStatus.DEFICIENT, CommodityStatus.BLOATED, CommodityStatus.BLOATED, CommodityStatus.SATISFIED);
                makeTradesSuchThat(other, CommodityStatus.DEFICIENT, CommodityStatus.BLOATED, CommodityStatus.SATISFIED, CommodityStatus.DEFICIENT);
                makeTradesSuchThat(other, CommodityStatus.SATISFIED, CommodityStatus.BLOATED, CommodityStatus.BLOATED, CommodityStatus.DEFICIENT);
                makeTradesSuchThat(other, CommodityStatus.DEFICIENT, CommodityStatus.SATISFIED, CommodityStatus.BLOATED, CommodityStatus.DEFICIENT);

                // 3. make ordinary-satisfiable trades happen.

                makeTradesSuchThat(other, CommodityStatus.DEFICIENT, CommodityStatus.SATISFIED, CommodityStatus.SATISFIED, CommodityStatus.DEFICIENT);
                makeTradesSuchThat(other, CommodityStatus.SATISFIED, CommodityStatus.BLOATED, CommodityStatus.BLOATED, CommodityStatus.SATISFIED);
                makeTradesSuchThat(other, CommodityStatus.SATISFIED, CommodityStatus.BLOATED, CommodityStatus.DEFICIENT, CommodityStatus.SATISFIED);
                makeTradesSuchThat(other, CommodityStatus.SATISFIED, CommodityStatus.DEFICIENT, CommodityStatus.BLOATED, CommodityStatus.SATISFIED);
        }

        private void swap(double x, Human other, int alow, int blow) {
                commoditiesHeld[alow]+=x;
                other.commoditiesHeld[alow]-=x;
                commoditiesHeld[blow]-=x;
                other.commoditiesHeld[blow]+=x;
        }

        private void trade(int comm1Num, int comm2Num,
                        double amtAWillingToBuyOf1, double amtBWillingToBuyOf2,
                        double amtAWillingToSellOf2, double amtBWillingToSellOf1,
                        Human B) {

                //cout << "ACTUALLY trading between " << getId() << " and " << B.getId() << "!!" << endl;
                double [] coms = new double [4];
                coms[0] = amtAWillingToBuyOf1;
                coms[1] = amtBWillingToSellOf1;
                coms[2] = amtBWillingToBuyOf2;
                coms[3] = amtAWillingToSellOf2;

                boolean good=true;
                for(int i=0; i<4; i++) {
                        if(coms[i]<0) {
                                coms[i]=10000;//Make all negative lows really high to prevent their being chosen to be traded
                        } else if(coms[i]==0) {
                                good=false;//If at least one is not negative or zero, make it possible to trade 
                        }
                }

                if(good) {
                        double x = min(coms);//Choose the lowest of the options for trades so that you don't overstep any of the bounds
                        double change=x;
                        //std::cout<<*this;
                        //std::cout<<B;
                        //std::cout<<"Exchanging "<<change<<" units of commodities " <<comm1Num<< " and " <<comm2Num <<std::endl;
                        Model.instance().incrementTrades();
                        Model.instance().addToTradedAmount(change);
                        swap(change, B, comm1Num, comm2Num);
                        //std::cout<<*this;
                }
        }

        private double min(double[] numbers){  
          double minValue = numbers[0];  
          for(int i=1;i<numbers.length;i++){  
            if(numbers[i] < minValue){  
              minValue = numbers[i];  
            }  
          }  
          return minValue;  
        }  

        private void incrementTrades() { timesTraded++; }

        //Commodity checking utilties
        private CommodityStatus checkStatus(int commodityNum) {
                if(commoditiesHeld[commodityNum]<minThreshold[commodityNum]) {
                        return CommodityStatus.DEFICIENT;
                }
                if(commoditiesHeld[commodityNum]>maxThreshold[commodityNum]) {
                        return CommodityStatus.BLOATED;
                }
                return CommodityStatus.SATISFIED;
        }

        // Return the number of the commodity greater than or equal to x that we are
        // deficient in. If we are not deficient in any, return NUM_COMM (which is an
        // illegal commodity number.)
        private int findStatusCommodityStartingAt(int x, CommodityStatus specifiedCommStatus) {
                for(int i=x; i<Commodity.NUM_COMM; i++) {
                        if(checkStatus(i)==specifiedCommStatus) {
                                return i;
                        }
                }
                return Commodity.NUM_COMM;
        }

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
        private void omniBequeath(Human man) {
                if(man.children.size()==0) {
                        //std::cout<<"Childless ";
                } else {
                        if(man.children.size()>1) {
                                Model.instance().addToWealthRedistributed(man.getWealth());
                                Model.instance().incrementOmniEvent();
                        }
                        for(int i=0; i<man.children.size(); i++) {
                                for(int j=0; j<Commodity.NUM_COMM; j++) {
                                        double progeny=man.children.size();
                                        man.children.get(i).commoditiesHeld[j]+=man.commoditiesHeld[j]/progeny;
                                }
                        }
                }
        }

        private void primoBequeath(Human man) {
                if(man.children.size()==0) {

                } else {
                        for(int i=0; i<Commodity.NUM_COMM; i++) {
                                man.children.get(0).commoditiesHeld[i]+=man.commoditiesHeld[i];
                        }
                }
        }
};
