.PHONY: all clean

all: rqt

rqt: main.c
	gcc -O2 -pipe -D_FORTIFY_SOURCE=2 -Wl,-z,relro -Wl,-z,now -flto -lm -o $@ $^

clean:
	rm rqt
