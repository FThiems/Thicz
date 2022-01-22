main: tren.cpp
	g++ -Wall -Wextra -c tgaimage.cpp -o tgaimage.o
	g++ -Wall -Wextra tgaimage.o tren.cpp -o main 
