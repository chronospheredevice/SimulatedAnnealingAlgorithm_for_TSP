TARGET = libsaatsp.so
OBJS = saarandom.o saatsp.o stdafx.o
CC = g++

$(TARGET): $(OBJS)
	$(CC) -shared -O2 -o $@ $(OBJS)
%.o : %.cpp
	$(CC) -fPIC -c $^ -o $@

.PHONY:clean
clean:
	-rm -rf $(OBJS)

