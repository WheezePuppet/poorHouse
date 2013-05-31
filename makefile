# compile the simulation
sim: Human.o Model.o sim.o
	g++ Human.o Model.o sim.o -o sim
	@echo "All done"
	rm Human.o Model.o sim.o
# Make the Human object file
Human.o: Human.cpp Human.h
	g++ -c Human.cpp

# Make the Model object file
Model.o: Model.cpp Model.h Human.h
	g++ -c Model.cpp

# Make the main object file
sim.o: sim.cpp Model.h
	g++ -c sim.cpp
