compile:
	gcc log.c -o log.o -c
	g++ -std=c++11 -pthread -c process.cpp -o process.o
	g++ -std=c++11 -pthread -c scheduler.cpp -o scheduler.o
	g++ -std=c++11 -pthread schedule.cpp process.o scheduler.o log.o -o schedule
	
run:
	make compile
	./schedule bursts_invalid_odd.txt
	./schedule bursts_invalid_zero.txt
	./schedule bursts.txt -a 0.5 
	./schedule -a 1 bursts_animation.txt
	make clean

clean:
	rm -f *.o
	rm -f schedule