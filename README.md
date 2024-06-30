# Wire Cutter System Using ATmega64 Microcontroller

This repository contains the implementation of a wire cutter system using the ATmega64 microcontroller. The system allows an operator to specify the number and length of wires to be cut using a keypad. The system uses a 7-segment display to show the current length of the wire and toggles an output to activate the cutter when the desired length is reached. The entire system is simulated using Proteus.

## Directory Structure

- **Final_Project_Report.pdf**: A detailed report of the project including the design, components used, and the functionality.
- **proteus file**: The Proteus simulation file for the project.
- **project.c**: The C code for the project.

## Components Used

1. **Keypad (3x4 Matrix Keypad)**
    - A 3x4 matrix keypad is used for user input to enter the number and length of wires.

2. **Common Cathode 7-Segment Display**
    - Two 7-segment displays are used to show the current length of the wire being processed.

3. **LED for 1-Bit Output**
    - An LED is used to indicate the state of the cutter system, toggling its state to signal the cutter activation.

4. **Pulse Generator**
    - A pulse generator provides an input signal to the microcontroller, where each pulse represents 25 cm of wire processed.

## System Operation

1. **Initialization**: The microcontroller initializes the ports for the keypad, 7-segment displays, and the cutter output. External interrupts are set up to count pulses from the pulse generator.

2. **Input via Keypad**: The operator enters a 2-digit number to specify the number of wires using the keypad. After pressing the '*' key, the operator enters another 2-digit number to specify the length of each wire in meters. Pressing the '#' key starts the cutting process.

3. **Pulse Counting**: The system counts pulses from the pulse generator, with each pulse representing 25 cm. For every four pulses, the displayed length increases by 1 meter.

4. **Display Update**: The current length of the wire being processed is displayed on the 7-segment displays.

5. **Cutter Activation**: When the current length reaches the specified length, the microcontroller toggles the output pin connected to the LED, signaling the cutter to activate. This process repeats for the specified number of wires.

## Code Overview

The code is written in C using CodeVisionAVR and includes the following key functions:
- `display_number(unsigned char number)`: Updates the 7-segment displays with the given number.
- `setup()`: Initializes the microcontroller ports and sets up external interrupts.
- `read_keypad()`: Reads key presses from the keypad and debounces the input.
- `process_pulse()`: Increments the pulse count and updates the display value.
- `external_interrupt_setup()`: Configures the external interrupt for pulse input.
- `ext_int0_isr()`: Interrupt service routine for handling external interrupt 0.
- `main()`: Main loop that handles user input and controls the wire cutting process.

## Getting Started

### Prerequisites

- CodeVisionAVR or a similar C compiler for AVR microcontrollers.
- Proteus software for simulation.
- Basic knowledge of microcontrollers and C programming.

### Running the Simulation

1. Open the Proteus file in Proteus software.
2. Load the `project.c` code onto the ATmega64 microcontroller in the simulation.
3. Run the simulation to see the wire cutter system in action.

## Future Improvements

- Adding error handling for invalid keypad entries.
- Implementing a more sophisticated user interface.
- Adding more precise measurement capabilities.
- Integrating safety features to prevent incorrect operation.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Special thanks to the contributors and developers of the tools and libraries used in this project.
