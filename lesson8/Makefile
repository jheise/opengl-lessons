all:
	g++ -c Camera.cpp
	g++ -o vbo Camera.o vbo-lesson8.cpp -lGL -lGLEW -lglfw -ljpeg `freetype-config --libs`  -I/usr/include/freetype2
clean:
	rm -rf *.o vbo
