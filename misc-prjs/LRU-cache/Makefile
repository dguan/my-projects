TARGET = LRU
TARGET_UT = LRU_ut

SRC_COMMON = $(TARGET).h
SOURCE = $(SRC_COMMON) $(TARGET)_main.cpp
SOURCE_UT = $(SRC_COMMON) $(TARGET_UT)Main.cpp

CXX = g++
FLAGS_BUILD = -std=c++11 -Wall -O3 -o $(TARGET)
FLAGS_UT  = -std=c++11 -Wall -o $(TARGET_UT) -lgtest -lpthread
FLAGS_DEBUG = -std=c++11 -Wall -g -o $(TARGET)

build : $(SOURCE)
	$(CXX) $(SOURCE) $(FLAGS_BUILD)
	
test : $(SOURCE_UT)
	$(CXX) $(SOURCE_UT) $(FLAGS_UT)
	
debug : $(SOURCE)
	$(CXX) $(SOURCE) $(FLAGS_DEBUG)

clean :
	rm $(TARGET) $(TARGET_UT)

