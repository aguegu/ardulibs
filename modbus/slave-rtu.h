/*
 * slave-rtu.h
 *
 *  Created on: Jul 30, 2013
 *      Author: agu
 */

#ifndef SLAVE_RTU_H_
#define SLAVE_RTU_H_

#include "Arduino.h"
#include "digitalpin.h"
#include "crc.h"

class SlaveRtu {
public:
	SlaveRtu(HardwareSerial & usart, uint8_t address, uint8_t de, uint8_t re);
	virtual ~SlaveRtu();
	void init();

	void handler();
	void onTimIrq();

	void initBitInputs(uint16_t length);
	void initShortInputs(uint16_t length);

	void initCoils(uint16_t length);
	void initHoldings(uint16_t length);

	void setBitInput(uint16_t index, bool state);
	bool getBitInput(uint16_t index);

	void setShortInput(uint16_t index, uint16_t val);
	uint16_t getShortInput(uint16_t index);

	void setCoil(uint16_t index, bool state);
	bool getCoil(uint16_t index);

	void setHolding(uint16_t index, uint16_t val);
	uint16_t getHolding(uint16_t index);

protected:
	uint16_t _coil_length;
	uint16_t _bit_input_length;
	uint16_t _short_input_length;
	uint16_t _holding_length;

	virtual uint8_t updateBitInputs(uint16_t index, uint16_t length) {return 0;};
	virtual uint8_t updateShortInputs(uint16_t index, uint16_t length) {return 0;};
	virtual uint8_t updateCoils(uint16_t index, uint16_t length) {return 0;};
	virtual uint8_t updateHoldings(uint16_t index, uint16_t length) {return 0;};

private:
	HardwareSerial & _usart;
	volatile bool _is_receiving;
	const uint8_t _address;

	static const uint16_t _BUFF_LENGTH = 256;
	uint8_t _buff_rx[_BUFF_LENGTH];
	uint8_t _buff_tx[_BUFF_LENGTH];

	DigitalPin _de;
	DigitalPin _re;

	void appendCrcAndReply(uint8_t length_tx);
	bool checkFrameCrc(const uint8_t *p, uint8_t length);

	uint8_t * _bit_inputs;
	uint16_t * _short_inputs;
	uint8_t * _coils;
	uint16_t * _holdings;

	uint8_t onReadBitInputs(uint8_t * p_length_tx);
	uint8_t onReadShortInputs(uint8_t * p_length_tx);

	uint8_t onReadCoils(uint8_t * p_length_tx);
	uint8_t onWriteSingleCoil(uint8_t * p_length_tx);
	uint8_t onWriteMultipleCoils(uint8_t length_rx, uint8_t * p_length_tx);

	uint8_t onReadHoldings(uint8_t * p_length_tx);
	uint8_t onWriteSingleHolding(uint8_t * p_length_tx);
	uint8_t onWriteMultipleHoldings(uint8_t length_rx, uint8_t * p_length_tx);

};

#endif /* SLAVE_RTU_H_ */
