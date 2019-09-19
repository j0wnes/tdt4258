        .syntax unified
	
	      .include "efm32gg.s"
		  
//this version uses interrupts, a verion without interrupts should be developed first
//useful link (DO NOT JUST COPY)
//https://github.com/pbsds/tdt4258_2018_group_14/blob/master/ex1/Improved/ex1.s

	/////////////////////////////////////////////////////////////////////////////
	//
  // Exception vector table
  // This table contains addresses for all exception handlers
	//
	/////////////////////////////////////////////////////////////////////////////
	
        .section .vectors
	
	      .long   stack_top               /* Top of Stack                 */
	      .long   _reset                  /* Reset Handler                */
	      .long   dummy_handler           /* NMI Handler                  */
	      .long   dummy_handler           /* Hard Fault Handler           */
	      .long   dummy_handler           /* MPU Fault Handler            */
	      .long   dummy_handler           /* Bus Fault Handler            */
	      .long   dummy_handler           /* Usage Fault Handler          */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* SVCall Handler               */
	      .long   dummy_handler           /* Debug Monitor Handler        */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* PendSV Handler               */
	      .long   dummy_handler           /* SysTick Handler              */

	      /* External Interrupts */
	      .long   dummy_handler
	      .long   gpio_handler            /* GPIO even handler */
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   gpio_handler            /* GPIO odd handler */
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler

	      .section .text

	/////////////////////////////////////////////////////////////////////////////
	//
	// Reset handler
  // The CPU will start executing here after a reset
	//
	/////////////////////////////////////////////////////////////////////////////

	      .globl  _reset
	      .type   _reset, %function
        .thumb_func
_reset: 
	      // Enable GPIO clock in CMU
		  ldr r1, =CMU_BASE
		  ldr r2, [r1, #CMU_HFPERCLKEN0]		  
		  mov r3, #1
		  lsl r3, r3, #CMU_HFPERCLKEN0_GPIO
		  orr r2, r2, r3		  
		  str r2, [r1, #CMU_HFPERCLKEN0]
		  
		  
		  // Setting pin 8-15 for output
		  ldr r5, =GPIO_PA_BASE
		  ldr r1, =0x2
		  str r1, [r5, #GPIO_CTRL]		  
		  ldr r1, =0x55555555
		  str r1, [r5, #GPIO_MODEH]
		  
		  
		  // setting pins 0-7 up to be read
		  ldr r5, =GPIO_PC_BASE
		  ldr r1, =0x33333333
		  str r1, [r5, #GPIO_MODEL]
		  
		  ldr r1, =0xff					// enable internal pull_up for buttons
		  str r1, [r5, #GPIO_DOUT]
		  
		  
		  // setting pins 0-7 as interrupt generators
		  ldr r5, =GPIO_BASE
		  ldr r1, =0x22222222
		  str r1, [r5, #GPIO_EXTIPSELL]
		  
		  //pressing butten results in falling edge --> interrupt on falling edge
		  ldr r1, =0xff
		  str r1, [r5, #GPIO_EXTIFALL]
		  
		  
		  //enable interrupt generation
		  ldr r5, =GPIO_BASE
		  ldr r1, =0xff
		  str r1, [r5, #GPIO_IEN]
		  
		  
		  
		  //energy saving techniques
		  
		  
		  
		  
		  //everything relatet to initialization/"default settings"
		  
		  //enabling interrupt handling
		  ldr r5, =ISER0
		  ldr r1, =0x802
		  str r1, [r5]
		  
	
	/////////////////////////////////////////////////////////////////////////////
	//
  // GPIO handler
  // The CPU will jump here when there is a GPIO interrupt
	//
	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
gpio_handler:  

	      b .  // do nothing
	
	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
dummy_handler:  
        b .  // do nothing

