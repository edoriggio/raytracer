render: main.cpp
	g++ main.cpp -w
	./a.out

display-mac:
	open ./result.ppm

display-linux:
	gimp ./result.ppm &

video:
	./scripts/animate.sh
	./scripts/convert.sh

clean:
	rm -f ./a.out
	rm -f ./*.ppm
	rm -rf ./out
