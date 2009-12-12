OPTIONS = -Wall -std=c++0x -ggdb
EXEC_NAME = nesem

	
compile: main.o rom.o cpu.o memory.o gpu.o window.o main.o
	g++ main.o rom.o cpu.o memory.o gpu.o window.o -o ${EXEC_NAME} ${OPTIONS} -lSDL
	
main.o: main.cpp
	g++ -c main.cpp ${OPTIONS}
	
rom.o: rom.cpp rom.hpp
	g++ -c rom.cpp ${OPTIONS}
	
cpu.o: cpu.cpp cpu.hpp
	g++ -c cpu.cpp ${OPTIONS}
	
memory.o: memory.cpp memory.hpp
	g++ -c memory.cpp ${OPTIONS}
	
gpu.o: gpu.cpp gpu.hpp
	g++ -c gpu.cpp ${OPTIONS}
	
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
