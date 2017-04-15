compile:
	g++ *.cpp -c
	g++ *.o -o Volimage

run:
	make compile
	./Volimage MRI

extract:
	make compile
	./Volimage MRI -x 10 extract

front:
	make compile
	./Volimage MRI -g 50

map:
	make compile
	./Volimage MRI -d 10 20 diffMapFile

clean:
	rm *.o
	rm ./Volimage
	cd extracted_Images && rm *.raw