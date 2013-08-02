/*
 * slave-rtu.cpp
 *
 *  Created on: Jul 30, 2013
 *      Author: agu
 */

#include "slave-rtu.h"

SlaveRtu::SlaveRtu(HardwareSerial & usart, uint8_t address) :
		_usart(usart), _address(address) {
	_is_receiving = true;

	_bit_inputs = NULL;
	_bit_input_length = 0;

	_short_inputs = NULL;
	_short_input_length = 0;

	_coils = NULL;
	_coil_length = 0;

	_holdings = NULL;
	_holding_length = 0;

	this->initBitInputs(16);
	this->initShortInputs(8);
	this->initCoils(32);
	this->initHoldings(16);
}

SlaveRtu::~SlaveRtu() {
	free(_bit_inputs);
	free(_short_inputs);

	free(_coils);
	free(_holdings);
}

void SlaveRtu::initBitInputs(uint16_t length) {
	if (_bit_inputs)
		free(_bit_inputs);

	_bit_input_length = length;
	uint8_t bit_input_byte_size = (_bit_input_length + 7) >> 3;
	_bit_inputs = (uint8_t *) malloc(bit_input_byte_size * sizeof(uint8_t));

	memset(_bit_inputs, 0, bit_input_byte_size);
}

void SlaveRtu::initShortInputs(uint16_t length) {
	if (_short_inputs)
		free(_short_inputs);

	_short_input_length = length;
	_short_inputs = (uint16_t *) malloc(_short_input_length * sizeof(uint16_t));

	memset(_short_inputs, 0, _short_input_length * sizeof(uint16_t));
}

void SlaveRtu::initCoils(uint16_t length) {
	if (_coils)
		free(_coils);

	_coil_length = length;
	uint8_t coils_byte_size = (_coil_length + 7) >> 3;
	_coils = (uint8_t *) malloc(coils_byte_size * sizeof(uint8_t));

	memset(_coils, 0, coils_byte_size);
}

void SlaveRtu::initHoldings(uint16_t length) {
	if (_holdings)
		free(_holdings);

	_holding_length = length;
	_holdings = (uint16_t *) malloc(_holding_length * sizeof(uint16_t));

	memset(_holdings, 0, _holding_length * sizeof(uint16_t));
}

void SlaveRtu::init() {
#if defined(TCCR2)
	TIMER_CS(TCCR2, 2, TIMER_WITHOUT_EXT_CLK_CS_NUL);
	TIMER_2BIT_WAVEFORM(2, TIMER_2BIT_WAVEFORM_PCPWM);
	sbi(TIMSK2, OCIE2);
#elif defined(TCCR2A) && defined(TCCR2B)
	TIMER_CS(TCCR2B, 2, TIMER_WITHOUT_EXT_CLK_CS_NUL);
	TIMER_3BIT_WAVEFORM(2, TIMER_3BIT_WAVEFORM_CTC);
	// (11/19200) * 3.5 / (1/(16000000/128))
	OCR2A = 250;
	cbi(TIMSK2, OCIE2A);
#else
#error Timer 2 reg not found
#endif

	_usart.begin(19200, SERIAL_8E1);
}

void SlaveRtu::handler() {

	extern DigitalPin led;
	static uint16_t length_rx = 0;

	if (_usart.available()) {
		_buff_rx[length_rx++] = _usart.read();
		TCNT2 = 0;
		sbi(TIMSK2, OCIE2A);
		TIMER_CS(TCCR2B, 2, TIMER_WITHOUT_EXT_CLK_CS_128);
		led.set(HIGH);
	}

	if (_is_receiving == false) {
		led.set(LOW);
		memset(_buff_tx, 0, _BUFF_LENGTH);

		do {
			if (length_rx < 4 || length_rx >= _BUFF_LENGTH)
				break;
			if (_buff_rx[0] && _buff_rx[0] != _address)
				break;
			if (this->checkFrameCrc(_buff_rx, length_rx) == false)
				break;

			_buff_tx[0] = _address;
			_buff_tx[1] = _buff_rx[1];	// function code

			uint8_t length_tx = 1;
			uint8_t exception = 0;

			switch (_buff_rx[1]) {
			case 0x01:
				exception = onReadCoils(&length_tx);
				break;
			case 0x02:
				exception = onReadBitInputs(&length_tx);
				break;
			case 0x03:
				exception = onReadHoldings(&length_tx);
				break;
			case 0x04:
				exception = onReadShortInputs(&length_tx);
				break;
			case 0x05:
				exception = onWriteSingleCoil(&length_tx);
				break;
			case 0x06:
				exception = onWriteSingleHolding(&length_tx);
				break;
			case 0x0f:
				exception = onWriteMultipleCoils(length_rx, &length_tx);
				break;
			case 0x10:
				exception = onWriteMultipleHoldings(length_rx, &length_tx);
				break;
			default:
				exception = 0x01;
				break;
			}

			if (exception) {
				_buff_tx[1] += 0x80;
				_buff_tx[2] = exception;
				length_tx = 3;
			}

			if (_buff_tx[0]) this->appendCrcAndReply(length_tx);

		} while (false);

		_is_receiving = true;
		length_rx = 0;
	}
}

