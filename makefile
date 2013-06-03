# compile the simulation
sim: Human.o Model.o sim.o Commodity.o
	grepo Human.o Model.o sim.o Commodity.o -o sim
	@echo "All done"
	rm Human.o Model.o sim.o Human.o Commodity.o
# Make the Human object file
Human.o: Human.cpp Human.h Model.h Commodity.h
	grepo -c Human.cpp

# Make the Model object file
Model.o: Model.cpp Model.h Human.h
	grepo -c Model.cpp

# Make the main object file
sim.o: sim.cpp Model.h
	grepo -c sim.cpp

# Make the commodity object file
Commodity.o: Commodity.cpp Commodity.h
	grepo -c Commodity.cpp
