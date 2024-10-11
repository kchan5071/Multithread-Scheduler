run:
	g++ -std=c++11 schedule.cpp -o schedule
	./schedule bursts.txt -a 0.5
	./schedule -a 1 bursts_animation.txt
	./schedule bursts_invalid_odd.txt
	./schedule bursts_invalid_zero.txt
	rm schedule