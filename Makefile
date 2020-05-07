################################################################################
# I3D Assignemnt 01 - Island Defence
################################################################################
# make
# make clean
################################################################################
# Run Command Format:
# ./id
################################################################################
HEADERS=wave.h boat.h island.h projectile.h defence.h keyboard.h functions.h vec2f.h vec2fPolar.h vec3f.h colour.h"
SPURCES=id.cpp wave.cpp boat.cpp island.cpp projectile.cpp defence.cpp keyboard.cpp
OBJECTS=id.o wave.o boat.o island.o projectile.o defence.o keyboard.o
CC=g++
CXXFLAGS=-Wall -Werror -lglut -lGLU -lGL -lm

all: id

id: $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(CXXFLAGS)

id.o:
	$(CC) $(CXXFLAGS) -c id.cpp

wave.o:
	$(CC) $(CXXFLAGS) -c wave.cpp

boat.o:
	$(CC) $(CXXFLAGS) -c boat.cpp

island.o:
	$(CC) $(CXXFLAGS) -c island.cpp

projectile.o:
	$(CC) $(CXXFLAGS) -c projectile.cpp

defence.o:
	$(CC) $(CXXFLAGS) -c defence.cpp

keyboard.o:
	$(CC) $(CXXFLAGS) -c keyboard.cpp

clean:
	rm -f $(OBJECTS) id *~
