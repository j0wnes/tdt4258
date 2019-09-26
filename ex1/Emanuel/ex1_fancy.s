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

    // enable GPIO clock
    ldr r1, [r0, #CMU_HFPERCLKEN0]
    orr r1, r1, #(1 << CMU_HFPERCLKEN0_GPIO)
    str r1, [r0, #CMU_HFPERCLKEN0]


    // init LEDs (GPIO_PA)
    ldr r0, =GPIO_PA_BASE

    // set high drive strength
    ldr r1, =2
    str r1, [r0, #GPIO_CTRL]

    // set pins 8-15 to output
    ldr r1, =0x55555555
    str r1, [r0, #GPIO_MODEH]

    // turn off LEDs
    ldr r1, =(0xff << 8)
    str r1, [r0, #GPIO_DOUT]


    // init buttons (GPIO_PC)
    ldr r0, =GPIO_PC_BASE

    // set pins 0-7 to input
    ldr r1, =0x33333333
    str r1, [r0, #GPIO_MODEL]

    // enable internal pull-up
    ldr r1, =0xff
    str r1, [r0, #GPIO_DOUT]


    // configure interrupts
    ldr r0, =GPIO_BASE

    // select GPIO Port C for external interrupts 0-7
    ldr r1, =0x22222222
    str r1, [r0, #GPIO_EXTIPSELL]

    // activate triggering of interrupts for rising edges
    ldr r1, =0xff
    str r1, [r0, #GPIO_EXTIRISE]

    // enable GPIO interrupt generation
    // r1 = 0xff
    str r1, [r0, #GPIO_IEN]

    // reset GPIO interrupt flags to make sure they are all inactive
    ldr r1, [r0, #GPIO_IF]
    str r1, [r0, #GPIO_IFC]


    // enable deep sleep and sleep on return from interrupt
    ldr r0, =SCR
    ldr r1, =6
    str r1, [r0]

    // r11 always holds the current LED position
    // set initial LED position to 0 for LED 1
    ldr r11, =0


    // all set up, so finally enable interrupt handling
    ldr r0, =ISER0
    ldr r1, =0x802
    str r1, [r0]

    // ready and waiting for button presses
    wfi


.thumb_func
move_left:
    // check if at the beginning
    cmp r11, #0
    // if not substract 1
    it ne
    subne r11, #1
    bx LR

.thumb_func
move_right:
    // check if at the end
    cmp r11, #7
    // if not add 1
    it ne
    addne r11, #1
    bx LR

.thumb_func
toggle_intensity:
    // load current intensity
    ldr r0, =GPIO_PA_BASE
    ldr r1, [r0, #GPIO_CTRL]
    // if 2 set to 1, if not, set to 2
    cmp r1, #2
    ite eq
    ldreq r1, =1
    ldrne r1, =2
    str r1, [r0, #GPIO_CTRL]
    bx LR

.thumb_func
toggle_led:
    // put 1 in current position
    ldr r1, =(1 << 8)
    lsl r1, r1, r11
    // toggle it
    ldr r0, =GPIO_PA_BASE
    str r1, [r0, #GPIO_DOUTTGL]
    bx LR


// GPIO handler
// The CPU will jump here when there is a GPIO interrupt   
.thumb_func
gpio_handler:
    push {LR}

    // read which button triggered the interrupt
    ldr r1, =GPIO_BASE
    ldr r0, [r1, #GPIO_IF]

    // left button pressed on either of the button crosses
    ands r2, r0, #0b00010001
    it ne
    blne move_left

    // right button pressed on either of the button crosses
    ands r2, r0, #0b01000100
    it ne
    blne move_right

    // up button pressed on either of the button crosses
    ands r2, r0, #0b00100010
    it ne
    blne toggle_intensity

    // down button pressed on either of the button crosses
    ands r2, r0, #0b10001000
    it ne
    blne toggle_led

    // clear the interrupt
    ldr r1, =GPIO_BASE
    ldr r0, [r1, #GPIO_IF]
    str r0, [r1, #GPIO_IFC]

    pop {LR}
    bx LR


.thumb_func
dummy_handler:
    b .  // do nothing
