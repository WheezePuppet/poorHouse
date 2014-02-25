#ifndef COMMODITY_H
#define COMMODITY_H
#include<Random.h>
class Commodity{
	public:
		static const int NUM_COMM=10;
		static Commodity & getCommNum(int num);
        static void initCommodities();
		void consume();
		void consFail(double x);
		void produce(double quantity);
		double getAmtCons() const;
		double getTotalAmt() const;

	private:
		static Commodity *theCommodities[NUM_COMM];
		Commodity();//Constructor is private, see line 9
		double totalAmountInSystem;
		double amtCons;
};
#endif
