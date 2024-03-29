/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                 Technische Informatik II                                      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                                               *
 *                                         M A I N                                               *
 *                                                                                               *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* * * * * * * * * * * * * * * * * * * * * * * * *\
#                   INCLUDES                      #
\* * * * * * * * * * * * * * * * * * * * * * * * */
#include "machine/multiboot.h"
#include "machine/cpu.h"
#include "machine/pic.h"
#include "machine/plugbox.h"

#include "device/keyboard.h"
#include "device/cgastr.h"
#include "device/panic.h"

#include "user/task1.h"


/* * * * * * * * * * * * * * * * * * * * * * * * *\
#                    MACROS                       #
\* * * * * * * * * * * * * * * * * * * * * * * * */
/// \~german  festlegen, welche Aufgabenanwendung verwendet werden soll
/// \~english define which task is desired
#define USE_TASK           30

//load the necessary header and define the class name of the task
#if USE_TASK == 10
  #include "user/task1.h"
  typedef Task1 TaskClass;
  
#elif USE_TASK == 20
  #include "user/task2.h"
  typedef Task2 TaskClass;

#elif USE_TASK == 30
  #include "user/task3A.h"
  #include "user/task3B.h"
  typedef Task3A TaskClass;
  typedef Task3B SndTaskClass;
  
#elif USE_TASK == 31
  #include "user/task3A.h"
  typedef Task3A TaskClass;
  
#elif USE_TASK == 32
  #include "user/task3B.h"
  typedef Task3B TaskClass;
  
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * *\
#                GLOBAL OBJECTS                   #
\* * * * * * * * * * * * * * * * * * * * * * * * */
CPU cpu;
PIC pic;
Panic panic;
Plugbox plugbox;
CGA_Stream kout;
Keyboard keyboard;

unsigned int globalTaskChoice;

/* * * * * * * * * * * * * * * * * * * * * * * * *\
#                   METHODS                       #
\* * * * * * * * * * * * * * * * * * * * * * * * */
extern "C" void kernel(uint32_t magic, const Multiboot_Info* info);

/** \brief kernel entry point
 *
 * \param magic bootloader magic value
 * \param info address of multiboot info structure
 *
 * This is the entry point of the operating system.  If this function returns
 * all interrupts will be disabled and the cpu will be halted.
 * 
 **/
void kernel(uint32_t magic, const Multiboot_Info* info){
    #if USE_TASK == 31
    TaskClass task1;
    SndTaskClass task2;

    kout.clear();
    kout.setpos(25,1);
    kout << "OOStuBs - Task 3A und Task 3B" << endl << endl;
    while(true){
        kout.flush();
        globalTaskChoice = 0;
        kout << endl << "bitte Aufgabe waehlen (Alt + [1/2])" << endl;
        while(globalTaskChoice!=1 && globalTaskChoice!=2){
            cpu.halt();
        }
        if(1 == globalTaskChoice){
            task1.action();
        }else{
            task2.action();
        }
    }
    #else
    #if USE_TASK == 10
        TaskClass task(magic, info);
    #else
        TaskClass task;
    #endif

    task.action();
    #endif
  
}
