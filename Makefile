all:
	gcc keylogger.c -o keylogger
	gcc background.c -o background
clean:
	rm -rf keylogger
	rm -rf background
