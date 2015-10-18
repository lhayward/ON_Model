CC = c++ 
CFLAGS = -std=c++0x -Wall -funroll-loops -O3 #-pg #-xHOST

PROG = on
HDRS = MersenneTwister.h FileReading.h Hyperrectangle.h Ising_Model.h IsingSpins.h Measure.h ON_Model.h ON_Model_Ngeq2.h SimParameters.h Vector_NDim.h VectorSpins.h
SRCS = Main.cpp FileReading.cpp Hyperrectangle.cpp Ising_Model.h IsingSpins.cpp Measure.cpp ON_Model.cpp ON_Model_Ngeq2.cpp SimParameters.cpp Vector_NDim.cpp VectorSpins.cpp
OBJS = Main.o FileReading.o Hyperrectangle.o Ising_Model.o IsingSpins.o Measure.o ON_Model.o ON_Model_Ngeq2.o SimParameters.o Vector_NDim.o VectorSpins.o

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(PROG)
	
FileReading.o: FileReading.cpp $(HDRS)
	$(CC) $(CFLAGS) -c FileReading.cpp -o FileReading.o
	
Hyperrectangle.o: Hyperrectangle.cpp $(HDRS)
	$(CC) $(CFLAGS) -c Hyperrectangle.cpp -o Hyperrectangle.o
	
Ising_Model.o: Ising_Model.cpp $(HDRS)
	$(CC) $(CFLAGS) -c Ising_Model.cpp -o Ising_Model.o
	
IsingSpins.o: IsingSpins.cpp $(HDRS)
	$(CC) $(CFLAGS) -c IsingSpins.cpp -o IsingSpins.o
	
Measure.o: Measure.cpp $(HDRS)
	$(CC) $(CFLAGS) -c Measure.cpp -o Measure.o
	
ON_Model.o: ON_Model.cpp $(HDRS)
	$(CC) $(CFLAGS) -c ON_Model.cpp -o ON_Model.o
	
ON_Model_Ngeq2.o: ON_Model_Ngeq2.cpp $(HDRS)
	$(CC) $(CFLAGS) -c ON_Model_Ngeq2.cpp -o ON_Model_Ngeq2.o
	
SimParameters.o: SimParameters.cpp $(HDRS)
	$(CC) $(CFLAGS) -c SimParameters.cpp -o SimParameters.o
	
Vector_NDim.o: Vector_NDim.cpp $(HDRS)
	$(CC) $(CFLAGS) -c Vector_NDim.cpp -o Vector_NDim.o
	
VectorSpins.o: VectorSpins.cpp $(HDRS)
	$(CC) $(CFLAGS) -c VectorSpins.cpp -o VectorSpins.o
	
Main.o: Main.cpp $(HDRS)
	$(CC) $(CFLAGS) -c Main.cpp -o Main.o 
	
clean:
	rm -f $(PROG) $(OBJS) 
