#ifndef CV2_MIDIMessages_h
#define CV2_MIDIMessages_h

const uint8_t DO_NOT_SAVE_VALUE = 0x80;

const uint8_t MIDI_CONTROL_CHANGE = 0xb0;
const uint8_t MIDI_PROGRAM_CHANGE = 0xc0;

// Control Change controller numbers
const uint8_t CV_OUTPUT_MIN_CC = 0;
const uint8_t CV_OUTPUT_MAX_CC = 1;
const uint8_t CV_OUTPUT_SOURCE_CC = 3;
const uint8_t CV_OUTPUT_SIDE_CHAIN_SOURCE_CC = 5;
const uint8_t CV_OUTPUT_SIDE_CHAIN_MIN_CC = 6;
const uint8_t CV_OUTPUT_SIDE_CHAIN_MAX_CC = 7;
const uint8_t CV_OUTPUT_SIDE_CHAIN_MODE_CC = 8;

const uint8_t TRIGGER_CONTROL_TYPE_CC = 9;
const uint8_t TRIGGER_MODE_CC = 10;

const uint8_t FREEZE_TRIGGER_MIDI_CHANNEL_CC = 11;


// the MIDI channels associated with each trigger
// if triggers become unassigned, change these to be CC values that select the
// trigger from the pool (and add a CC for each thing that needs a trigger)
const uint8_t FREEZE_1_TRIGGER_MIDI_CHANNEL = 0;
const uint8_t FREEZE_2_TRIGGER_MIDI_CHANNEL = 1;
const uint8_t ENV_STATE_TRIGGER_MIDI_CHANNEL = 2;
const uint8_t EXPR_TRIGGER_MIDI_CHANNEL = 3;
const uint8_t RAMP_TRIGGER_MIDI_CHANNEL = 4;
const uint8_t NOISE_TRIGGER_MIDI_CHANNEL = 5;
const uint8_t LFO1_TRIGGER_MIDI_CHANNEL = 6;
const uint8_t LFO2_TRIGGER_MIDI_CHANNEL = 7;

const uint8_t ENV_STATE_LOW_HIGH_THRESHOLD_CC = 13;
const uint8_t ENV_STATE_HIGH_LOW_THRESHOLD_CC = 14;
const uint8_t ENV_STATE_THRESHOLD_SENSITIVITY_CC = 15;
const uint8_t ENV_STATE_TRIGGER_MIDI_CHANNEL_CC = 16;

const uint8_t EXPR_TRIGGER_MIDI_CHANNEL_CC = 17;

const uint8_t RAMP_TIME_CC = 18;
const uint8_t RAMP_DIRECTION_CC = 19;
const uint8_t RAMP_TRIGGER_MIDI_CHANNEL_CC = 20;

const uint8_t NOISE_TRIGGER_MIDI_CHANNEL_CC = 21;
const uint8_t NOISE_RATE_CC = 22;
const uint8_t NOISE_SMOOTHING_CC = 23;

// LFO
const uint8_t LFO_FREQUENCY_CC = 24;
const uint8_t LFO_FREQUENCY_RANGE_CC = 25;
const uint8_t LFO_TRIGGER_MIDI_CHANNEL_CC = 26;

const uint8_t DISPLAY_RESET_CC = 71;
const uint8_t DISPLAY_TEST_CC = 72;
const uint8_t PATCH_SAVE_CC = 73;
const uint8_t PATCH_COPY_CC = 74;
const uint8_t ERASE_PATCH = 75;

// the values assigned to source types -  enum?
const uint8_t CV_OUTPUT_SOURCE_FIXED_VALUE = 1;
const uint8_t CV_OUTPUT_SOURCE_LFO1_VALUE = 2;
const uint8_t CV_OUTPUT_SOURCE_EXPR1_VALUE = 3;
const uint8_t CV_OUTPUT_SOURCE_EXPR2_VALUE = 4;
const uint8_t CV_OUTPUT_SOURCE_ENV_VALUE = 5;
const uint8_t CV_OUTPUT_SOURCE_RAMP_VALUE = 6;
const uint8_t CV_OUTPUT_SOURCE_NOISE_VALUE = 7;
const uint8_t CV_OUTPUT_SOURCE_LFO2_VALUE = 8;

const uint8_t CV_OUTPUT_SIDE_CHAIN_SOURCE_NONE_VALUE = 0;
const uint8_t CV_OUTPUT_SIDE_CHAIN_SOURCE_LFO1_VALUE = 1;
const uint8_t CV_OUTPUT_SIDE_CHAIN_SOURCE_EXPR1_VALUE = 2;
const uint8_t CV_OUTPUT_SIDE_CHAIN_SOURCE_EXPR2_VALUE = 3;
const uint8_t CV_OUTPUT_SIDE_CHAIN_SOURCE_LFO2_VALUE = 4;

const uint8_t CV_OUTPUT_SIDE_CHAIN_MODE_MIN_VALUE = 1;
const uint8_t CV_OUTPUT_SIDE_CHAIN_MODE_MAX_VALUE = 2;
const uint8_t CV_OUTPUT_SIDE_CHAIN_MODE_RANGE_VALUE = 3;

const uint8_t TRIGGER_CONTROL_NONE_VALUE = 0;
const uint8_t TRIGGER_CONTROL_FSW_1_VALUE = 1;
const uint8_t TRIGGER_CONTROL_FSW_2_VALUE = 2;
const uint8_t TRIGGER_CONTROL_FSW_3_VALUE = 3;
const uint8_t TRIGGER_CONTROL_ENV_VALUE = 4;

const uint8_t TRIGGER_MODE_LEVEL_NON_INV_VALUE = 0;
const uint8_t TRIGGER_MODE_LEVEL_INV_VALUE = 1;
const uint8_t TRIGGER_MODE_EDGE_OFF_ON_VALUE = 2;
const uint8_t TRIGGER_MODE_EDGE_ON_OFF_VALUE = 3;

const uint8_t RAMP_DIRECTION_UP_VALUE = 0;
const uint8_t RAMP_DIRECTION_DOWN_VALUE = 1;

// values for ERASE_PATCH
const uint8_t ERASE_ALL_VALUE = 0x7f;
const uint8_t ERASE_CURRENT_VALUE = 0x7e;

#endif
