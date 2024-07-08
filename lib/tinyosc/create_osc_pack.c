#include "tinyosc.h"
#include <zephyr/kernel.h>

/**
 * A basic program to listen to port 9000 and print received OSC packets.
 */
int create_msg(char *tx_buffer, int max_len) {

	  printk("Starting write tests:\n");
	  int len = 0;
	  len = tosc_writeMessage(tx_buffer, max_len, "/address", "fs", 2.0, "Hello");
	  printk("Print osc:\n");
	  tosc_printOscBuffer(tx_buffer, len);
	  return(len);
}
void read_msg(char* buffer, int len) {
	  tosc_message osc;
	  tosc_parseMessage(&osc, buffer, len);
	  printk("Print msg:\n");
	  tosc_printMessage(&osc);
}
