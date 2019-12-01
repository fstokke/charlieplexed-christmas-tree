#include <Arduino.h>
#include <math.h>

const uint8_t NUM_ROWS = 5;
const uint8_t NUM_LEDS = 20;
const uint8_t MAX_BRIGHTNESS = 32;
volatile uint8_t led_brightness[NUM_LEDS];


void set_led_brightness(uint8_t led_idx, uint8_t brightness) {
  led_brightness[led_idx] = brightness;
}

void set_all_brightness(uint8_t brightness) {
  memset((void*)led_brightness, brightness, NUM_LEDS);
}

struct LedSparkleState {
    uint8_t led_idx;
    uint8_t peak_brightness;
    uint8_t brightess;
    int8_t brightness_incremenet;
    uint8_t brightness_step_delay;
    uint8_t delay_counter;
};


const uint8_t NUM_SPARKLING_LEDS = 6;
const uint8_t BASE_SPARKLING_BRIGHTNESS = 6;
LedSparkleState sparkle_state_table[NUM_SPARKLING_LEDS];

uint8_t pick_random_free_led() {
    int8_t led_idx = -1;
    while (led_idx == -1) {
        led_idx = random(NUM_LEDS);
        for (uint8_t i = 0; i < NUM_SPARKLING_LEDS; ++i) {
            if (led_idx == sparkle_state_table[i].led_idx) {
                led_idx = -1;
                break;
            }
        }
    }
    return led_idx;
}

void sparkling_light() {
    memset(sparkle_state_table, 0, sizeof(sparkle_state_table));

    set_all_brightness(BASE_SPARKLING_BRIGHTNESS);
    
    for (uint16_t j = 0; j < 40000; ++j) {

        for (uint8_t i = 0; i < NUM_SPARKLING_LEDS; ++i) {

            LedSparkleState* sparkle_state = &sparkle_state_table[i];

            if (sparkle_state->brightess <= BASE_SPARKLING_BRIGHTNESS) {
                sparkle_state->led_idx = pick_random_free_led();
                sparkle_state->peak_brightness = random(BASE_SPARKLING_BRIGHTNESS + 1, MAX_BRIGHTNESS + 1);
                sparkle_state->brightess = BASE_SPARKLING_BRIGHTNESS;
                sparkle_state->brightness_incremenet = 1;
                sparkle_state->brightness_step_delay = random(5, 10);
                //sparkle_state->brightness_step_delay = 2;
                sparkle_state->delay_counter = 0;
            }

            if (sparkle_state->delay_counter == 0) {
                sparkle_state->brightess += sparkle_state->brightness_incremenet;
                sparkle_state->delay_counter = sparkle_state->brightness_step_delay;

                if (sparkle_state->brightess == MAX_BRIGHTNESS) {
                    sparkle_state->brightness_incremenet = -sparkle_state->brightness_incremenet;
                }
            }
            --sparkle_state->delay_counter;

            set_led_brightness(sparkle_state->led_idx, sparkle_state->brightess);
        }
        delay(5);
    }
}

void pulse_all() {
  for (uint8_t j = 0; j < 3; ++j) {
    for (uint8_t i=1; i < MAX_BRIGHTNESS; ++i) {
      set_all_brightness(i);
      delay(50);
    }
    for (uint8_t i=MAX_BRIGHTNESS; i > 0; --i) {
      set_all_brightness(i);
      delay(50);
    }
  }
}

void tree_walk(uint8_t base_brightness, uint8_t brightness) {
  
  for (uint8_t j=0; j < 3; ++j) {
    for (uint8_t i=0; i < NUM_LEDS; ++i) {
      set_all_brightness(base_brightness);
      set_led_brightness(i, brightness);
      delay(250);
    }
  }
}

void side_walk(uint16_t pause) {
  for (uint8_t j=0; j < 3; ++j) {
    for (uint8_t i=0; i < 4; ++i) {
      set_all_brightness(0);
      set_led_brightness(i*5, MAX_BRIGHTNESS);
      set_led_brightness(i*5+1, MAX_BRIGHTNESS);
      set_led_brightness(i*5+2, MAX_BRIGHTNESS);
      set_led_brightness(i*5+3, MAX_BRIGHTNESS);
      set_led_brightness(i*5+4, MAX_BRIGHTNESS);
      delay(pause);
    }
  }
}

