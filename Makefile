CXX = g++
CXXFLAGS += `pkg-config --cflags sdl2 SDL2_image`
CXXFLAGS += -Wall -Werror -Wextra -Weffc++ -pedantic -std=c++11
LDFLAGS += `pkg-config --libs sdl2 SDL2_image`
EXEC := fathactory
OBJS := $(patsubst src/%.cpp,obj/%.o, $(wildcard src/*.cpp))
DEPS := $(OBJS:.o=.d)

.PHONY: all clean

all: build

build: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $^ $(CXXFLAGS) $(LDFLAGS) -o $@

obj/%.o : src/%.cpp
	$(CXX) $< $(CXXFLAGS) -c -MD -o $@

-include $(DEPS)

clean:
	rm -rf $(EXEC) $(OBJS)