#ifndef TASK1_RANDOM_H
#define TASK1_RANDOM_H
#include <Task1_sensing.h>
//this unit creates and stores the random sequence to be used in game
class simon_sequence {//
	cmdEnum_push_button::cmdEnum sequence[100];//stores the sequence
	int buttons_used;
	bool init_flag;
	void generateSequence() {//populates the sequence 
		unsigned long int seed = millis();
		srand(seed);//set seed to be ms since board started running
		for (int i = 0; i < 100; i++) {//populate enum array
			sequence[i] = lables[rand() % buttons_used];//range [1, buttons_used]
		}
	}
public:
	simon_sequence() {}//basic constructor
	cmdEnum_push_button::cmdEnum* getSequence() {//return pointer to the sequence
		if (init_flag) {
			return(sequence);
		}
	}
	void setButtonsUsed(int numb_of_buttons) {
		buttons_used = numb_of_buttons;
		generateSequence();
		init_flag = true;
	}
	~simon_sequence() {}
	
};

//test

void testRandom() {//test with 4 buttons used
	simon_sequence test, test2;
	test.setButtonsUsed(4);
	cmdEnum_push_button::cmdEnum* sample_sequence = test.getSequence();
	for (int i = 0; i < 10; i++) {//print sequence
		switch (*(sample_sequence+i)) {
		case lables[0]:Serial.println("button 1"); break;
		case lables[1]:Serial.println("button 2"); break;
		case lables[2]:Serial.println("button 3"); break;
		case lables[3]:Serial.println("button 4"); break;
		case lables[4]:Serial.println("button 5"); break;
		}
	}
	Serial.println("");
	test2.setButtonsUsed(3);
	sample_sequence = test2.getSequence();
	for (int i = 0; i < 10; i++) {//print sequence
		switch (*(sample_sequence + i)) {
		case lables[0]:Serial.println("button 1"); break;
		case lables[1]:Serial.println("button 2"); break;
		case lables[2]:Serial.println("button 3"); break;
		case lables[3]:Serial.println("button 4"); break;
		case lables[4]:Serial.println("button 5"); break;
		}
	}
}
#endif