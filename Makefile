main: tinyrenderer.cpp
	g++ -Wall -Wextra -c tgaimage.cpp -o tgaimage.o
	g++ -Wall -Wextra -c model.cpp -o model.o
	g++ -Wall -Wextra model.o tgaimage.o tinyrenderer.cpp -o main 
	 
clean:
	rm *.o
	rm *.tga
