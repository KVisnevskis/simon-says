#ifndef TASK1_SYSTEM_H
#define TASK1_SYSTEM_H

#include <Task1_sensing.h>
#include <Task1_action.h>
#include <Task1_control.h>
#include <Task1_random.h>
#include <IntervalCheckTimer.h>

class simon {
protected:
	//variables
	const int default_difficulty = 3, default_length = 5;//default values
	int sequence_length;//sequence length
	int cue_length;//cue length. length of buzz
	bool result;//used to store if result has been reached
	bool time_elapsed;//flag for if time has elapsed
	cmdEnum_push_button::cmdEnum player_input;//stores player input
	buzzer_unit output;//output object
	button_input input;//input object
	controller control;//control object
	IntervalCheckTimer timer;//timer object for response time
	int gamemode; //variable to hold gamemode
	bool init_flag; 
	//functions
	bool playGame(int length, cmdEnum_push_button::cmdEnum* sequence) {//plays a game of input length
		for (int i = 0; i < length; i++) {//check user input for each cue
			time_elapsed = false;//set time elapsed flag to false when checking each new user input
			timer.updateCheckTime();//update check timer before each user input
			while (!time_elapsed) {//while time has not elapsed
				time_elapsed = timer.isMinChekTimeElapsed();//update time elapsed flag
				if (time_elapsed) { return false; }//if time elapsed, before input detected exit game as a loss
				if (input.isButtonPressed()) {//check for user input button press detected
					if (control.checkInput(sequence[i], input.getPlayerInput())) {//check if input is correct
						output.playCue(sequence[i], true, cue_length);//play feedback
						time_elapsed = true;//set time elapsed flag to true so that next cue can be checked
					}
					else {//if incorrect input
						return false;//end game as a loss
					}
				}
			}
		}
		return true;
	}
	void playMatchDefault() {//this function starts match
		userSelectDifficulty();//prompt user to select difficulty level for the match
		while (!control.isMatchOver()) {//while match end conditions not met
			simon_sequence new_sequence;//generate new sequence
			new_sequence.setButtonsUsed(input.getButtonsUsed());//initialises the sequence
			Serial.print("Current score: ");
			control.printScore();//print score to serial before game starts
			Serial.println("Are you rady for the next round? (press any button to start)");
			input.waitForInput();//halt game until user is ready
			output.countdown();//play the countdown
			output.playCueSequence(new_sequence.getSequence(), sequence_length, cue_length);//play the cue sequence for this game
			result = playGame(sequence_length, new_sequence.getSequence());//play game, game result stored in result flag
			control.updateScore(result);//update score
			if (result) {//if player wins game
				output.gameWin();//play game win song
			}
			else {//if player loses
				output.gameLoss();//play game loss song
			}
		}
		result = control.didPlayerWin();//if player wins assign true, simon wins assign false
		if (result) {//if player wins match
			output.matchWin();//play match win sequence
		}
		else {//if simon wins match
			output.matchLoss();//play match loss song
		}
		Serial.print("Final score: ");
		control.printScore();//prints final score
		control.resetScore();//reset scores to, can now start new game
	}
	void playMatchHS() {//one sequence generated at start of match, pllayer tries to get the highest score possible
		simon_sequence new_sequence;//generate new sequence
		new_sequence.setButtonsUsed(input.getButtonsUsed());//initialises the sequence
		sequence_length = 1;//start with sequence length 1
		userSelectDifficulty();
		result = true;//kind of like 0th game was won
		while (result) {
			Serial.println("Current score: " + (String)(sequence_length - 1));//print current high score
			Serial.println("Are you rady for the next round? (press any button to start)");
			input.waitForInput();//halt game until user is ready
			output.countdown();
			output.playCueSequence(new_sequence.getSequence(), sequence_length, cue_length);//play the cue sequence for this round
			result = playGame(sequence_length, new_sequence.getSequence());
			if (result) { output.gameWin(); }
			sequence_length++;
		}
		output.gameLoss();
		Serial.println("final score: " + (String)(sequence_length - 2));//print current high score
	}

