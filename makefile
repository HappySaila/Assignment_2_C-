compile:
	g++ *.cpp -c
	g++ *.o -o Volimage

run:
	make compile
	./Volimage MRI

extract:
	make compile
	./Volimage MRI -x 10 output

map:
	make compile
	./Volimage MRI -d 10 20 diffMapFile

both:
	make compile
	./Volimage MRI -x 5 outfileName -d 10 20 outfileName

both2:
	make compile
	./Volimage MRI -d 10 20 outfileName -x 5 outfileName

clean:
	rm *.o
	rm ./Volimage