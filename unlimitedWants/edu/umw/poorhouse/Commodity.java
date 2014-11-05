//package jsim;
package edu.umw.poorhouse;

public class Commodity{
    //Functions
        public static void initCommodities() {
                for (int i=0; i<NUM_COMM; i++) {
                        theCommodities[i] = new Commodity(i);
                }
                System.out.println("Total consumption: " + 
                    theoreticalTotalOfAllConsumption * 
                        Model.NUM_INITIAL_AGENTS);
        }

        private Commodity(int commodityNum)//Determine consumption rate, set 
        {
                this.commodityNum = commodityNum;
                totalAmountInSystem = 0;
                makerNum = 0;
                producedQuantity = 0;
                amtNeeded = 0;
                amtCons=Model.instance().generateConsume();
                theoreticalTotalOfAllConsumption += amtCons;
        }

    //For actor use
        public void consume() {
                totalAmountInSystem-=amtCons;
                totalCons += amtCons;
                allModelCons += amtCons;
        }

        public void reportSale(double amt){
                amtSold+=amt;
        }

        //Consumer does not have enough to consume, so doesn't quite
        public void consFail(double x) {
                totalAmountInSystem-=x;
                totalCons += x;
                allModelCons += x;
        }

        public void consumeProp(double prop){
                totalAmountInSystem-=prop;
                totalCons += prop;
                allModelCons += prop;
        }

        public void produce(double quantity) {
                totalAmountInSystem+=quantity;
        }

        public void incMakerNum(double salary) {
            makerNum++;
            producedQuantity += salary;
        }

        public void decMakerNum(double salary) {
            makerNum--;
            producedQuantity -= salary;
        }

        public void incNeed (double need) {
            amtNeeded += need;
        }

        public void decNeed (double need) {
            amtNeeded -= need;
        }

        public void resetCons () {
            totalCons = 0;
        }

    //Returning things
        public static Commodity getCommNum(int num) { return theCommodities[num]; }

        public double getTotalAmt() { return totalAmountInSystem; }

        public double getAmtCons() { return amtCons; }

        public int getMakerNum() { return makerNum; }

        public double getProducedQuantity() { return producedQuantity; }

        public double getAmtNeeded() { return amtNeeded; }

        public double getTotalCons() { return totalCons; }

        public double getAmtSold() { return amtSold; }

        static public double getAllModelCons() { return allModelCons; }

    //Data
        public static final int NUM_COMM=10;
        private static Commodity [] theCommodities = new Commodity [NUM_COMM];
        private double totalAmountInSystem;
        private double amtCons;
        private int makerNum;
        private int commodityNum;
        private double producedQuantity;
        private double amtNeeded;
        private double totalCons;
        private double amtSold;
        static private double allModelCons=0;
        public static double theoreticalTotalOfAllConsumption;
}
