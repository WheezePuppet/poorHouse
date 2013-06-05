#include<RepastProcess.h>
#include"Model.h"

using namespace std;

int main(int argc, char *argv[])
{
    Model *model;
    try {
        boost::mpi::environment env(argc,argv);
        repast::RepastProcess::init("");

        //When you're feeling brave, Russell, comment this in.
cout << "start" << endl;
        model = Model::instance();
        model->startSimulation();
cout << "end" << endl;

        repast::RepastProcess::instance()->done();
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

	return 0;
}
