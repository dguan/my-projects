TARGET = test_observer
SOURCE = $(TARGET).cpp SubjectObserver.h

CPP = g++
FLAGS_DEBUG = -std=c++11 -Wall -g -o $(TARGET)
FLAGS_BUILD = -std=c++11 -Wall -O3 -o $(TARGET)


default : $(SOURCE)
	$(CPP) $(FLAGS_DEBUG) $(SOURCE)
	
build : $(SOURCE)
	$(CPP) $(FLAGS_BUILD) $(SOURCE)
	
clean :
	rm $(TARGET)
	

