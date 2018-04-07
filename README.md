CS-457 2nd Assignment

George Parasyris, #993


Task 1
	Fig1 shows all connected devices.

	Fig2, with red annotation shows the keyboard device and 
			the event handler.

	As a consequence, i use the following

	#define KEYBOARD "/dev/input/event5"

Task B
	make all (make clean to reverse)
	sudo ./keylogger -s 
		or
	sudo ./keylogger -f somefile.txt

	This process ends by
	 - Esc
	 - L_ctrl 	+ C
	 - R_ctrol 	+ C 

	 It acts as expected by the assignment

Task C

1.  The file background.c does exactly that.
    In order to terminate the process, I added an uncommon key combination
    (L_shift + R_shift + L_alt + R_alt and when all these are pressed 
    add L_ctrl to the list).

    We Fork and terminate the parrent process, and then we can see either via stdout
    or  through the specified file the child process to run on its own with full privilleges,
    logging our actions.

2.  Protecting your (banking) credentials:
    1)The first thought is autocomplete from password managers,
    If somethings isn't typed it can't realy be stolen via a keylogger.
    
    2)On top of that, we may have built a simple keylogger, that acts locally, but
    in real-world situations, such programs tend to send your information to the attack
    through the network, in case of which a Firewall would be more than welcome. It may 
    not be 100% secure, but surely adds up to your defence.

    3)Anti-spyware programs are also another line of defence, in order to keep
    your system as secure as possible.