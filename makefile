CC = $(GBDKDIR)bin/lcc
CFLAGS = -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG
BUILDPATH = build/rom

all: clean main.gb

main.gb: 
	mkdir -p $(BUILDPATH)
	$(CC) $(CFLAGS) -c -o build/main.o main.c
	$(CC) $(CFLAGS) -o build/rom/main.gb build/main.o

clean:
	rm -fr build

