# compile the simulation

COMPILER = /usr/local/bin/mpic++
INCLUDES = -I/usr/local/include/repast_hpc -I/usr/include/boost -I/usr/include/mpich2
LIBS = -lnetcdf_c++ -lrepast_hpc-1.0.1 -lboost_mpi -lboost_serialization -lboost_serialization-mt -lboost_filesystem
LIB_DIRS = -L/shared/repast/repasthpc-1.0.1/src/repast_hpc/.libs -L/shared/repast/netcdf/netcdf-4.3.0/liblib/.libs -L/shared/repast/netcdf-cxx4-4.2/cxx4/.libs -L/shared/repast/hdf5-1.8.11/hdf5/lib/ -L/shared/repast/hdf5-1.8.11/hl/src/.libs
COMPILE_COMMAND = $(COMPILER) $(INCLUDES) $(LIBS) $(LIB_DIRS)
RUN_COMMAND = mpirun -np 1 sim 7

run: sim
	$(RUN_COMMAND)
    
runtolog: sim
	$(RUN_COMMAND) > /tmp/out
    
sim: Human.o Model.o main.o Commodity.o
	$(COMPILE_COMMAND) Human.o Model.o main.o Commodity.o -o sim
	@echo "All done"

# Make the Human object file
Human.o: Human.cpp Human.h Model.h Commodity.h
	$(COMPILE_COMMAND) -c Human.cpp

# Make the Model object file
Model.o: Model.cpp Model.h Human.h
	$(COMPILE_COMMAND) -c Model.cpp

# Make the main object file
main.o: main.cpp Model.h
	$(COMPILE_COMMAND) -c main.cpp

# Make the commodity object file
Commodity.o: Commodity.cpp Commodity.h
	$(COMPILE_COMMAND) -c Commodity.cpp

clean:
	rm -f Human.o Model.o main.o Human.o Commodity.o sim
