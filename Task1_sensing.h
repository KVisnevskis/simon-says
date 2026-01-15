#ifndef TASK1_SENSING_H
#define TASK1_SENSING_H

#include "CmdEnumPushButton.h"

constexpr cmdEnum_push_button::cmdEnum lables[5] = { cmdEnum_push_button::button1, cmdEnum_push_button::button2,
cmdEnum_push_button::button3, cmdEnum_push_button::button4, cmdEnum_push_button::stop };
//this class implements the sensing unit of the system

class button_input {
protected:
	const static int MAX_SIZE = 5;	//max size is 5 input devices
	cmdEnum_push_button inputs[MAX_SIZE];	//array of digital input objects
	cmdEnum_push_button::cmdEnum player_input;	//stores the label of last recorded button press
	int buttons_used;
	bool init_flag;		//initialisation flag
public:
	button_input() { init_flag = false; }	//basic constructor
	button_input(int* pointer_to_pin_array, unsigned int longpush, int input_size) {	//constructor with arguments
		for (int i = 0; i < input_size; i++) {	//go through array of pushbutton objects
			inputs[i].assign_pin_command(*(pointer_to_pin_array + i), lables[i], longpush);//assigns pin numbers and labels to buttons
		}
		buttons_used = input_size;//number of buttons used
		init_flag = true;	//set initialisation flag to true
	}
	void setUpInputs(int* pointer_to_pin_array, unsigned int longpush, int input_size) {//set up inputs with pin array and longpush length
		if (!init_flag) {
			for (int i = 0; i < input_size; i++) {
				inputs[i].assign_pin_command(*(pointer_to_pin_array + i), lables[i], longpush);//assigns pin numbers and labels to buttons
			}
			buttons_used = input_size;//number of buttons used
			init_flag = true;
		}
	}
	bool isInitialised() { return init_flag; }
	bool isButtonPressed() {	//returns true if any button has been pressed
		if (init_flag) {	//if sensing unit initialised
			for (int i = 0; i < buttons_used; i++) {	//read all pins
				if (inputs[i].check_enabled()) {	//check if pin is initialised
					if (inputs[i].check_n_get_new_input(player_input)) {	//check if button pressed, assign button label to player_input
						return true;	//exit loop if button press detected
					}
				}
			}
		}
		return false;//if no press detected, return false;
	}
	cmdEnum_push_button::cmdEnum getPlayerInput() {//returns last user input
		if (init_flag) {
			return player_input;
		}
		
	}
	int getButtonsUsed() {
		if (init_flag) {
			return buttons_used;
		}
	}
	void waitForInput() {//function to wait for user input before proceeding
		if (init_flag) {
			while (!isButtonPressed()) {//stop waiting when any button pressed
			//waiting for user confirmation to proceed
			}
		}
	}
};

//Testing functions

void testSensingUnit() {
	int pins[5] = { 2, 4, 7, 8 , 13};//pins to be used as inputs
	button_input test(pins, 150, 5);//initialise with pins in pins[], longpush 150ms, pins used 5
	if(test.isInitialised()){//if sensing unit initialised
		if (test.isButtonPressed()) {//if button press detected
			Serial.println("buttons used: " + (String)(test.getButtonsUsed()));//print number of buttons used
			switch (test.getPlayerInput()) {//get which button was pressed
			case lables[0]:Serial.println("button 1 pressed"); break;//print which button was pressed
			case lables[1]:Serial.println("button 2 pressed"); break;
			case lables[2]:Serial.println("button 3 pressed"); break;
			case lables[3]:Serial.println("button 4 pressed"); break;
			case lables[4]:Serial.println("button 5 pressed"); break;
			default: Serial.println("no input"); break;
			}//note that i have only 4 buttons, so to test if all 5 work i need to connect one of the buttons to pin 13 here.
			test.waitForInput();//wait for user confirmation before recording next input
		}	
	}
}
void testSensingUnit3Pins() {//same test but initialised using only 3 pins
	int pins[5] = { 2, 4, 7, 8, 13};//pins to be used as inputs
	button_input test(pins, 150, 3);//initialise with pins in pins[], longpush 150ms
	if (test.isInitialised()) {//if sensing unit initialised
		if (test.isButtonPressed()) {//if button press detected
			Serial.println("buttons used: " + (String)(test.getButtonsUsed()));//print number of buttons used
			switch (test.getPlayerInput()) {//get which button was pressed
			case lables[0]:Serial.println("button 1 pressed"); break;//print which button was pressed
			case lables[1]:Serial.println("button 2 pressed"); break;
			case lables[2]:Serial.println("button 3 pressed"); break;
			case lables[3]:Serial.println("button 4 pressed"); break;//shouldnt happen here, no button 5 initialised
			case lables[4]:Serial.println("button 5 pressed"); break;//shouldnt happen here, no button 5 initialised
			default: Serial.println("no input"); break;
			}//note that i have only 4 buttons, so to test if all 5 work i need to connect one of the buttons to pin 13 here.
			test.waitForInput();//wait for user confirmation before recording next input
		}
	}
}
#endif