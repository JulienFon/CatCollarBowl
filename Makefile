SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:.c=.o)

CFLAGSA := -L./libs/cJSON/lib -I./libs/cJSON/include -lcjson -lcurl `pkg-config --libs gtk+-3.0` -lwiringPi
CFLAGSB := `pkg-config --cflags gtk+-3.0`

%.o: %.c
	gcc $(CFLAGSB) -o $@ $< $(CFLAGSA)

all: $(OBJS)

clean:
	rm -rf build
