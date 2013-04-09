/*
 * Twi.h
 *
 *  Created on: 2012-9-7
 *      Author: agu
 */

#include <util/twi.h>
#include <avr/interrupt.h>
#include <Arduino.h>

#ifndef TWI_H_
#define TWI_H_

#define TWI_READY	0
#define TWI_M_RX   1
#define TWI_M_TX   2
#define TWI_S_RX   3
#define TWI_S_TX   4

#define TW_NO_ERROR	0xff

struct linear_buff {
	uint8_t *buffer;
	volatile uint8_t index;
	volatile uint8_t length;
};

class Twi {
public:

	Twi(uint8_t buff_size = 32);
	virtual ~Twi();

	void init(uint32_t freq = 100000UL);
	void setAddress(uint8_t address, uint8_t gc_en = false);

	uint8_t readFrom(uint8_t address, uint8_t * data, uint8_t length,
			uint8_t send_stop);
	uint8_t writeTo(uint8_t address, uint8_t * data, uint8_t length, bool wait,
			bool send_stop);

	uint8_t writeTo(uint8_t address, uint8_t data, bool wait = true,
			bool send_stop = true);

	void sendStart(void);
	void sendStop(void);

	void reply(uint8_t);
	void stop(void);

	void releaseBus(void);
	void loadSlaRW(void);

	void buffOut(void);
	void buffIn(void);

	bool buffOverflow(void);

	void error(uint8_t);

	void clearBuff(void);
	void setState(uint8_t);

	void (*_onRecive)(uint8_t *, uint8_t);
	void attachOnReceiveEvent(void (*)(uint8_t*, uint8_t));
	void onReceiveEvent(void);

	void (*_onTransmit)(uint8_t *, volatile uint8_t*);
	void attachOnTransmitEvent(void (*)(uint8_t*, volatile uint8_t*));
	void onTransmitEvent(void);

private:

	volatile uint8_t _state;
	volatile uint8_t _sla_rw;
	volatile bool _send_stop;
	volatile uint8_t _in_repeated_start;
	volatile uint8_t _error;
	linear_buff _main_buff;
	const uint8_t _buff_length;
};

extern Twi twi;

#endif /* TWI_H_ */
