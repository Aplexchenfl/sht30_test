
all:
	arm-linux-gnueabihf-gcc sht30.c  -o sht30
	sync

clean:
	rm sht30 -rf
	sync
