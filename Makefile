CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Isrc

SRCS = src/main.cpp src/LanguageModel.cpp src/TextGenerator.cpp
DEPS = src/LanguageModel.h src/TextGenerator.h
OBJS = src/main.o src/LanguageModel.o src/TextGenerator.o
TARGET = slm

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

src/%.o: src/%.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean