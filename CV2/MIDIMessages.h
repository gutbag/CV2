#ifndef CV2_MIDIMessages_h
#define CV2_MIDIMessages_h

const uint8_t DO_NOT_SAVE_VALUE = 0x80;

const uint8_t MIDI_CONTROL_CHANGE = 0xb0;
const uint8_t MIDI_PROGRAM_CHANGE = 0xc0;

// Control Change controller numbers
//const uint8_t CV_OUTPUT_MIN_CC = 0;
//const uint8_t CV_OUTPUT_MAX_CC = 1;
//const uint8_t CV_OUTPUT_SOURCE_CC = 3;
//const uint8_t CV_OUTPUT_SIDE_CHAIN_SOURCE_CC = 5;
//const uint8_t CV_OUTPUT_SIDE_CHAIN_MIN_CC = 6;
//const uint8_t CV_OUTPUT_SIDE_CHAIN_MAX_CC = 7;
//const uint8_t CV_OUTPUT_SIDE_CHAIN_MODE_CC = 8;
const uint8_t CV_OUTPUT_SOURCE_CC = 3;
const uint8_t CV_OUTPUT_LOW_MIN_CC = 0; // SourceValueProvider CCs
const uint8_t CV_OUTPUT_LOW_MAX_CC = 5;
const uint8_t CV_OUTPUT_LOW_SOURCE_CC = 6;
const uint8_t CV_OUTPUT_HIGH_MIN_CC = 1; // SourceValueProvider CCs
const uint8_t CV_OUTPUT_HIGH_MAX_CC = 7;
const uint8_t CV_OUTPUT_HIGH_SOURCE_CC = 8;

const uint8_t TRIGGER_CONTROL_TYPE_CC = 9;
const uint8_t TRIGGER_MODE_CC = 10;

const uint8_t FREEZE_TRIGGER_INSTANCE_CC = 11;

const uint8_t ENV_FOLLOWER_GAIN_CC = 39;
const uint8_t ENV_FOLLOWER_TRIGGER_INSTANCE_CC = 41;

const uint8_t ENV_STATE_LOW_HIGH_THRESHOLD_CC = 13;
const uint8_t ENV_STATE_HIGH_LOW_THRESHOLD_CC = 14;
const uint8_t ENV_STATE_THRESHOLD_SENSITIVITY_CC = 15;
const uint8_t ENV_STATE_TRIGGER_INSTANCE_CC = 16;

const uint8_t EXPR_TRIGGER_INSTANCE_CC = 17;
const uint8_t EXPR_INVERT_CC = 2;

const uint8_t RAMP_TIME_CC = 18;
const uint8_t RAMP_INVERT_CC = 19;
const uint8_t RAMP_TRIGGER_INSTANCE_CC = 20;

const uint8_t NOISE_TRIGGER_INSTANCE_CC = 21;
const uint8_t NOISE_RATE_MIN_CC = 22;
const uint8_t NOISE_RATE_MAX_CC = 33;
const uint8_t NOISE_RATE_SOURCE_CC = 34;
const uint8_t NOISE_SMOOTHING_MIN_CC = 23;
const uint8_t NOISE_SMOOTHING_MAX_CC = 35;
const uint8_t NOISE_SMOOTHING_SOURCE_CC = 36;

// LFO
const uint8_t LFO_FREQUENCY_MIN_CC = 24;
const uint8_t LFO_FREQUENCY_MAX_CC = 31;
const uint8_t LFO_FREQUENCY_SOURCE_CC = 32;
const uint8_t LFO_FREQUENCY_RANGE_CC = 25;
const uint8_t LFO_TRIGGER_INSTANCE_CC = 26;
const uint8_t LFO_PHASE_OFFSET_CC = 12;

// Bus
const uint8_t BUS_SOURCE_1_CC = 27;
const uint8_t BUS_SOURCE_2_CC = 28;
const uint8_t BUS_MIX_MIN_CC = 29;
const uint8_t BUS_TRIGGER_INSTANCE_CC = 30;
const uint8_t BUS_MIX_MAX_CC = 37;
const uint8_t BUS_MIX_SOURCE_CC = 38;

// footswitch
const uint8_t FOOTSWITCH_TRIGGER_INSTANCE_CC = 40;