void line_walk() {
  
  for (uint8_t j=0; j < 3; ++j) {
    for (uint8_t i=0; i < 5; ++i) {
      set_all_brightness(0);
      set_led_brightness(i, MAX_BRIGHTNESS);
      set_led_brightness(i+5, MAX_BRIGHTNESS);
      set_led_brightness(i+10, MAX_BRIGHTNESS);
      set_led_brightness(i+15, MAX_BRIGHTNESS);
      delay(250);
    }
    for (uint8_t i=3; i > 0; --i) {
      set_all_brightness(0);
      set_led_brightness(i, MAX_BRIGHTNESS);
      set_led_brightness(i+5, MAX_BRIGHTNESS);
      set_led_brightness(i+10, MAX_BRIGHTNESS);
      set_led_brightness(i+15, MAX_BRIGHTNESS);
      delay(250);
    }
  }
}


void running_light() {
  for (uint8_t i=0; i < NUM_LEDS; ++i) {
    led_brightness[i] = (i % 3) == 0 ? MAX_BRIGHTNESS : 0 ;
  }
  for(uint8_t j = 0; j < 50; ++j) {
    uint8_t first_led_brightness = led_brightness[0];
    memmove((void*)led_brightness, (void*)(led_brightness+1), NUM_LEDS-1);
    led_brightness[NUM_LEDS-1] = first_led_brightness;
    delay(200);
  }  
}

const uint8_t WAVE_TABLE_LEN = 100;
const float PERIOD_S = 1.2f; // Wave period in seconds
const uint8_t MIN_WAVE_BRIGHTNESS = 2;
const uint8_t MAX_WAVE_BRIGHTNESS = MAX_BRIGHTNESS;

const uint8_t VISIBLE_WAVE_SEGMENT = WAVE_TABLE_LEN / 2; // One half of a sine wave
const uint8_t wave_segment_step = VISIBLE_WAVE_SEGMENT / 5; // Tree is 5 LEDs high.

uint8_t wave_table[WAVE_TABLE_LEN];
const uint16_t wave_step_delay_millis = (uint16_t)((float)PERIOD_S * 1000 / WAVE_TABLE_LEN);

// Initialize wave table with sine wave.
void init_wave_table() {

    const uint8_t wave_amplitude = (MAX_WAVE_BRIGHTNESS - MIN_WAVE_BRIGHTNESS) / 2;
    const uint8_t wave_offset = MIN_WAVE_BRIGHTNESS + wave_amplitude;

    for (uint8_t i = 0; i < WAVE_TABLE_LEN; ++i) {
        float t = (2.0f * (float)M_PI * i) / WAVE_TABLE_LEN;
        wave_table[i] = (uint8_t) round((sin(t) * wave_amplitude) + wave_offset);
    }
}

void falling_wave() {
    for (uint8_t k = 0; k < 10; ++k) {
        for (uint8_t i = 0; i < WAVE_TABLE_LEN; ++i) {
            uint8_t idx_0 = i;
            uint8_t idx_1 = (i + wave_segment_step) % WAVE_TABLE_LEN;
            uint8_t idx_2 = (i + wave_segment_step * 2) % WAVE_TABLE_LEN;
            uint8_t idx_3 = (i + wave_segment_step * 3) % WAVE_TABLE_LEN;
            uint8_t idx_4 = (i + wave_segment_step * 4) % WAVE_TABLE_LEN;

            for (uint8_t j = 0; j < 4; ++j) {
                set_led_brightness(j * 5, wave_table[idx_0]);
                set_led_brightness(j * 5 + 1, wave_table[idx_1]);
                set_led_brightness(j * 5 + 2, wave_table[idx_2]);
                set_led_brightness(j * 5 + 3, wave_table[idx_3]);
                set_led_brightness(j * 5 + 4, wave_table[idx_4]);
            }
            
            delay(wave_step_delay_millis);
        }
    }
}


const uint8_t t1_comp = 124;

