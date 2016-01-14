CXX = 			g++
CXXFLAGS = 		#-Wall
LDLIBS =		-lglut -lGLEW -lGL -lSOIL

CPP_FILES = main.cpp shader.cpp shape.cpp mathHelper.cpp camera.cpp lighting.cpp
OBJFILES = main.o shader.o shape.o mathHelper.o camera.o lighting.o

main: $(OBJFILES)
	$(CXX) $(CXXFLAGS) -o main $(OBJFILES) $(LDLIBS)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp $(LDLIBS)

shader.o: shader.cpp
	$(CXX) $(CXXFLAGS) -c shader.cpp $(LDLIBS)

shape.o: shape.cpp
	$(CXX) $(CXXFLAGS) -c shape.cpp $(LDLIBS)

mathHelper.o: mathHelper.cpp
	$(CXX) $(CXXFLAGS) -c mathHelper.cpp $(LDLIBS)

camera.o: camera.cpp
	$(CXX) $(CXXFLAGS) -c camera.cpp $(LDLIBS)

lighting.o: lighting.cpp
	$(CXX) $(CXXFLAGS) -c lighting.cpp $(LDLIBS)

# Dependencies

main.o: shader.h shape.h mathHelper.h camera.h
shader.o: shader.h
shape.o: shape.h
mathHelper.o: mathHelper.h
camera.o: camera.h
lighting.o: lighting.h

# Clean

clean:
	rm *.o main