// AWG
const uint8_t AWG_TRIGGER_INSTANCE_CC = 50;
const uint8_t AWG_LOOP_CC = 51;
const uint8_t AWG_INVERT_CC = 62;
const uint8_t AWG_MS_PER_TIME_STEP_MIN_CC = 52;
const uint8_t AWG_MS_PER_TIME_STEP_MAX_CC = 53;
const uint8_t AWG_MS_PER_TIME_STEP_SOURCE_CC = 54;
const uint8_t AWG_PH1_SUSTAIN_CC = 55;
const uint8_t AWG_PH2_SUSTAIN_CC = 56;
const uint8_t AWG_PH3_SUSTAIN_CC = 57;
const uint8_t AWG_PH4_SUSTAIN_CC = 58;
const uint8_t AWG_PH5_SUSTAIN_CC = 59;
const uint8_t AWG_PH6_SUSTAIN_CC = 60;
const uint8_t AWG_PH7_SUSTAIN_CC = 61;
//const uint8_t AWG_PH8_SUSTAIN_CC = 62;
const uint8_t AWG_PH1_END_AMPLITUDE_CC = 63;
const uint8_t AWG_PH2_END_AMPLITUDE_CC = 64;
const uint8_t AWG_PH3_END_AMPLITUDE_CC = 65;
const uint8_t AWG_PH4_END_AMPLITUDE_CC = 66;
const uint8_t AWG_PH5_END_AMPLITUDE_CC = 67;
const uint8_t AWG_PH6_END_AMPLITUDE_CC = 68;
const uint8_t AWG_PH7_END_AMPLITUDE_CC = 69;
//const uint8_t AWG_PH8_END_AMPLITUDE_CC = 70;
const uint8_t AWG_PH1_DURATION_CC = 90;
const uint8_t AWG_PH2_DURATION_CC = 91;
const uint8_t AWG_PH3_DURATION_CC = 92;
const uint8_t AWG_PH4_DURATION_CC = 93;
const uint8_t AWG_PH5_DURATION_CC = 94;
const uint8_t AWG_PH6_DURATION_CC = 95;
const uint8_t AWG_PH7_DURATION_CC = 96;
const uint8_t AWG_PH8_DURATION_CC = 97;

const uint8_t TEMPO_BPM_CC = 98;
const uint8_t TEMPO_TRIGGER_INSTANCE_CC = 102;

const uint8_t DISPLAY_RESET_CC = 71;
const uint8_t DISPLAY_TEST_CC = 72;

const uint8_t CPU_METER_CC = 80;

// TODO: could move above patch-related CCs to this (as unique values)
const uint8_t PATCH_CONTROL_CC = 81;

// values sent on channel 0, feature enable/disable on channel 1
const uint8_t PITCHFORK_TRIGGER_INSTANCE_CC = 82;
const uint8_t PITCHFORK_SHIFT_CC = 83;
const uint8_t PITCHFORK_LATCH_CC = 84;
const uint8_t PITCHFORK_OCTAVE_CC = 85; // up/down/both
const uint8_t PITCHFORK_BLEND_CC = 86;

// Axoloti CCs - prefixed with CV2_ to differentiate them from the Axo's own CCs
const uint8_t CV2_AXOLOTI_PATCH_NO_CC = 87; // ch 0
const uint8_t CV2_AXOLOTI_PRESET_NO_CC = 88; // ch 0
const uint8_t CV2_AXOLOTI_VAR_SOURCE_CC = 105;
// multi-channel CCs
const uint8_t CV2_AXOLOTI_VAR_LOW_MIN_CC = 89; // SourceValueProvider CCs
const uint8_t CV2_AXOLOTI_VAR_LOW_MAX_CC = 99;
const uint8_t CV2_AXOLOTI_VAR_LOW_SOURCE_CC = 100;
const uint8_t CV2_AXOLOTI_VAR_HIGH_MIN_CC = 101; // SourceValueProvider CCs
const uint8_t CV2_AXOLOTI_VAR_HIGH_MAX_CC = 103;
const uint8_t CV2_AXOLOTI_VAR_HIGH_SOURCE_CC = 104;

const uint8_t CV2_AXOLOTI_MIDI_OUTPUT_ON_VALUE_CC = 106;
const uint8_t CV2_AXOLOTI_MIDI_OUTPUT_TRIGGER_INSTANCE_CC = 107;

// the values assigned to source types -  enum?
const uint8_t CV_OUTPUT_SOURCE_FIXED_VALUE = 1;
const uint8_t CV_OUTPUT_SOURCE_LFO1_VALUE = 2;
const uint8_t CV_OUTPUT_SOURCE_EXPR1_VALUE = 3;
const uint8_t CV_OUTPUT_SOURCE_EXPR2_VALUE = 4;
const uint8_t CV_OUTPUT_SOURCE_ENV_VALUE = 5;
const uint8_t CV_OUTPUT_SOURCE_RAMP1_VALUE = 6;
const uint8_t CV_OUTPUT_SOURCE_NOISE1_VALUE = 7;
const uint8_t CV_OUTPUT_SOURCE_LFO2_VALUE = 8;
const uint8_t CV_OUTPUT_SOURCE_RAMP2_VALUE = 9;
const uint8_t CV_OUTPUT_SOURCE_NOISE2_VALUE = 10;
const uint8_t CV_OUTPUT_SOURCE_BUS1_VALUE = 11;
const uint8_t CV_OUTPUT_SOURCE_BUS2_VALUE = 12;
const uint8_t CV_OUTPUT_SOURCE_BUS3_VALUE = 13;
const uint8_t CV_OUTPUT_SOURCE_BUS4_VALUE = 14;
const uint8_t CV_OUTPUT_SOURCE_AWG1_VALUE = 15;
const uint8_t CV_OUTPUT_SOURCE_AWG2_VALUE = 16;

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
const uint8_t TRIGGER_CONTROL_FSW_4_VALUE = 5;
const uint8_t TRIGGER_CONTROL_FSW_5_VALUE = 6;
const uint8_t TRIGGER_CONTROL_FSW_6_VALUE = 7;