// 5 rows * 100 Hz PWM refresh rate * 32 brightness values
// gives 16kHz update freq. 
//
// Initialize Timer1 interrupt with 16kHZ interrupt
// frequency
void setup_interrupts() {
  // GTCCR – General Timer/Counter1 Control Register
  GTCCR = 0; // NO PWM

  // Set up interrupt frequencey.
  //
  // CTC1 : Clear Timer/Counter on Compare Match
  // CS11=1 && CS10=1: Prescaler CK/4
  // This together with ORC1C = 124 (0-124 values)
  // gives interrupt freq: 8MHz/(4*125) = 16kHZ
  TCCR1 = (1 << CTC1) | (1 << CS11) | (1 << CS10); 

  // TCNT1 – Timer/Counter1. This 8-bit register contains the value of Timer/Counter1.
  TCNT1 = 0; // Start at zero.

  // OCR1A – Timer/Counter1 Output Compare Register A
  // TIM1_COMPA_vect interrupt will trigger when Counter 1 reaches t1_comp.
  OCR1A = t1_comp; 

  // OCR1C – Timer/Counter1 Output Compare Register C
  // Setting CTC bit = 1 means that counter will reset to zero once the counter
  // reaches ORC1C.
  OCR1C = t1_comp;

  // TIMSK – Timer/Counter Interrupt Mask Register
  // OCIE1A: Timer/Counter1 Output Compare Interrupt Enable
  // Don't touch other bits, delay() uses TIMER 0
  TIMSK |= (1 << OCIE1A); 
   
  sei(); // Enable interrupts (not really necessary, enabled by default)
}

const uint8_t LINE_A_BIT = 0x01;
const uint8_t LINE_B_BIT = 0x02;
const uint8_t LINE_C_BIT = 0x04;
const uint8_t LINE_D_BIT = 0x10;
const uint8_t LINE_E_BIT = 0x08;

uint8_t i_ramp = MAX_BRIGHTNESS - 1;
uint8_t i_row_idx = NUM_ROWS - 1;
uint8_t i_led0_brightess = 0;
uint8_t i_led1_brightess = 0; 
uint8_t i_led2_brightess = 0; 
uint8_t i_led3_brightess = 0; 
uint8_t i_col_enable_mask = 0; 
uint8_t i_row_outputs = 0;
uint8_t i_row_mask = 0;
uint8_t line_to_output_map[] = {LINE_A_BIT, LINE_B_BIT, LINE_C_BIT, LINE_D_BIT, LINE_E_BIT};

ISR(TIM1_COMPA_vect) {
  
  ++i_ramp;
  if (i_ramp >= MAX_BRIGHTNESS) {
    i_ramp = 0;
    ++i_row_idx;

    if (i_row_idx >= NUM_ROWS) {
      i_row_idx = 0;
    }

    uint8_t first_led_in_row_idx = i_row_idx << 2;
    i_row_mask = (1 << i_row_idx) - 1;

    i_led0_brightess = led_brightness[first_led_in_row_idx];
    i_led1_brightess = led_brightness[first_led_in_row_idx+1];
    i_led2_brightess = led_brightness[first_led_in_row_idx+2];
    i_led3_brightess = led_brightness[first_led_in_row_idx+3];

    i_col_enable_mask = 0x0f; 

    i_row_outputs = line_to_output_map[i_row_idx];
  }

  if (i_ramp == i_led0_brightess)
    i_col_enable_mask &= ~0x01;
  if (i_ramp == i_led1_brightess)
    i_col_enable_mask &= ~0x02;
  if (i_ramp == i_led2_brightess)
    i_col_enable_mask &= ~0x04;
  if (i_ramp == i_led3_brightess)
    i_col_enable_mask &= ~0x08;
  
  uint8_t column_line_outputs = (i_col_enable_mask & i_row_mask) | (i_col_enable_mask & ~i_row_mask) << 1;

  // Keep bits 0-2, swap bits 3 and 4 (PB3 and PB4 are swapped on board to make routing easier)
  uint8_t column_outputs = (column_line_outputs & 0x07) | ((column_line_outputs & 0x10) >> 1) | ((column_line_outputs & 0x08) << 1);
  uint8_t output_pins = column_outputs != 0 ? column_outputs | i_row_outputs : 0;

  DDRB = output_pins;
  PORTB = output_pins != 0 ? i_row_outputs : 0;
}


void setup() {
  DDRB = 0;
  PORTB = 0;
  setup_interrupts();  

  set_all_brightness(0);
  randomSeed(42);
  init_wave_table();
}


void loop() {
  // side_walk(2500);
  falling_wave();
  sparkling_light();

  // pulse_all();
  // tree_walk(0, MAX_BRIGHTNESS);
  // tree_walk(MAX_BRIGHTNESS, 0);
  // line_walk();
  // running_light();
}