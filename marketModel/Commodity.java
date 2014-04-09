//package jsim;

public class Commodity{
    //Functions
        public static void initCommodities() {
                for (int i=0; i<NUM_COMM; i++) {
                        theCommodities[i] = new Commodity();
                }
        }

        public void consume() {
                totalAmountInSystem-=amtCons;
        }

        //Consumer does not have enough to consume, so doesn't quite
        public void consFail(double x) {
                totalAmountInSystem-=x;
        }

        public void produce(double quantity) {
                totalAmountInSystem+=quantity;
        }

        public static Commodity getCommNum(int num) {
                return theCommodities[num];
        }

        public double getTotalAmt() {
                return totalAmountInSystem;
        }

        private Commodity()//Determine consumption rate, set 
        {
                totalAmountInSystem=0;
                amtCons=Model.instance().generateConsume();
        }

        public double getAmtCons() {
                return amtCons;
        }

    //Data
        public static final int NUM_COMM=10;
        private static Commodity [] theCommodities = new Commodity [NUM_COMM];
        private double totalAmountInSystem;
        private double amtCons;
}
