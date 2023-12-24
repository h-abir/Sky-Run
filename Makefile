# Makefile for sky run

CC = g++
CFLAGS = -Wall -Wextra -std=c++11
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf


SRCDIR = src
OBJDIR = obj
BINDIR = .

SRCFILES := $(wildcard $(SRCDIR)/*.cpp)
OBJFILES := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCFILES))
EXECUTABLE = $(BINDIR)/myapp

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJFILES)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJFILES) $(EXECUTABLE)
