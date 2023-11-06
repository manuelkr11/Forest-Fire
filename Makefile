# Define source files
SRCS = main.cpp ff-ca.cpp neighbors.cpp Timing.cpp

# Define object files
OBJS = $(SRCS:.cpp=.o)

# Define compiler and flags
CXX = g++
CXXFLAGS = -Isrc/include -Ofast -fopenmp

# Define linker and flags
LDFLAGS = -Lsrc/lib -lmingw32 -mwindows -lSDL2main -lSDL2 -fopenmp -Ofast

# Target for the final executable
TARGET = main

# Default target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJS)