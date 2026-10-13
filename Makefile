CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -Isrc

SRC = src/main.cpp src/LanguageModel.cpp src/TextGenerator.cpp
DEPS = src/LanguageModel.h src/TextGenerator.h
OBJ = $(SCR: .cpp=.o)
TARGET = slm

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean 