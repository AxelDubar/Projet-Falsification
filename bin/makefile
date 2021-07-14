CPPFLAGS += -I/usr/local/include/ImageMagick-7

LDFLAGS += -L/usr/local/lib

LDLIBS += -lrtmidi

convertisseur:	src/jpeg-9d/convertisseur.cpp
	g++ -o convertisseur src/jpeg-9d/convertisseur.cpp -std=c++17 -Dcimg_use_png -lpng -lz -lX11 -lpthread
	#g++ -o convertisseur src/jpeg-9d/convertisseur.cpp -std=c++17 $(CPPFLAGS) -lz -lX11 -lpthread
	
main: src/main.cpp src/image_ppm.h src/Bloc.h src/Clist.h
	g++ -o main src/main.cpp 
	
main_all: src/main_all.cpp src/image_ppm.h src/Bloc.h src/Clist.h
	g++ -o main_all src/main_all.cpp -std=c++17
	
test: src/test.cpp src/image_ppm.h src/Bloc.h src/Clist.h
	g++ -o test src/test.cpp 

test_all: src/test_all.cpp src/image_ppm.h src/Bloc.h src/Clist.h
	g++ -o test_all src/test_all.cpp -std=c++17
 
	
