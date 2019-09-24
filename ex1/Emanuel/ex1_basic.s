.syntax unified

.include "efm32gg.s"

// Exception vector table
// This table contains addresses for all exception handlers

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

// Reset handler
// The CPU will start executing here after a reset

.globl  _reset
.type   _reset, %function
.thumb_func
_reset:
    // power down RAM blocks 1-3 to save energy
    // saves only about 0.5 uA in theory, reading shows like 0.2 uA difference?
    ldr r0, =EMU_BASE
    ldr r1, =7
    str r1, [r0, #EMU_MEMCTRL]

    // configure clocks
    ldr r0, =CMU_BASE

    // switch to low frequency clock to save energy
    // lets power consumption go from 3.5 mA down to 42.4 uA
    // enable it
    ldr r1, =(1 << CMU_OSCENCMD_LFRCOEN)
    str r1, [r0, #CMU_OSCENCMD]
    // switch to it
    ldr r1, =(3 << CMU_CMD_HFCLKSEL)
    str r1, [r0, #CMU_CMD]
    // disable high frequency clock
    ldr r1, =(1 << CMU_OSCENCMD_HFRCODIS)
    str r1, [r0, #CMU_OSCENCMD]

    // enable GPIO clock
    ldr r1, [r0, #CMU_HFPERCLKEN0]
    orr r1, r1, #(1 << CMU_HFPERCLKEN0_GPIO)
    str r1, [r0, #CMU_HFPERCLKEN0]


    // init LEDs (GPIO_PA)
    ldr r0, =GPIO_PA_BASE

    // set high drive strength
    ldr r1, =1                  // easier on the eyes than 2
    str r1, [r0, #GPIO_CTRL]

    // set pins 8-15 to output
    ldr r1, =0x55555555
    str r1, [r0, #GPIO_MODEH]

    // turn off LEDs
    ldr r1, =(0xff << 8)
    str r1, [r0, #GPIO_DOUT]


    // init buttons (GPIO_PC)
    ldr r0, =GPIO_PC_BASE

    // set pins 8-15 to output
    ldr r1, =0x33333333
    str r1, [r0, #GPIO_MODEL]

    // enable internal pull-up
    ldr r1, =0xff
    str r1, [r0, #GPIO_DOUT]


.thumb_func
main_loop:
    bl read_buttons

    // write to LEDs
    ldr r1, =GPIO_PA_BASE
    lsl r0, r0, #8
    str r0, [r1, #GPIO_DOUT]

    b main_loop


// returns button status in r0
.thumb_func
read_buttons:
    // read button status
    ldr r1, =GPIO_PC_BASE
    ldr r0, [r1, #GPIO_DIN]
    bx LR


.thumb_func
dummy_handler:
    b .  // do nothing

