#ifndef CV2_MIDIMessages_h
#define CV2_MIDIMessages_h

const uint8_t CV_OUTPUT_MIN_CC = 0;
const uint8_t CV_OUTPUT_MAX_CC = 1;

const uint8_t CV_OUTPUT_SOURCE_CC = 3;
// the values assigned to source types -  enum?
const uint8_t CV_OUTPUT_SOURCE_FIXED_VALUE = 1;
const uint8_t CV_OUTPUT_SOURCE_LFO_VALUE = 2;
const uint8_t CV_OUTPUT_SOURCE_EXPR1_VALUE = 3;
const uint8_t CV_OUTPUT_SOURCE_EXPR2_VALUE = 4;

const uint8_t CV_OUTPUT_SIDE_CHAIN_SOURCE_CC = 5;
const uint8_t CV_OUTPUT_SIDE_CHAIN_SOURCE_NONE_VALUE = 0;
const uint8_t CV_OUTPUT_SIDE_CHAIN_SOURCE_LFO_VALUE = 1;
const uint8_t CV_OUTPUT_SIDE_CHAIN_SOURCE_EXPR1_VALUE = 2;
const uint8_t CV_OUTPUT_SIDE_CHAIN_SOURCE_EXPR2_VALUE = 3;

const uint8_t CV_OUTPUT_SIDE_CHAIN_MIN_CC = 6;
const uint8_t CV_OUTPUT_SIDE_CHAIN_MAX_CC = 7;

const uint8_t CV_OUTPUT_SIDE_CHAIN_MODE_CC = 8;
const uint8_t CV_OUTPUT_SIDE_CHAIN_MODE_MIN_VALUE = 1;
const uint8_t CV_OUTPUT_SIDE_CHAIN_MODE_MAX_VALUE = 2;
const uint8_t CV_OUTPUT_SIDE_CHAIN_MODE_RANGE_VALUE = 3;

const uint8_t LFO_FREQUENCY_CC = 2;

const uint8_t DISPLAY_RESET_CC = 71;
const uint8_t DISPLAY_TEST_CC = 72;

#endif