void SlaveRtu::onTimIrq() {
	_is_receiving = false;
}

void SlaveRtu::appendCrcAndReply(uint8_t length_tx) {
	uint16_t v = crc.calc(_buff_tx, length_tx);
	_buff_tx[length_tx] = lowByte(v);
	_buff_tx[length_tx + 1] = highByte(v);
	_usart.Print::write(_buff_tx, length_tx + 2);
	_usart.flush();
}

bool SlaveRtu::checkFrameCrc(const uint8_t *p, uint8_t length) {
	uint16_t crc0 = crc.calc(p, length - 2);
	uint16_t crc1 = p[length - 2] | (p[length - 1] << 8);
	return crc0 == crc1;
}

void SlaveRtu::setBitInput(uint16_t index, bool state) {
	bitWrite(_bit_inputs[index >> 3], index & 0x07, state);
}

bool SlaveRtu::getBitInput(uint16_t index) {
	return bitRead(_bit_inputs[index >> 3], index & 0x07);
}

void SlaveRtu::setShortInput(uint16_t index, uint16_t val) {
	_short_inputs[index] = val;
}

uint16_t SlaveRtu::getShortInput(uint16_t index) {
	return _short_inputs[index];
}

void SlaveRtu::setCoil(uint16_t index, bool state) {
	bitWrite(_coils[index >> 3], index & 0x07, state);
}

bool SlaveRtu::getCoil(uint16_t index) {
	return bitRead(_coils[index >> 3],index & 0x07);
}

void SlaveRtu::setHolding(uint16_t index, uint16_t val) {
	_holdings[index] = val;
}

uint16_t SlaveRtu::getHolding(uint16_t index) {
	return _holdings[index];
}

uint8_t SlaveRtu::onReadBitInputs(uint8_t * p_length_tx) {
	uint16_t length = make16(_buff_rx[4], _buff_rx[5]);
	if (!length || length > 0x07d0)
		return 0x03;

	uint16_t address = make16(_buff_rx[2], _buff_rx[3]);
	if (address + length > _bit_input_length)
		return 0x02;

	if (updateBitInputs(address, length))
		return 0x04;

	for (uint16_t i = 0; i < length; i++)
		bitWrite(_buff_tx[3 + (i >> 3)], i & 0x07,
				this->getBitInput(address + i));

	_buff_tx[2] = (length + 7) >> 3;

	*p_length_tx = 3 + _buff_tx[2];
	return 0;
}

uint8_t SlaveRtu::onReadShortInputs(uint8_t * p_length_tx) {
	uint16_t length = make16(_buff_rx[4], _buff_rx[5]);
	if (!length || length > 0x07d)
		return 0x03;

	uint16_t index = make16(_buff_rx[2], _buff_rx[3]);
	if (index + length > _short_input_length)
		return 0x02;

	if (updateShortInputs(index, length))
		return 0x04;

	for (uint8_t i = 0; i < length; i++) {
		_buff_tx[3 + i + i] = highByte(_short_inputs[index + i]);
		_buff_tx[4 + i + i] = lowByte(_short_inputs[index + i]);
	}

	_buff_tx[2] = length * 2;
	*p_length_tx = _buff_tx[2] + 3;
	return 0;
}