const uint8_t TRIGGER_MODE_LEVEL_NON_INV_VALUE = 0;
const uint8_t TRIGGER_MODE_LEVEL_INV_VALUE = 1;
const uint8_t TRIGGER_MODE_EDGE_OFF_ON_VALUE = 2;
const uint8_t TRIGGER_MODE_EDGE_ON_OFF_VALUE = 3;

const uint8_t RAMP_DIRECTION_UP_VALUE = 0;
const uint8_t RAMP_DIRECTION_DOWN_VALUE = 1;

const uint8_t BUS_SOURCE_NONE_VALUE = 0;
const uint8_t BUS_SOURCE_LFO1_VALUE = 1;
const uint8_t BUS_SOURCE_LFO2_VALUE = 2;
const uint8_t BUS_SOURCE_EXPR1_VALUE = 3;
const uint8_t BUS_SOURCE_EXPR2_VALUE = 4;
const uint8_t BUS_SOURCE_ENV_VALUE = 5;
const uint8_t BUS_SOURCE_NOISE1_VALUE = 6;
const uint8_t BUS_SOURCE_NOISE2_VALUE = 7;
const uint8_t BUS_SOURCE_RAMP1_VALUE = 8;
const uint8_t BUS_SOURCE_RAMP2_VALUE = 9;
const uint8_t BUS_SOURCE_AWG1_VALUE = 10;
const uint8_t BUS_SOURCE_AWG2_VALUE = 11;

const uint8_t SOURCE_VALUE_PROVIDER_SOURCE_NONE_VALUE = 0;
const uint8_t SOURCE_VALUE_PROVIDER_SOURCE_LFO1_VALUE = 1;
const uint8_t SOURCE_VALUE_PROVIDER_SOURCE_LFO2_VALUE = 2;
const uint8_t SOURCE_VALUE_PROVIDER_SOURCE_EXPR1_VALUE = 3;
const uint8_t SOURCE_VALUE_PROVIDER_SOURCE_EXPR2_VALUE = 4;
const uint8_t SOURCE_VALUE_PROVIDER_SOURCE_ENV_VALUE = 5;
const uint8_t SOURCE_VALUE_PROVIDER_SOURCE_NOISE1_VALUE = 6;
const uint8_t SOURCE_VALUE_PROVIDER_SOURCE_NOISE2_VALUE = 7;
const uint8_t SOURCE_VALUE_PROVIDER_SOURCE_RAMP1_VALUE = 8;
const uint8_t SOURCE_VALUE_PROVIDER_SOURCE_RAMP2_VALUE = 9;
const uint8_t SOURCE_VALUE_PROVIDER_SOURCE_BUS1_VALUE = 10;
const uint8_t SOURCE_VALUE_PROVIDER_SOURCE_BUS2_VALUE = 11;
const uint8_t SOURCE_VALUE_PROVIDER_SOURCE_BUS3_VALUE = 12;
const uint8_t SOURCE_VALUE_PROVIDER_SOURCE_BUS4_VALUE = 13;
const uint8_t SOURCE_VALUE_PROVIDER_SOURCE_AWG1_VALUE = 14;
const uint8_t SOURCE_VALUE_PROVIDER_SOURCE_AWG2_VALUE = 15;

// values for PATCH_CONTROL_CC
const uint8_t PATCH_REFRESH = 1;
const uint8_t PATCH_EEPROM_DUMP = 2;
const uint8_t PATCH_DISPLAY_NUMBER = 3;
const uint8_t PATCH_COPY = 4;
const uint8_t PATCH_SAVE = 5;
const uint8_t PATCH_TRANSMIT_DISABLE = 6;
const uint8_t PATCH_TRANSMIT_ENABLE = 7;
const uint8_t PATCH_DUMP_PATCH = 8;
// erase values deliberately away from other to reduce risk of accidents
const uint8_t PATCH_ERASE_ALL = 0x7f;
const uint8_t PATCH_ERASE_CURRENT = 0x7e;

#endif
