CXX := gcc
CFLAGS := -g -ansi -pedantic-errors -ftime-report
LDFLAGS := -lglfw -lGLEW -lGLU -lGL -ldl -lm 
SRCE := ./source/
TARGET := main

SRCS := $(wildcard  *.c $(SRCE)*.c)
OBJS := $(patsubst %.c,%.o,$(SRCS))

all: $(TARGET)
%.o: %.c
	$(CXX) $(CFLAGS) -c $< -o $@
$(TARGET): $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)
clean:
	rm -rf $(TARGET) *.o $(SRCE)*.o
	
.PHONY: all clean
