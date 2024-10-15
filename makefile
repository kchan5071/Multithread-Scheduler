compile:
	gcc log.c -o log.o -c
	g++ -std=c++11 -pthread -c process.cpp -o process.o
	g++ -std=c++11 -pthread -c scheduler.cpp -o scheduler.o
	g++ -std=c++11 -pthread -c sorter.cpp -o sorter.o
	g++ -std=c++11 -pthread -c threaded.cpp -o threaded.o
	g++ -std=c++11 -pthread schedule.cpp process.o scheduler.o log.o sorter.o threaded.o -o schedule
	
schedule:
	make compile
run:
	make compile
#	./schedule bursts_invalid_odd.txt
#	./schedule bursts_invalid_zero.txt
	./schedule -a .1 bursts_animation.txt
	./schedule hard_test.txt
	./schedule -a 0.5 bursts.txt
	make clean

clean:
	rm -f *.o
	rm -f schedule