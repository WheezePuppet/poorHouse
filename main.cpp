#include<fstream>
#include<RepastProcess.h>
#include"Model.h"

using namespace std;

int main(int argc, char *argv[])
{
    Model *model;
    try {
        boost::mpi::environment env(argc,argv);
        repast::RepastProcess::init("");

        Commodity::initCommodities();

        //When you're feeling brave, Russell, comment this in.
        model = Model::instance();
        model->startSimulation();


        ofstream commodityStatsFile;
        commodityStatsFile.open("commodityStats.txt");
        model->printCommodityStats(commodityStatsFile);
		model->printCommunityStats(std::cout);
        repast::RepastProcess::instance()->done();
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

	return 0;
}
