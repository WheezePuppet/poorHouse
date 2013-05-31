#ifndef MODEL_H
#define MODEL_H
#include<SharedContext.h>
#include"Human.h"
#include<Random.h>
//#include"Commodity.h"
using namespace repast;

class Model
{
private:
public:
	SharedContext<Human> actors;

	Model();
};
#endif
