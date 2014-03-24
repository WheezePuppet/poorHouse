#include<fstream>
#include<RepastProcess.h>
#include"Model.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " dial." << endl;
        exit(1);
    }

    Model::INTROVERT_DIAL = atoi(argv[1]);
	Model::SEED = atoi(argv[2]);
	//Model::LEMMINGNESS = atoi(argv[3]);

    Model *model;
    
    try {
        boost::mpi::environment env(argc,argv);
        repast::RepastProcess::init("");

        Commodity::initCommodities();

        //When you're feeling brave, Russell, comment this in.
        model = Model::instance();
        model->startSimulation();


        ofstream commodityStatsFile;
//		ofstream communityStatsFile;
//		communityStatsFile.open("communityStats.txt");
        commodityStatsFile.open("commodityStats.txt");
        model->printCommodityStats(std::cout);//commodityStatsFile);
	//	model->printCommunityStats(std::cout);//communityStatsFile);
        repast::RepastProcess::instance()->done();
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

	return 0;
}
