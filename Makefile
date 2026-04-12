CXX := gcc
CFLAGS := -g -std=c99 -Wall -Wextra -Wpedantic -pedantic-errors -ftime-report -Wdeclaration-after-statement
LDFLAGS := -lglfw -lGLEW -lGLU -lGL -ldl -lm -lcglm
SCENES := ./scenes/
SRCE := ./source/
TARGET := main

SRCS := $(wildcard  *.c $(SRCE)*.c $(SCENES)*.c)
OBJS := $(patsubst %.c,%.o,$(SRCS))

all: $(TARGET)
%.o: %.c
	$(CXX) $(CFLAGS) -c $< -o $@
$(TARGET): $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)
clean:
	rm -rf $(TARGET) *.o $(SRCE)*.o
	
.PHONY: all clean
