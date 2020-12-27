CC=g++

DEPENDENCIES := -lftd2xx -lpthread -lrt -Wl,-rpath,/usr/local/lib

APP = spiTest

all: $(APP)

$(APP): spiTest.cpp
	$(CC) spiTest.cpp -o $(APP) $(DEPENDENCIES)

clean:
	-rm -f *.o ; rm $(APP)
