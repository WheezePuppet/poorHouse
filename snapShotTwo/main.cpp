#include<fstream>
#include<RepastProcess.h>
#include"Model.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 4) {
        cout << "Usage: " << argv[0] << " dial." << endl;
        exit(1);
    }

    Model::INTROVERT_DIAL = atoi(argv[1]);
	Model::SEED = atoi(argv[2]);
	Human::BEQ = atoi(argv[3]);
	//Model::LEMMINGNESS = atoi(argv[3]);
    Model *model;
    try {
        boost::mpi::environment env(argc,argv);
        repast::RepastProcess::init("");

        Commodity::initCommodities();

        //When you're feeling brave, Russell, comment this in.
        model = Model::instance();
        model->startSimulation();

        model->printCommodityStats(std::cout);
        repast::RepastProcess::instance()->done();
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

	return 0;
}
