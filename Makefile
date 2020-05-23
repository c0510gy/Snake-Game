CC		:= g++
CFLAGS	:= -std=c++14 -Wall -Wextra -g -lnCurses

BIN		:= bin
SRC		:= src
BUILDDIR := build
INCLUDE	:= include
LIB		:= lib

SRCEXT := cpp

LIBRARIES	:=

ifeq ($(OS),Windows_NT)
EXECUTABLE	:= main.exe
SOURCEDIRS	:= $(SRC)
INCLUDEDIRS	:= $(INCLUDE)
LIBDIRS		:= $(LIB)
else
EXECUTABLE	:= main.o
SOURCEDIRS	:= $(shell find $(SRC) -type d)
INCLUDEDIRS	:= $(shell find $(INCLUDE) -type d)
LIBDIRS		:= $(shell find $(LIB) -type d)
endif

CINCLUDES	:= $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))
CLIBS		:= $(patsubst %,-L%, $(LIBDIRS:%/=%))

SOURCES		:= $(wildcard $(patsubst %,%/*.$(SRCEXT), $(SOURCEDIRS)))
OBJECTS		:= $(patsubst $(SOURCEDIRS)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

all: $(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(OBJECTS)
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) $(CINCLUDES) $(CLIBS) $^ -o $@ $(LIBRARIES)

$(BUILDDIR)/%.o: $(SOURCEDIRS)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(CINCLUDES) -c -o $@ $<"; $(CC) $(CFLAGS) $(CINCLUDES) -c -o $@ $<

tester: $(OBJECTS)
	g++ -std=c++14 -Wall -Wextra -g -lnCurses test/test.cpp -Iinclude -Iinclude/Snake-Game -Llib build/Snake-Game.o build/interface.o build/FileManager.o -o test/test.o

.PHONY: clean
clean:
	-$(RM) $(BIN)/$(EXECUTABLE)
	-$(RM) $(OBJECTS)

run: all
	./$(BIN)/$(EXECUTABLE)
