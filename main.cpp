#include<fstream>
#include<RepastProcess.h>
#include"Model.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " numYrs." << endl;
        exit(1);
    }

    Model::NUM_YEARS = atoi(argv[1]);

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
