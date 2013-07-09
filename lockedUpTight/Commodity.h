#ifndef COMMODITY_H
#define COMMODITY_H
#include<Random.h>
class Commodity{
	public:
		static const int NUM_COMM=10;
		static Commodity & getCommNum(int num);
        static void initCommodities();
		void consume();
		void consFail(int x);
		void produce(float quantity);
		double getAmtCons();
		double getTotalAmt();

	private:
		static Commodity *theCommodities[NUM_COMM];
		Commodity();//Constructor is private, see line 9
		double totalAmountInSystem;
		double amtCons;
};
#endif
