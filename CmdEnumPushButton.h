#ifndef CmdEnumPushButton_h
#define CmdEnumPushButton_h

// various include options: all seem to work.
//#include "..\IntervalCheckTimer\IntervalCheckTimer.h"
//#include <IntervalCheckTimer.h>
#include "IntervalCheckTimer.h"


class cmdEnum_push_button{
public:
	enum cmdEnum{cmd_start=0,
		button1, button2, button3, button4,
		start, stop, reverse, fwd, bwd,
		myOwnCommand1, myOwnCommand2,
		cmd_stop};
private:
	cmdEnum command_name;
	boolean new_command;
	int buttonPin;
	boolean initialized;
	int buttonState; 
	int lastButtonState, reset_buttonState; // LOW or HIGH;  current and previous reading from the input pin
	int buttonState_for_new_command;
	long lastDebounceTime;  // the last time the output pin was toggled
	long debounceDelay; //50 ms;
	
	// used to avoid reading a "long push" as multiple individual pushes
	bool avoid_longpush;
	IntervalCheckTimer longpush_check;

	//
	void setup_in_push_button()
	{
		new_command=false;		
		//buttonState_for_new_command=HIGH;
		buttonState_for_new_command=LOW;

		command_name = cmd_start;

		lastDebounceTime=0;  // the last time the output pin was toggled
		debounceDelay=25; //ms;
		initialized=false;
		//
		reset_buttonState=LOW;
		buttonState=reset_buttonState;
		lastButtonState=reset_buttonState; 

		//
		avoid_longpush=false;
		//
	}

	void setup_in_push_button(int in_buttonPin, cmdEnum in_command)
	{
		setup_in_push_button();	
		buttonPin=in_buttonPin;		
		//pinMode(buttonPin,INPUT); 
		pinMode(buttonPin,INPUT_PULLUP); 

		initialized=true;
		// set the associated command
		command_name = in_command;
	}

	void setup_in_push_button(int in_buttonPin, cmdEnum in_command, unsigned long int longpush_ms)
	{
		setup_in_push_button(in_buttonPin, in_command);
		//
		avoid_longpush=true;
		longpush_check.setInterCheck(longpush_ms);
		//
	}

public:
	cmdEnum_push_button() {setup_in_push_button();}
	cmdEnum_push_button(int in_buttonPin, cmdEnum in_command) {setup_in_push_button(in_buttonPin, in_command);}
	cmdEnum_push_button(int in_buttonPin, cmdEnum in_command, unsigned long int longpush_ms) {setup_in_push_button(in_buttonPin, in_command, longpush_ms);}
	//
	cmdEnum_push_button(const cmdEnum_push_button& other) 
	{
		setup_in_push_button(); 
		copyFrom(other);
	}
	//
	void assign_pin_command(int in_buttonPin, cmdEnum in_command){setup_in_push_button(in_buttonPin, in_command);}
	void assign_pin_command(int in_buttonPin, cmdEnum in_command, unsigned long int longpush_ms){setup_in_push_button(in_buttonPin, in_command, longpush_ms);}
	
	// independet setup of "long_push"
	void setup_longpush(unsigned long int longpush_ms)
	{
		avoid_longpush = true;
		longpush_check.setInterCheck(longpush_ms);
	}

	// return one i the button has been pressed.
	boolean check_new_input()
	{
		boolean new_input_found=false;
		long curr_Time;
		int in_buttonState;
		new_command=false;

		if( initialized==true)
		{	
			in_buttonState=digitalRead(buttonPin);
			//test:
			buttonState=in_buttonState;
			//

			if(in_buttonState!=lastButtonState)
				lastDebounceTime=millis();

			curr_Time=millis();
			if( (curr_Time - lastDebounceTime) > debounceDelay )
			{
				//buttonState=in_buttonState;
				if(in_buttonState==buttonState_for_new_command)
				{
					new_input_found=true;
					new_command=true;
				}
			}
			lastButtonState=in_buttonState;
			buttonState=reset_buttonState;

			
			// if avoid_longpush is true:
			// ignore a positive detection if the last succeful reading was recent
			if(avoid_longpush && new_input_found)
				if( !longpush_check.isMinChekTimeElapsedAndUpdate() )
					new_input_found=false;
			

		}

		return new_input_found;
	}

	cmdEnum get_new_input()
	{
		cmdEnum current_command;

		if(new_command==true)
			current_command = command_name ;
		else
			current_command= cmd_start;

		return current_command;
	}

	bool check_n_get_new_input(cmdEnum& current_command)
	{
		current_command = cmd_start;
		bool result = check_new_input();
		if(result)
			current_command=get_new_input();		
		return result;
	}

	boolean check_enabled() {return initialized;}
	// equal operator
	cmdEnum echoCmdSetting() {return command_name;}
	int echoPinSetting() { return buttonPin;}
	bool echoLongpushSetting(unsigned long int& out_longpush_ms)
	{
		if (avoid_longpush)
		{
			out_longpush_ms = longpush_check.echoInterCheckMsSetting();
		}
		return avoid_longpush;
	}
	void copyFrom(const cmdEnum_push_button& other)
	{
		// avoid self copy
		if (this != &other)
		{	// only copy from an initialized object to a not-initialized one
			if ((other.check_enabled()) && (!this->check_enabled()))
			{
				unsigned long int out_longpush_ms;				
				//set pins and command
				this->setup_in_push_button(other.echoPinSetting(),other.echoCmdSetting());
				if (other.echoLongpushSetting(out_longpush_ms))
				{	// copy longpush if enabled
					this->setup_longpush(out_longpush_ms);
				}
			}
		}
	}
	cmdEnum_push_button& operator = (const cmdEnum_push_button& other)
	{
		this->copyFrom(other);
		return*this;
	}
	////
};



#endif
