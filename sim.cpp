#include<iostream>
#include"Model.h"
#include<Random.h>
#include<time.h>
#include<mpi.h>
#include<RepastProcess.h>

int main(int argc, char *argv[])
{
    Model *sim;
    try {
        MPI_Init(&argc, &argv);
//        boost::mpi::environment env(argc,argv);
        repast::RepastProcess::init("bob");

        sim = Model::instance();
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    delete sim;
	return 0;
}
