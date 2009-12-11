OPTIONS = -Wall -std=c++0x -ggdb
EXEC_NAME = nesem

	
NESEM: NESEM.o Rom.o CPU.o Memory.o GPU.o Window.o
	g++ NESEM.o Rom.o CPU.o Memory.o GPU.o Window.o -o ${EXEC_NAME} ${OPTIONS} -lSDL
	
NESEM.o: NESEM.cpp
	g++ -c NESEM.cpp ${OPTIONS}
	
Rom.o: Rom.cpp Rom.h
	g++ -c Rom.cpp ${OPTIONS}
	
CPU.o: CPU.cpp CPU.h
	g++ -c CPU.cpp ${OPTIONS}
	
Memory.o: Memory.cpp Memory.h
	g++ -c Memory.cpp ${OPTIONS}
	
GPU.o: GPU.cpp GPU.h
	g++ -c GPU.cpp ${OPTIONS}
	
Window.o: Window.cpp Window.h
	g++ -c Window.cpp ${OPTIONS}
	
clean:	
	rm *.o
	rm ${EXEC_NAME}
	
run:	NESEM
	./${EXEC_NAME}
	
kill:
	killall -9 ${EXEC_NAME}
	
runkill: NESEM
	killall -9 ${EXEC_NAME}
	./${EXEC_NAME}