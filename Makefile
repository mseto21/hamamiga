# Create the obj directory
CXX = g++
CXXFLAGS += `pkg-config --cflags sdl2 SDL2_image`
CXXFLAGS += -Wall -Wextra -pedantic -std=c++11
LDFLAGS += `pkg-config --libs sdl2 SDL2_mixer SDL2_image`
EXECUTABLE := fathactory
OBJDIR=obj
OBJECTS := $(patsubst src/%.cpp,$(OBJDIR)/%.o, $(wildcard src/*.cpp))

-include $(OBJECTS:.o=.d)

.PHONY: all $(OBJDIR) clean

all: build

build: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $^ $(CXXFLAGS) $(LDFLAGS) -o $@

$(OBJDIR)/%.o : src/%.cpp
	$(CXX) $< $(CXXFLAGS) -c -MD -o $@

$(OBJS): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS) $(OBJDIR)/*.d