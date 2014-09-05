TARGET = comb_perm_test
SOURCE = $(TARGET).cpp combination.h permutation.h comb_impl.h perm_impl.h

CPP = g++
FLAGS_DEBUG = -std=c++11 -Wall -g -o $(TARGET)
FLAGS_BUILD = -std=c++11 -Wall -O3 -o $(TARGET)

default: $(SOURCE)
	$(CPP) $(FLAGS_DEBUG) $(SOURCE)

build: $(SOURCE)
	$(CPP) $(FLAGS_BUILD) $(SOURCE)

clean:
	rm $(TARGET)


