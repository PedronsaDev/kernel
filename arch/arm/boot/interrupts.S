//stub

.section .text
.global vector_table
.global irq_handler
.global setup_core_for_irq

.extern serial_puts 
.type irq_handler, %function
.type setup_core_for_irq, %function

.align 5			// (32 bits)     
vector_table:			// offset |  interrupcao  |  modo de execucao 
    b _start			// 0x00   |   Reset	  |  supervisor
    b undefined_handler     	// 0x04   |   Undefined	  |  undefined
    b svc_handler           	// 0x08   |   SVC  	  |  supervisor
    b .                     	// 0x0C   |   Prefetch    |  abort
    b data_abort_handler    	// 0x10   |   Data Abort  |  abort 
    b .                     	// 0x14   |   Hyp Trap    |  hypervisor
    b irq_handler           	// 0x18   |   IRQ         |  interrupt
    b .                     	// 0x1C   |   FIQ         |  fast interrupt                    

irq_handler:
    sub lr, lr, #4
    stmfd sp!, {r0-r12, lr}

    bl timer_interrupt_c

    ldmfd sp!, {r0-r12, pc}^

setup_core_for_irq:
    ldr r0, =vector_table
    mcr p15, 0, r0, c12, c0, 0
    mrs r1, cpsr
    bic r0, r1, #0x1f
    orr r0, r0, #0xd2
    msr cpsr_c, r0
    ldr sp, =stack_top
    sub sp, sp, #8192
    bic sp, sp, #7
    msr cpsr_c, r1
    bx lr

// no arquivo real precisa ter esta função
svc_handler:
    STMFD sp!, {r1-r12, lr}  // salva o estado de usuario
    bl syscall_Handler       //chama o handler
    LDMFD sp!, {r1-r12, pc}^ // retorna para o estado de usuario
                             // após executar a chamada          

// tratadores de erro

undefined_handler:
    ldr r0, =msg_undef
    bl serial_puts
    b .                     

data_abort_handler:
    ldr r0, =msg_abort
    bl serial_puts
    b .                                 

// mensagens
.section .rodata
msg_undef:  .asciz ">>> PANIC: Instrucao indefinida! <<<\n"
msg_abort:  .asciz ">>> PANIC: Data Abort! <<<\n"