	void playMatchAdaptive() {//in this gamemode the difficulty changes based on score
		while (!control.isMatchOver()) {//while match end conditions not met
			setDifficulty(control.determineDifficulty());//control determines what difficulty level to select at start of game
			simon_sequence new_sequence;//generate new sequence
			new_sequence.setButtonsUsed(input.getButtonsUsed());//initialises the sequence
			Serial.print("Current score: ");
			control.printScore();//print score to serial before game starts
			Serial.println("Are you rady for the next round? (press any button to start)");
			input.waitForInput();//halt game until user is ready
			output.countdown();//play the countdown
			output.playCueSequence(new_sequence.getSequence(), sequence_length, cue_length);//play the cue sequence for this game
			result = playGame(sequence_length, new_sequence.getSequence());//play game, game result stored in result flag
			control.updateScore(result);//update score
			if (result) {//if player wins game
				output.gameWin();//play game win song
			}
			else {//if player loses
				output.gameLoss();//play game loss song
			}
		}
		result = control.didPlayerWin();//if player wins assign true, simon wins assign false
		if (result) {//if player wins match
			output.matchWin();//play match win sequence
		}
		else {//if simon wins match
			output.matchLoss();//play match loss song
		}
		Serial.print("Final score: ");
		control.printScore();//prints final score
		control.resetScore();//reset scores to, can now start new game

	}
	void userSelectDifficulty() {//prompt user to select difficulty
		int difficulty = default_difficulty;//set difficulty to default
		bool confirmed = false;//flag for if the difficulty selection is confirmed
		Serial.println("Please select difficulty: (button1:++  button2:-- button3: confirm)");//give user instructions for difficulty selection
		Serial.println("Default difficulty: " + (String)(difficulty));//print default difficulty
		while (!confirmed) {//while user hasnt confirmed difficulty selection
			input.waitForInput();//wait for an input
			switch (input.getPlayerInput()) {//when input received decide how to change difficulty
			case lables[0]://if user pressed button with lable "1"
				if (difficulty < 5) { difficulty++; } break;// increase diff, make sure the difficulty doesnt go out of bounds
			case lables[1]:
				if (difficulty > 1) { difficulty--; } break;// decrease diff, make sure the difficulty doesnt go out of bounds
			case lables[2]:
				confirmed = true; break; //if button3 pressed, confirm difficulty setting
				//if button4 or button 5 pressed do nothing
			}
			Serial.println("Slected difficulty: " + (String)(difficulty));//print difficulty selection
		}
		setDifficulty(difficulty);
		Serial.println("Difficulty set, have fun!");//notify user that difficulty selected and match will begin
	}
	void setDifficulty(int level) {//preset difficulty levels in range [1,5]
		int t_resp;//resposne time for each input
		switch (level) {//based on selected difficulty level
		case 1: t_resp = 2000; break;//very easy
		case 2: t_resp = 1500; break;
		case 3: t_resp = 1000; break;
		case 4: t_resp = 800; break;
		case 5: t_resp = 500; break;//very difficultf
		}
		timer.setInterCheck(t_resp);//set timer to check in an interval of t_reso
		cue_length = t_resp / 2;//cue lengths made shorter than t_resp for nicer playing experience
	}
	void userSelectGamemode() {
		gamemode = 1;//set gamemode to default
		bool confirmed = false;
		Serial.println("Please select gamemode: (button1:++  button2:-- button3: confirm)");//give user instructions for gamemode selection
		Serial.println("Available gamemodes:");//print gamemode options
		Serial.println("1 - Default");//game mode as required per guidelines
		Serial.println("2 - High score");//like the original simon says
		Serial.println("3 - Default but simon's difficulty is adaptive");// simon will do his best to win the game (adaptive difficulty)
		while (!confirmed) {//while user hasnt confirmed diffficulty selection
			input.waitForInput();//wait for an input
			switch (input.getPlayerInput()) {//when input received decide how to change difficulty
			case lables[0]://if user pressed button with lable "1"
				if (gamemode < 3) { gamemode++; } break;// increase gamemode
			case lables[1]:
				if (gamemode > 1) { gamemode--; } break;// decrease gamemode
			case lables[2]:
				confirmed = true; break; //if button3 pressed, confirm difficulty setting
				//if button4 or button 5 pressed do nothing
			}
			Serial.println("Slected gamemode: " + (String)(gamemode));//print difficulty selection
		}
		Serial.println("Gamemode set, have fun!");//notify user that difficulty selected and match will begin
	}
public:
	simon() {
		init_flag = false;
		sequence_length = default_length;
	}
	simon(buzzer_unit buzzer, button_input button) {
		output = buzzer;//assign output module to be used
		input = button;//assign input module to be used
		sequence_length = default_length;//set sequence length to default length
		init_flag = output.isInitialised() && input.isInitialised();//controller always initialised when called so no need to check
	}
	setUpSimon(buzzer_unit buzzer, button_input button) {
		if (!init_flag) {
			output = buzzer;//assign output module to be used
			input = button;//assign input module to be used
			sequence_length = default_length;//set sequence length to default length
			init_flag = output.isInitialised() && input.isInitialised();//controller always initialised when called so no need to check
		}
	}
	void gameSelect(){//main function that system executes
		if (init_flag) {
			userSelectGamemode();
			switch (gamemode) {
			case 1: playMatchDefault(); break;
			case 2: playMatchHS(); break;
			case 3: playMatchAdaptive(); break;
			}
		}
	}
};

//Test function.

void test_system() {
	buzzer_unit action(5);//initialise action unit
	int pins[5] = { 2, 4, 7, 8, 13 };//pins for pushbuttons
	button_input sensing(pins, 200, 4);//initialise sensing unit
	simon test(action, sensing);//declare system unit with initialised action and sensing units
	test.gameSelect();// test if match plays properly, game results as expected, difficulty selection works etc.
}
#endif