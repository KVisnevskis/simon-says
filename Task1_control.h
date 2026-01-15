#ifndef TASK1_CONTROL_H
#define TASK1_CONTROL_H
#include <Task1_sensing.h>//included for cmdEnum definiton
//this class keeps track of score, checks if input correct, checks if match is over
class controller {
protected:
	int simon_score, player_score; //keeps track of score
	int match_length;//number of games to be played in match, excluding tie breaks
	const int default_match_length = 6;//default match length
public:
	controller() {//if no value specified initialise to default values
		simon_score = 0;
		player_score = 0;
		match_length = default_match_length;
	}
	void setScores(int player, int simon) {
		simon_score = simon;
		player_score = player;
	}
	void resetScore() {
		simon_score = 0;
		player_score = 0;
	}
	void updateScore(bool playerWin) {
		if (playerWin) { //if playerWin = true, increment plater score
			player_score++; 
		}
		else { //otherwise increment simon score;
			simon_score++;
		}
	}
	bool checkInput(cmdEnum_push_button::cmdEnum expected, cmdEnum_push_button::cmdEnum player_input) {//checks if the player input is correct
		return (expected == player_input);//return true if input matches expected input
	}
	bool isMatchOver() {
		return ((simon_score + player_score == match_length) && (simon_score != player_score))
			||((simon_score + player_score == match_length+1)&&(simon_score==match_length/2|| player_score == match_length / 2));
		//if match length reached and not a tie, or if match had a tiebreak;
	}
	bool didPlayerWin() {
		if (isMatchOver()) {//if match is over
			return player_score > simon_score;//and player scored more
		}
		else {
			return false;
		}
	}
	void printScore() {//prints score to serial
		Serial.println("player (" + (String)(player_score) + ") - simon (" + (String)(simon_score)+")");
	}
	int determineDifficulty() {//for adaptive difficulty setting
		if (player_score == match_length/2) { return 5; }//if player about to win, set max difficulty
		if (simon_score == match_length / 2 + 1) { return 1; }//if simon has won, set easiest difficulty. this is simon milking his victory
		switch (player_score - simon_score) {
		case 0: return 3; break;//if currently a tie, set medium difficulty
		case 1: return 4; break;//if simon losing by 1 set to 2nd hardest
		case -1: return 2; break;//if simon winning by one set to 2nd easiest
		case -2: return 1; break;//if simon winning by 2 set to easiest
		}
		if (player_score - simon_score > 1) { return 4; }//if player winning by more than 1, set to 2nd hardest
		if (player_score - simon_score < -2) { return 1; }//if player losing by more than 2, set to easiest difficulty
		//this function probably should be rewritten in a proper way
	}
};

//Test functions

void testController() {
	controller test;
	for (int i = 0; i < 7; i++) {//all possible player scores
		for (int j = 0; j < 7; j++) {//all possible simon scores
			test.setScores(i, j);//set the scores
			if (test.isMatchOver()) {//if match end condition met
				test.printScore();//print final score (expected 8 scores printed)
				Serial.println("Adaptive simon would choose difficulty: "+(String)(test.determineDifficulty()));
				//print which difficulty level would be chosen at that score in an adaptive difficulty match
				//expected higher difficulty when simon is losing, lower when simon is winning. max when player at 3pts
			}//should print all possible score outcomes;
			if (test.didPlayerWin()) {
				Serial.println("player won!");
			}//4 player win cases expected
		}
	}
	int count = 0;//store number of matches found
	for (int i = 0; i < 5; i++) {//all possible sequence lables
		for (int j = 0; j < 5; j++) {//all possible sequence lables
			if (test.checkInput(lables[i], lables[j])) {//if the lables match
				count++;//increment match count
			}
		}
	}
	Serial.println((String)(count)+" matches found");//5 matches expected, (whenever i=j)
}
#endif