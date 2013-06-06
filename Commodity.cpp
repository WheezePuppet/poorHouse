#include"Commodity.h"
#include"Model.h"
#include<cstdlib>

Commodity * Commodity::theCommodities[NUM_COMM];

void Commodity::initCommodities() {
    for (int i=0; i<NUM_COMM; i++) {
        theCommodities[i] = new Commodity;
    }
}

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
	return *theCommodities[num];
}

Commodity::Commodity()//Determine consumption rate, set 
{
std::cout << "entering constructor" << std::endl;
	totalAmountInSystem=0;
	amtCons=Model::instance()->generateConsume();
std::cout << "leaving constructor" << std::endl;
}

double Commodity::getAmtCons()
{
	return amtCons;
}

