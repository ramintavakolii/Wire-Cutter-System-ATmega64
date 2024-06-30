#include <mega64.h>
#include <delay.h>
#include <interrupt.h>

/* Defines macros and global variables */

// Define ports for keypad
#define ROW1 PINE.0
#define ROW2 PINE.1
#define ROW3 PINE.2
#define ROW4 PINE.3
#define COL1 PINE.4
#define COL2 PINE.5
#define COL3 PINE.6
#define keypadPort PORTE
#define keypadPin PINE

// Define the output port for cutter activation
#define CUTTER PORTC.0

// Variables
unsigned char key;
unsigned char num_wires = 0;
unsigned char Number_Wires = 0;
unsigned char wire_length = 0;
unsigned int pulse_count = 0;
unsigned char display_value = 0;
unsigned char I1, I2 = 0;
unsigned char temp = 0;

// Define keypad matrix
const unsigned char keypad[4][3] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};

// Bit patterns for digits 0-9 on a common-cathode 7-segment display
const unsigned char segment_map[10] = {
    0b11111100, // 0
    0b01100000, // 1
    0b11011010, // 2
    0b11110010, // 3
    0b01100110, // 4
    0b10110110, // 5
    0b10111110, // 6
    0b11100000, // 7
    0b11111110, // 8
    0b11110110  // 9
};

/* Function declarations */

void display_number(unsigned char number);
void setup();
unsigned char read_keypad();
void process_pulse();
void external_interrupt_setup();

/* Function definitions */

void display_number(unsigned char number) {
    // Map number to segments
    unsigned char tens = number / 10;
    unsigned char units = number % 10;

    // Display the tens place digit on PORTA
    PORTA = segment_map[tens];

    // Display the units place digit on PORTB
    PORTB = segment_map[units];
}

void setup() {
    // Set ports for 7-segment display as output
    DDRA = 0xFF;
    DDRB = 0xFF;

    DDRE = 0x70;  // Columns (PE4-PE6) as output, Rows (PE0-PE3) as input
    PORTE = 0x0F; // Enable pull-ups for rows

    // Set cutter output port as output
    DDRC = 0x01;
    display_number(0);

    // Setup external interrupt
    external_interrupt_setup();
}

// Function to read the keypad
unsigned char read_keypad() {
    unsigned char col, row;

    for (col = 0; col < 3; col++) {
        // Set the current column to low
        PORTE = ~(0x10 << col) & 0x70 | 0x0F;

        // Check each row
        for (row = 0; row < 4; row++) {
            if (!(PINE & (1 << row))) {
                // Debounce: wait for stable press
                //delay_ms(20);
                if (!(PINE & (1 << row))) {
                    // Wait until the key is released
                    while (!(PINE & (1 << row)));

                    // Return the key value
                    return keypad[row][col];
                }
            }
        }
    }

    return 0; // Return 0 if no key is pressed
}

void process_pulse() {
    pulse_count++;
    if (pulse_count >= 4) {
        pulse_count = 0;
        display_value++;
        display_number(display_value);

        if (display_value >= wire_length) {
            CUTTER = !CUTTER; // Toggle cutter state
        }
    }
}

void external_interrupt_setup() {
    // Configure INT0 (PD2) to trigger on rising edge
    EICRA |= (1 << ISC01) | (1 << ISC00);
    EIMSK |= (1 << INT0);
    sei(); // Enable global interrupts
}

// External interrupt 0 service routine
interrupt [EXT_INT0] void ext_int0_isr(void) {
    if (temp == 1) { 
        process_pulse();  
    }
}

void main() {
    setup();
    while (1) {
        key = read_keypad();
        if (key == '*') {
            // Read number of wires
            do {
                I1 = read_keypad();
            } while (!I1); 
                                   
            do {
                I2 = read_keypad();
            } while (!I2);  
            
            delay_ms(20);
            
            num_wires = (I1 - '0') * 10 + (I2 - '0');
            display_number(num_wires);
            I1 = 0;
            I2 = 0;
        } else if (key == '#') {               
            // Read wire length
            do {
                I1 = read_keypad();
            } while (!I1); 
                                   
            do {
                I2 = read_keypad();
            } while (!I2);  
            
            delay_ms(20);
            
            wire_length = (I1 - '0') * 10 + (I2 - '0');
            display_number(wire_length);
            delay_ms(100);
            display_value = 0;
            display_number(display_value); 
            
            // Start the cutter system
            display_value = 0;
            pulse_count = 0;
            Number_Wires = 0;
            while (Number_Wires < num_wires) {
                temp = 1;
                while (display_value < wire_length) {
                    // Wait for the wire to reach the required length
                }
                temp = 0;
                delay_ms(150);
                display_value = 0;
                display_number(display_value);                
                Number_Wires++;
                CUTTER = !CUTTER;
                pulse_count = 0;
            }
            Number_Wires = 0;
            display_number(0);  
        }
    }
}
