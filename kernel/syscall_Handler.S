
.text
.global Syscall_Table
Syscall_Table:
    .word printf_handler
    .word malloc_handler
    .word fork_handler
    .word exec_handler
    .word wait_handler
    .word exit_handler


printf_handler:

    //N esta retornando ao modo usuario
    mov r0,r1
    push {lr}
    bl serial_puts
    mov r0,#1
    pop {pc}

malloc_handler:
    
fork_handler:

exec_handler:

wait_handler:

exit_handler:

.global syscall_Handler
.type syscall_Handler, %function
.extern serial_puts

.align 2
syscall_Handler:
    // é necessario implementar uma tabela para permitir a checagem das chamadas de sistema
    // a checagem atual é apenas por um if por conveniência de testes e simplicidade



    ldr r3, =Syscall_Table
    ldr r3, [r3,r0,LSL #2]
    bx r3

    mov r0, #0
    SUB r0,r0, #1
    bx lr








