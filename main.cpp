#include<fstream>
#include<RepastProcess.h>
#include"Model.h"

using namespace std;

int main(int argc, char *argv[])
{
	std::cout<<"Got anywhere\n";
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " dial." << endl;
        exit(1);
    }

    Model::INTROVERT_DIAL = atoi(argv[1]);
	Model::SEED = atoi(argv[2]);
	std::cout<<"Set the statics\n";
	//Model::LEMMINGNESS = atoi(argv[3]);

    Model *model;
	std::cout<<"Problems with model?\n";
    
    try {
        boost::mpi::environment env(argc,argv);
        repast::RepastProcess::init("");
		std::cout<<"Init comm\n";

        Commodity::initCommodities();
		std::cout<<"init comm\n";

        //When you're feeling brave, Russell, comment this in.
		std::cout<<"Got to the model\n";
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