uint8_t SlaveRtu::onReadCoils(uint8_t * p_length_tx) {

	uint16_t length = make16(_buff_rx[4], _buff_rx[5]);
	if (!length || length > 0x07d0)
		return 0x03;

	uint16_t address = make16(_buff_rx[2], _buff_rx[3]);
	if (address + length > _coil_length)
		return 0x02;

	for (uint16_t i = 0; i < length; i++) {
		bitWrite(_buff_tx[3 + (i >> 3)], i & 0x07, this->getCoil(address + i));
	}

	_buff_tx[2] = (length + 7) >> 3;

	*p_length_tx = 3 + _buff_tx[2];
	return 0;
}

uint8_t SlaveRtu::onWriteSingleCoil(uint8_t * p_length_tx) {
	uint16_t val = make16(_buff_rx[4], _buff_rx[5]);
	if (val && val != 0xff00)
		return 0x03;

	uint16_t address = make16(_buff_rx[2], _buff_rx[3]);
	if (address >= _coil_length)
		return 0x02;

	this->setCoil(address, val);

	if (updateCoils(address, 1))
		return 0x04;

	memcpy(_buff_tx + 2, _buff_rx + 2, 4);
	*p_length_tx = 6;
	return 0;
}

uint8_t SlaveRtu::onWriteMultipleCoils(uint8_t length_rx,
		uint8_t * p_length_tx) {
	uint16_t length = make16(_buff_rx[4], _buff_rx[5]);
	if (!length || length > 0x07b0)
		return 0x03;
	if (length_rx - 9 != _buff_rx[6])
		return 0x03;

	uint16_t index = make16(_buff_rx[2], _buff_rx[3]);
	if (index + length > _coil_length)
		return 0x02;

	for (uint16_t i = 0; i < length; i++)
		this->setCoil(index + i, bitRead(_buff_rx[7 + (i >> 3)], i & 0x07));

	if (updateCoils(index, length))
		return 0x04;

	memcpy(_buff_tx + 2, _buff_rx + 2, 4);
	*p_length_tx = 6;
	return 0;
}

uint8_t SlaveRtu::onReadHoldings(uint8_t * p_length_tx) {
	uint16_t length = make16(_buff_rx[4], _buff_rx[5]);
	if (!length || length > 0x07d) return 0x03;

	uint16_t index = make16(_buff_rx[2], _buff_rx[3]);
	if (index + length > _holding_length) return 0x02;

	for (uint8_t i = 0; i < length; i++) {
		_buff_tx[3 + i + i] = highByte(_holdings[index + i]);
		_buff_tx[4 + i + i] = lowByte(_holdings[index + i]);
	}

	_buff_tx[2] = length * 2;
	*p_length_tx = _buff_tx[2] + 3;
	return 0;
}

uint8_t SlaveRtu::onWriteSingleHolding(uint8_t * p_length_tx) {
	uint16_t val = make16(_buff_rx[4], _buff_rx[5]);

	uint16_t index = make16(_buff_rx[2], _buff_rx[3]);
	if (index >= _holding_length) return 0x02;

	this->setHolding(index, val);

	if (updateHoldings(index, 1)) return 0x04;

	memcpy(_buff_tx + 2, _buff_rx + 2, 4);
	*p_length_tx = 6;
	return 0;
}

uint8_t SlaveRtu::onWriteMultipleHoldings(uint8_t length_rx,
	uint8_t * p_length_tx) {
	uint16_t length = make16(_buff_rx[4], _buff_rx[5]);
	if (!length || length > 0x7b) return 0x03;
	if (length_rx - 9 != _buff_rx[6]) return 0x03;
	if ((uint8_t)length != _buff_rx[6] >> 1) return 0x03;

	uint16_t index = make16(_buff_rx[2], _buff_rx[3]);
	if (index + length > _holding_length) return 0x02;

	for (uint8_t i = 0; i < length; i++) {
		_holdings[index + i] =
		make16(_buff_rx[7 + i + i], _buff_rx[8 + i + i]);
	}

	if (updateHoldings(index, length)) return 0x04;

	memcpy(_buff_tx + 2, _buff_rx + 2, 4);
	*p_length_tx = 6;
	return 0;
}



ISR(TIMER2_COMPA_vect) {
	TIMER_CS(TCCR2B, 2, TIMER_WITHOUT_EXT_CLK_CS_NUL);
	cbi(TIMSK2, OCIE2A);
	extern SlaveRtu node;
	node.onTimIrq();
}

