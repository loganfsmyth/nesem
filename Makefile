CC=/home/thesmyth/projects/llvm/Debug+Asserts/bin/clang
OPTIONS = -Wall #-std=c++0x -ggdb
EXEC_NAME = nesem
LIBS = -lSDL -lboost_program_options -lboost_thread
	
compile: main.o rom.o cpu.o mapper.o gpu.o window.o main.o bus.o dma.o memory.o
	${CC} main.o rom.o cpu.o mapper.o gpu.o window.o bus.o dma.o memory.o -o ${EXEC_NAME} ${OPTIONS} ${LIBS}

	
main.o: main.cpp
	${CC} -c main.cpp ${OPTIONS}
	
rom.o: rom.cpp rom.hpp
	${CC} -c rom.cpp ${OPTIONS}
	
cpu.o: cpu.cpp cpu.hpp
	${CC} -c cpu.cpp ${OPTIONS}
	
mapper.o: mapper.cpp mapper.hpp
	${CC} -c mapper.cpp ${OPTIONS}
	
memory.o: memory.cpp memory.hpp
	${CC} -c memory.cpp ${OPTIONS}
	
gpu.o: gpu.cpp gpu.hpp
	${CC} -c gpu.cpp ${OPTIONS}
	
bus.o: bus.cpp bus.hpp
	${CC} -c bus.cpp ${OPTIONS}
	
dma.o: dma.cpp dma.hpp
	${CC} -c dma.cpp ${OPTIONS}
	
window.o: window.cpp window.hpp
	${CC} -c window.cpp ${OPTIONS}
	
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
