#include"Commodity.h"
#include"Model.h"
#include<cstdlib>

Commodity Commodity::theCommodities[NUM_COMM];

void Commodity::consume()
{
	totalAmountInSystem-=amtCons;
}

void Commodity::produce(float quantity)
{
	totalAmountInSystem+=quantity;
}

Commodity & Commodity::getCommNum(int num)
{
	return theCommodities[num];
}

Commodity::Commodity()//Determine consumption rate, set 
{
	totalAmountInSystem=0;
	amtCons=Model::instance()->generateConsume();
}

double Commodity::getAmtCons()
{
	return amtCons;
}

