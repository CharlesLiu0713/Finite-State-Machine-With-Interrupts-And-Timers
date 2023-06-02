####           **ARM MCBSTM32C Finite State Machine with timers and interrupts**

​	This project illustrates the behavior of a finite state machine using **timers and interrupts**. We use the case of a Mealy machine. The Mealy machine is a finite state machine whose output values are determined both by its current state and the current inputs.

​	Source, headers, startup files and STM32F107xx reference manual: http://goo.gl/0Mdx44 General purpose timers are defined in section **14** of the STM32F107xx reference manual.

​	Interrupts are defined in section **9** of the STM32F107xx reference manual. **Finite State Machine**

**STEP 1: Connect and power the board**

. Connect the ULINK-ME adapter to the PC via a standard USB cable 1. 

. Connect power from your PC to the board using the USB cable 2.

**STEP 2: Create the project
** . Create a directory for the project named: ‘**statemachine’**

 . Start Keil μVision
 . **Create** a **new Project File** in **Project - New - μVision Project,** name the project **‘statemachine’** and save it in your project directory.
 **.** Select a microcontroller from the Device Database. Under STMicroelectronics, Choose the STM32F107VC . Answer **No** if prompted to add the startup code
 . To organize the project files create 2 groups:

​		Create a group named ‘**Source**’ (This group will contain your source code) 

​		Create a group named ‘**Startup**’ (This group will contain your startup files)

**Header files**

Copy the files Font_6x8_h.h, Font_16x24_h.h and GLCD.h to the project directory

**Startup files**

. Copy the files *startup_stm32f10x_cl.s* and *system_stm32f10x_cl.c* to the project directory . Add the files *startup_stm32f10x_cl.s* and *system_stm32f10x_cl.c* to group ‘Startup’ **Source files
** . Copy the files *GLCD_SPI_STM32.c, IRQ.c* and *statemachine.c* to the project directory

. Add the files *GLCD_SPI_STM32.c, IRQ.c* and *statemachine.c* to group ‘**Source**’

**STEP 3: Implementation**

For this lab we will create a finite state machine to control a toy car’s servomotor and the 08 LEDs using the ARM board. The toy car has 4 gears: **Neutral – Gear 1 - Gear 2 - Gear 3 – Gear 4**

The car starts in neutral state
 **a**- The car waits for the user to press the **‘WAKEUP’** push button. (interrupt)
 **b**- The car switches to **Gear 1** and drives for 3 seconds. (timer)
 **c**- The car switches to **Gear 2** and drives for 5 seconds. (timer)
 **d**- The car switches to **Gear 3** and holds the same gear until the driver makes a choice. (interrupt)

- if the driver presses the **‘TAMPER’** push button, the car downshift to **Gear 2**.

- if the driver presses the **‘WAKEUP’** push button, the car should cycle through the 8 LEDS on the board switches then switch to **Gear 4**.

**e**- In **Gear 4**, the car drives for 6 seconds and switches back to Neutral, the initial state. (timer)

​	The state machine uses a combination of timers loops and interrupts. You will need 5 case statements to implement the state machine. Steps **a** and **d** use interrupts. Steps **b**, **c** and **e** use timers.
​	For more details, please refer to the file **statemachine.c** and read the comments included.
​    Use the LCD to display the current states and operations of the program.

![alt text]([Toy car state diagram.png](https://github.com/CharlesLiu0713/Finite-State-Machine-With-Interrupts-And-Timers/blob/main/Toy%20car%20state%20diagram.png))

​																				**Toy car state diagram**

**STEP 4: Build and Flash the program
** . Build the program, **Project - Build target
** . Program it to the on-chip Flash ROM, **Flash - Download** 

 . Press the reset button on the board

**DEMONSTRATION:**

Demonstrate your result to the project instructor 

Print and submit the file *statemachine.c*
