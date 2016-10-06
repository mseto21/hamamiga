# Create the obj directory
CXX = g++
CXXFLAGS += `pkg-config --cflags sdl2 SDL2_image`
CXXFLAGS += -Wall -Wextra -pedantic -std=c++11
LDFLAGS += `pkg-config --libs sdl2 SDL2_mixer SDL2_image SDL2_ttf`
EXECUTABLE := fathactory
OBJDIR=obj
OBJECTS := $(patsubst src/%.cpp,$(OBJDIR)/%.o, $(wildcard src/*.cpp))

test := $(shell test -d $(OBJDIR) || mkdir -p $(OBJDIR))

-include $(OBJECTS:.o=.d)

all: build

build: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $^ $(CXXFLAGS) $(LDFLAGS) -o $@

$(OBJDIR)/%.o : src/%.cpp
	$(CXX) -c $< $(CXXFLAGS) -o $@

$(OBJDIR)/%.o : src/%.h

-include $(OBJECTS:.o=.d)

.PHONY: clean
clean:
	rm -rf $(EXECUTABLE) $(OBJDIR)/*.o $(OBJDIR)/*.d