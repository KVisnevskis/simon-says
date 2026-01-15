#ifndef TASK1_ACTION_H
#define TASK1_ACTION_H
#include <Task1_sensing.h>
#include <Beginner_LED.h> //imcluded to visualise buzzer output
class buzzer_unit {
protected:
	bool init_flag;		//initialisation flag
	int buzzer_pin;		//pin for buzzer output
	const static int tones[5] = {392, 523, 660, 784, 100};	//tones for playing cues/feedback. not changeable by user.
	dig_LED led[4];		//only used to visualize tones
public:
	buzzer_unit() {		//basic constructor
		init_flag = false;
	}
	buzzer_unit(int pin_number) {	//constructor with pin number
		buzzer_pin = pin_number;	//set buzzer pin
		init_flag = true;	//set initialised
		led[0].setup_LED(9);	//just for visualisation
		led[1].setup_LED(10);
		led[2].setup_LED(11);
		led[3].setup_LED(12);
	}
	void setUpBuzzer(int pin_number) {//set up function
		if (!init_flag) {
			buzzer_pin = pin_number;	//set buzzer pin
			init_flag = true;	//set initialised
			led[0].setup_LED(9);	//just for visualisation
			led[1].setup_LED(10);
			led[2].setup_LED(11);
			led[3].setup_LED(12);
		}
	}
	bool isInitialised() { return init_flag; }
	//next section is all the different tone sequences, they are set to specific lengths, not intended to be changable by user
	void countdown() { //3 med tones followed by high tone signify start of game, large delay between
		if (init_flag) {//chek if initialised
			tone(buzzer_pin, 559, 500); delay(800);
			tone(buzzer_pin, 559, 500); delay(800);
			tone(buzzer_pin, 559, 500); delay(800);
			tone(buzzer_pin, 745, 500); delay(800);
		} 
		else { Serial.println("Buzzer not initialised"); }
	}
	void gameWin() { //sequence to be played when game is won rising melody
		if (init_flag) {//check if initialised
			tone(buzzer_pin, 392, 1000); delay(500);
			tone(buzzer_pin, 440, 1000); delay(500);
			tone(buzzer_pin, 523, 1000); delay(500);
			tone(buzzer_pin, 659, 1000); delay(1500);
		}
		else{ Serial.println("Buzzer not initialised"); }
		
	}
	void gameLoss() { //falling melody
		if (init_flag) {
			noTone(buzzer_pin);//added to stop feedback from playing
			tone(buzzer_pin, 659, 1000); delay(500);
			tone(buzzer_pin, 523, 1000); delay(500);
			tone(buzzer_pin, 440, 1000); delay(500);
			tone(buzzer_pin, 392, 1000); delay(1500);
		}
		else{ Serial.println("Buzzer not initialised"); }
	}
	void matchWin() { //"We are the champions" by queen
		if (init_flag) {
			noTone(buzzer_pin);
			tone(buzzer_pin, 587, 1500); delay(1500);
			tone(buzzer_pin, 554, 1000); delay(1000);
			tone(buzzer_pin, 587, 500); delay(500);
			tone(buzzer_pin, 554, 1500); delay(1500);
			tone(buzzer_pin, 494, 2000); delay(2000);
		}
		else { Serial.println("Buzzer not initialised"); }	
	}
	void matchLoss() { //ominous/sad melody
		if (init_flag) {
			tone(buzzer_pin, 349, 1000); delay(1000);
			tone(buzzer_pin, 349, 1000); delay(1000);
			tone(buzzer_pin, 349, 1000); delay(1000);
			tone(buzzer_pin, 277, 2000); delay(2000);
		}
		else{ Serial.println("Buzzer not initialised"); }		
	}
	void warning() { //Three high tones
		if (init_flag) {
			tone(buzzer_pin, 698, 1000); delay(1000);
			tone(buzzer_pin, 698, 1000); delay(1000);
			tone(buzzer_pin, 698, 1000); delay(1000);
		}else{ Serial.println("Buzzer not initialised"); }
	}
	void playCue(cmdEnum_push_button::cmdEnum button, bool isFeedback, int cue_length) { //same function used to play cue and feedback
		
		if (init_flag) {
			int tone_length = cue_length, time_delay = cue_length * 1.2;
			if (isFeedback) {//if a feedback is being played, reduce the tone length
				tone_length = cue_length / 2;
				time_delay = 0;//when feedback is being provided, dont wait for one feedback to end before starting next
			}
			switch (button) {
			case lables[0]:
				noTone(buzzer_pin); delay(50);//make sure there is gap between feedbacks
				tone(buzzer_pin, tones[0], tone_length);
				led[0].switch_on();
				delay(time_delay);
				led[0].switch_off();
				break;
			case lables[1]:
				noTone(buzzer_pin); delay(50);//make sure there is gap between feedbacks
				tone(buzzer_pin, tones[1], tone_length);
				led[1].switch_on();
				delay(time_delay);
				led[1].switch_off();
				break;
			case lables[2]:
				noTone(buzzer_pin);
				delay(50);//make sure there is gap between feedbacks
				tone(buzzer_pin, tones[2], tone_length);
				led[2].switch_on();
				delay(time_delay);
				led[2].switch_off();
				break;
			case lables[3]:
				noTone(buzzer_pin); delay(50);//make sure there is gap between feedbacks
				tone(buzzer_pin, tones[3], tone_length);
				led[3].switch_on();
				delay(time_delay);
				led[3].switch_off();
				break;
			case lables[4]:
				noTone(buzzer_pin); delay(50);//make sure there is gap between feedbacks
				tone(buzzer_pin, tones[4], tone_length);
				delay(time_delay);
				break;
			}
		}
		else { Serial.println("Buzzer not initialised"); }
		//the one big problem with the play_cue() function is that when you get a sequence of same tones, and you
		//press the buttons quickly enough, the feedbacks overlap, so you dont get a sense of how many times you
		//have pressed the same button
		
	}
	void playCueSequence(cmdEnum_push_button::cmdEnum* sequence, int sequence_length, int cue_length) {//plays an array containing sequence
		if (init_flag) {
			for (int i = 0; i < sequence_length; i++) {//go through array that holds sequence and play all the cues
				playCue(sequence[i], false, cue_length);
			}
		}
	}

};

void testActionModule() {
	buzzer_unit test(5);//initialise buzzer object on pin 5
	//play all pre-set frequencies
	test.countdown();
	test.gameLoss();
	test.gameWin();
	test.matchLoss();
	test.matchWin();
	test.warning();
	//test the play_cue function
	for (int i = 0; i < 5; i++) {
		test.playCue(lables[i], false, 1000);//test cues
		test.playCue(lables[i], true, 1000);//test feedback
		delay(700);//need to add this delay because feedback tones have no delay after them as defined
	}
	//test play_cue_sequence
	test.playCueSequence(lables, 5, 1000);
}

#endif
