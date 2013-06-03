#include<RepastProcess.h>
#include"Model.h"

int main(int argc, char *argv[])
{
    Model *sim;
    try {
        boost::mpi::environment env(argc,argv);
        repast::RepastProcess::init("");

        //When you're feeling brave, Russell, comment this in.
        //sim = Model::instance();

        repast::RepastProcess::instance()->done();
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

	return 0;
}
