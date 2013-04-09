/*
 * Twi.cpp
 *
 *  Created on: 2012-9-7
 *      Author: agu
 */

#include "twi.h"

Twi::Twi(uint8_t buff_length) :
		_buff_length(buff_length) {
	_main_buff.index = 0;
	_main_buff.length = 0;
	_main_buff.buffer = (uint8_t*) malloc(sizeof(uint8_t) * _buff_length);

	_state = TWI_READY;

	_onRecive = 0;
	_onTransmit = 0;
}

Twi::~Twi() {
	free(_main_buff.buffer);
}

void Twi::init(uint32_t freq) {
	_state = TWI_READY;
	_send_stop = true; // default value
	_in_repeated_start = false;

	digitalWrite(SDA, 1);
	digitalWrite(SCL, 1);

	cbi(TWSR, TWPS0);
	cbi(TWSR, TWPS1);
	TWBR = ((F_CPU / freq) - 16) / 2;

	TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA);
}

void Twi::setAddress(uint8_t address, uint8_t gc_en) {
	TWAR = gc_en ? 0x01 : 0x00;
	TWAR |= address << 1;
}

uint8_t Twi::readFrom(uint8_t address, uint8_t *data, uint8_t length,
		uint8_t send_stop) {
	if (length > _buff_length)
		return 0;

	while (_state != TWI_READY)
		;

	_state = TWI_M_RX;
	_send_stop = send_stop;
	_error = TW_NO_ERROR;

	_main_buff.index = 0;
	_main_buff.length = length - 1;

	_sla_rw = TW_READ;
	_sla_rw |= address << 1;

	this->sendStart();

	while (_state == TWI_M_RX)
		;

	if (length > _main_buff.index)
		length = _main_buff.index;

	memcpy(data, _main_buff.buffer, length);
	return length;
}

uint8_t Twi::writeTo(uint8_t address, uint8_t * data, uint8_t length, bool wait,
		bool send_stop) {
	if (length > _buff_length)
		return 1;

	while (_state != TWI_READY)
		;

	_state = TWI_M_TX;
	_send_stop = send_stop;
	_error = TW_NO_ERROR;

	_main_buff.index = 0;
	_main_buff.length = length;

	memcpy(_main_buff.buffer, data, length);

	_sla_rw = TW_WRITE;
	_sla_rw |= address << 1;

	this->sendStart();

	while (wait && _state == TWI_M_TX)
		;

	return _error;
}

uint8_t Twi::writeTo(uint8_t address, uint8_t data, bool wait, bool send_stop) {
	return this->writeTo(address, &data, 1, wait, send_stop);
}

void Twi::reply(uint8_t ack) {
	TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | (ack ? _BV(TWEA) : 0x00);
}

void Twi::sendStart(void) {
	if (_in_repeated_start) {
		_in_repeated_start = false;
		TWDR = _sla_rw;
		TWCR = _BV(TWINT) | _BV(TWEA) | _BV(TWEN) | _BV(TWIE);
	} else
		TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTA);
}

void Twi::sendStop(void) {
	if (_send_stop)
		stop();
	else {
		_in_repeated_start = true;
		TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);
		_state = TWI_READY;
	}
}

void Twi::stop(void) {
	TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTO);

	while (TWCR & _BV(TWSTO))
		;

	_state = TWI_READY;
}

void Twi::releaseBus(void) {
	TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT);
	_state = TWI_READY;
}

void Twi::loadSlaRW(void) {
	TWDR = _sla_rw;
}

void Twi::buffOut(void) {
	if (_main_buff.index < _main_buff.length) {
		TWDR = _main_buff.buffer[_main_buff.index++];
		twi.reply(true);
	} else {
		sendStop();
	}
}

void Twi::buffIn(void) {
	_main_buff.buffer[_main_buff.index++] = TWDR;
}

bool Twi::buffOverflow(void) {
	return (_main_buff.index < _main_buff.length);
}

void Twi::error(uint8_t error) {
	_error = error;
}

void Twi::clearBuff(void) {
	_main_buff.index = 0;
	_main_buff.length = _buff_length;
}

void Twi::setState(uint8_t state) {
	_state = state;
}

void Twi::attachOnReceiveEvent(void (*fun)(uint8_t*, uint8_t)) {
	_onRecive = fun;
}

void Twi::onReceiveEvent(void) {
	if (_onRecive)
		_onRecive(_main_buff.buffer, _main_buff.index);
}

void Twi::attachOnTransmitEvent(void (*fun)(uint8_t*, volatile uint8_t*)) {
	_onTransmit = fun;
}

void Twi::onTransmitEvent(void) {
	if (_onTransmit)
		_onTransmit(_main_buff.buffer, &_main_buff.length);
	else {
		_main_buff.length = 1;
		_main_buff.buffer[0] = 0x00;
	}
}

ISR(TWI_vect) {
	switch (TW_STATUS) {
	case TW_START:
	case TW_REP_START:
		twi.loadSlaRW();
		twi.reply(true);
		break;

		// Master TX
	case TW_MT_SLA_ACK:
	case TW_MT_DATA_ACK:
		twi.buffOut();
		break;
	case TW_MT_SLA_NACK:
	case TW_MT_DATA_NACK:
	case TW_MT_ARB_LOST:
		twi.error(TW_STATUS);
		twi.stop();
		break;

		// Master RX
	case TW_MR_SLA_ACK:
		twi.reply(twi.buffOverflow());
		break;
	case TW_MR_DATA_ACK:
		twi.buffIn();
		twi.reply(twi.buffOverflow());
		break;
	case TW_MR_DATA_NACK:
		twi.buffIn();
		twi.sendStop();
		break;
	case TW_MR_SLA_NACK:
		twi.error(TW_MR_SLA_NACK);
		twi.stop();
		break;

		// SLAVE RX
	case TW_SR_SLA_ACK:
	case TW_SR_GCALL_ACK:
	case TW_SR_ARB_LOST_SLA_ACK:
	case TW_SR_ARB_LOST_GCALL_ACK:
		twi.setState(TWI_S_RX);
		twi.clearBuff();
		twi.reply(true);
		break;
	case TW_SR_DATA_ACK:
	case TW_SR_GCALL_DATA_ACK:
		twi.buffIn();
		twi.reply(twi.buffOverflow());
		break;
	case TW_SR_STOP:
		twi.stop();
		twi.onReceiveEvent();
		twi.clearBuff();
		twi.releaseBus();
		break;
	case TW_SR_DATA_NACK:
	case TW_SR_GCALL_DATA_NACK:
		twi.error(TW_STATUS);
		break;

		// SLAVE TX
	case TW_ST_SLA_ACK:
	case TW_ST_ARB_LOST_SLA_ACK:
		twi.setState(TWI_S_TX);
		twi.clearBuff();
		twi.onTransmitEvent();
		break;
	case TW_ST_DATA_ACK:
		twi.buffOut();
		break;
	case TW_ST_DATA_NACK:
	case TW_ST_LAST_DATA:
		twi.reply(true);
		twi.setState(TWI_READY);
		break;
	case TW_NO_INFO:
		break;
	case TW_BUS_ERROR:
		twi.error(TW_BUS_ERROR);
		twi.stop();
		break;
	}

}

Twi twi;
