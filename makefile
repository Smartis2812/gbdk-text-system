CC = $(GBDKDIR)/bin/lcc
CFLAGS = -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG
BUILDPATH = build/rom

all: clean main.gb sameboy

main.gb: 
	mkdir -p $(BUILDPATH)
	$(CC) $(CFLAGS) -c -o build/main.o main.c
	$(CC) $(CFLAGS) -o build/rom/main.gb build/main.o

clean:
	if pgrep SameBoy; then killall SameBoy; fi
	rm -fr build

sameboy:
	# SameBoy is my emulator on OSX
	open /Applications/SameBoy.app  --args $(realpath build/rom/main.gb)

