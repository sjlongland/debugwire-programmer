#include "protocol/interface.h"
#include "protocol/state.h"
#include "protocol/delimiter.h"

static struct proto_state_t state;

static void host_rx_evth(struct fifo_t* const fifo, uint8_t events);
static void target_rx_evth(struct fifo_t* const fifo, uint8_t events);

/*! Initialise the protocol handler */
void proto_init() {
	state.state = PROTO_STATE_START;
	proto_host_uart_rx.consumer_evth = host_rx_evth;
	proto_host_uart_rx.consumer_evtm = FIFO_EVT_NEW;
	proto_target_uart_rx.consumer_evth = target_rx_evth;
	proto_target_uart_rx.consumer_evtm = FIFO_EVT_NEW;
}

/*! Handle the internal tick counter */
void proto_tick() {
	timer_tick(&state.timer);
}

static void host_rx_evth(struct fifo_t* const fifo, uint8_t events) {
}

static void target_rx_evth(struct fifo_t* const fifo, uint8_t events) {
}
