CFLAGS = -ggdb -O2 -Wall
LDFLAGS = `pkg-config --cflags --libs fontconfig`
OBJECTS = has_char

all: $(OBJECTS)
clean :
	\rm $(OBJECTS)
.PHONY: all clean

% : %.c
