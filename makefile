CC = g++
BINDIR = bin
CFLAGS = -Wall `pkg-config --cflags opencv4`
LDFLAGS = `pkg-config --libs opencv4`

all: vid showFaces

vid: vidDisplay.o filter.o faceDetect.o
	$(CC) $^ -o $(BINDIR)/$@ $(LDFLAGS)

showFaces: showFaces.o filter.o faceDetect.o
	$(CC) $^ -o $(BINDIR)/$@ $(LDFLAGS)

vidDisplay.o: vidDisplay.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

showFaces.o: showFaces.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

filter.o: filter.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

faceDetect.o: faceDetect.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -rf *.o $(BINDIR)/vid $(BINDIR)/showFaces

