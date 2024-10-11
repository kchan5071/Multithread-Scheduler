compile:
	g++ -std=c++11 scheduler.cpp -o scheduler.o -c
	g++ -std=c++11 process.cpp -o process.o -c
	gcc log.c -o log.o -c
	g++ -std=c++11 schedule.cpp -o schedule -c

# link:
 	g++ -std=c++11 -pthread process.o scheduler.o log.o main.o -o schedule


run:
	make compile
	./schedule bursts.txt -a 0.5 
	./schedule -a 1 bursts_animation.txt
	./schedule bursts_invalid_odd.txt
	./schedule bursts_invalid_zero.txt
	rm schedule

clean:
	rm -f *.o
	rm -f schedule