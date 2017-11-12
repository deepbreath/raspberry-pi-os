#include <stddef.h>
#include <stdint.h>

#include "printf.h"
#include "utils.h"
#include "timer.h"
#include "irq.h"
#include "fork.h"
#include "sched.h"
#include "mini_uart.h"

void process(char *array)
{
	while (1){
		for (int i = 0; i < 5; i++){
			uart_send(array[i]);
			DELAY(100000);
		}
	}
}

void error(char *message){
	printf(message);
}

void kernel_main(void)
{
	uart_init();
	init_printf(NULL, putc);
	irq_vector_init();
	timer_init();
	enable_interrupt_controller();
	enable_processor_interrupts();

	int res = copy_process(1, (unsigned long)&process, (unsigned long)"12345");
	if (res != 0) {
		error("error while starting process 1");
		return;
	}
	res = copy_process(2, (unsigned long)&process, (unsigned long)"abcde");
	if (res != 0) {
		error("error while starting process 2");
		return;
	}

	while (1){
		schedule();
	}	
}