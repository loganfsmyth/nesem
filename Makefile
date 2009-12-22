OPTIONS = -Wall -std=c++0x -ggdb
EXEC_NAME = nesem

	
compile: main.o rom.o cpu.o mapper.o gpu.o window.o main.o bus.o
	g++ main.o rom.o cpu.o mapper.o gpu.o window.o bus.o -o ${EXEC_NAME} ${OPTIONS} -lSDL -lboost_program_options-mt -lboost_thread-mt
	
main.o: main.cpp
	g++ -c main.cpp ${OPTIONS}
	
rom.o: rom.cpp rom.hpp
	g++ -c rom.cpp ${OPTIONS}
	
cpu.o: cpu.cpp cpu.hpp
	g++ -c cpu.cpp ${OPTIONS}
	
mapper.o: mapper.cpp mapper.hpp
	g++ -c mapper.cpp ${OPTIONS}
	
gpu.o: gpu.cpp gpu.hpp
	g++ -c gpu.cpp ${OPTIONS}
	
bus.o: bus.cpp bus.hpp
	g++ -c bus.cpp ${OPTIONS}
	
window.o: window.cpp window.hpp
	g++ -c window.cpp ${OPTIONS}
	
clean:	
	rm *.o
	rm ${EXEC_NAME}
	
run:	nesem
	./${EXEC_NAME}
	
kill:
	killall -9 ${EXEC_NAME}
	
runkill: nesem
	killall -9 ${EXEC_NAME}
	./${EXEC_NAME}
