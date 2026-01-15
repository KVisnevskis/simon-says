#include <IntervalCheckTimer.h>
#include <CmdEnumPushButton.h>
#include <Beginner_LED.h>
#include <Task1_system.h>
#include <Task1_control.h>
#include <Task1_action.h>
#include <Task1_sensing.h>

simon the_game;//declare system uninitialised
void setup() {
  // put your setup code here, to run once:
  buzzer_unit action(5);//declare and initialise action unit
  int pins[5] = { 2, 4, 7, 8, 13 };//pins for pushbuttons
  button_input sensing(pins, 200, 4);//declare and initialise sensing unit
  the_game.setUpSimon(action, sensing);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  the_game.gameSelect();

}
