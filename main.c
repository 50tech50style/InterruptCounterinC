/*********************************************************************** 

  * Interrupt Counter for Ubuntu OS 

  * DESCRIPTION : Identifies number of times keyboard is pressed during a  

  *               user specified span of time.  

  * AUTHOR :      50tech50style       

  * COURSE :      Embedded Operating Systems  

  * CONTENTS :    get_proc_interrupts_info function and main function   

  *************************************************************************/ 
 

#include <stdio.h> // Printf function, file opening/reading/writing/closing 

#include <stdlib.h> //sizeof() is defined 

#include <string.h> //also defines size_t and defines NULL macro and strstr() 

#include <unistd.h> //Provides access to POSIX operating system  

#include <time.h> //allows us to access system time 

 

/* 

 * Function: get_proc_interrupts_info 

 * ---------------------------------- 

 *   Extract info from /proc directory about interrupts  	 

 *   No function parameters. This function requires customizing the following:  

 *		1. The exact proc directory file path  

 *		2. What number your keyboard is assigned in proc directory  

 * 	3. If keyboard ID number is high buffer size needs to be changed 

 *		4. Format that proc provides info, this impacts sscanf() function call 

 *   Returns: 0 or 1   

 */ 

 

int get_proc_interrupts_info () { 

FILE* fp;   

char buffer[1024];  //Change this depending on # keyboard is assigned 

char* match; 

int keyboard_interrupts; 

 

fp = fopen("/proc/interrupts", "r"); //directory can be diff on diff pcs 

fread (buffer, 1, sizeof (buffer), fp);//read content of fp into buffer 	 

fclose(fp); 

 

match = strstr(buffer, "1: "); //Look for needle(1:) in haystack(buffer) 

//The needle needs to be changed in accordance to the number assigned  

//To your keyboard in procs directory  

 

sscanf(match,"1:\t %d", &keyboard_interrupts); //reads data from match  

//second parameter specifies format of data, store it in third param  

return keyboard_interrupts; 

} 

 

/* 

 * Function: main 

 * ---------------------------------- 

 *   Calculates number of interrupts made by keyboard   

 *   Two command line args. 1=Name of file storing info about interrupts made.  

 * 	 2=How many seconds the program should track keyboard interrupts.   

 *   Returns: 0    

 */ 

 

int main (int argc, char *argv[]) { 

 

FILE* write_to_WaveFile = fopen(argv[1], "w"); 

int user_provided_program_run_time = argv[2]; //This is in seconds  

time_t current_time = time(NULL); 

time_t previous_time = time(NULL); 

int program_runtime = 0; //Starts at 0 

int systemCall1= 0; 

int systemCall2 =0; 

pid_t shell_pid = getpid(); 

 

do { 

if (shell_pid == getpid()) { //Causes interupts to only be counted w/in prog window 			program_runtime++; 		    

current_time = time(NULL); 

if (current_time>previous_time) { 

systemCall1 = get_proc_interrupts_info(); 

previous_time = current_time; 

 

if (systemCall1 == systemCall2){ 

fprintf(write_to_WaveFile,"0"); //The value of procs hasnt changed  

 

} else if (systemCall1 != systemCall2) { 

fprintf(write_to_WaveFile, "1"); //The value of procs has changed  

} 

systemCall2 = systemCall1; //update value of current point  

}  

} 

 

 

} while (program_runtime < user_provided_program_run_time); //when the times match stop prog 

 

fclose(write_to_WaveFile); 

return 0; 

 

} 
