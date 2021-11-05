#include <Bounce2.h>
#include <Keyboard.h>
#include <Encoder.h>

/*
 * Sketch made for my 3x3 macro pad with an encoder and state button. The pad can cycle through 4 states so a total of 36 commands can be set on it.
 * The encoder is used for the media, the rotation is for volume, CW for volume up and CCW for volume down. The button will be used to play/pause on 
 * a short press and a long (500ms) for skip track.
 * If you add more buttons or states, you will need to updated the pin arrays, as well as add the options in the massive switch/case in the loop.
 */
 
//Set up the button grid
const int numButtons = 9;
const int buttonPins[numButtons] = {4,3,2,7,6,5,10,9,8}; //Array of button pins in an order that makes sense to me
//Set up all the buttons as bounce objects
Bounce buttons[] = {Bounce(buttonPins[0],10),Bounce(buttonPins[1],10),Bounce(buttonPins[2],10),Bounce(buttonPins[3],10),Bounce(buttonPins[4],10),Bounce(buttonPins[5],10),Bounce(buttonPins[6],10),Bounce(buttonPins[7],10),Bounce(buttonPins[8],10)};

//Set up the states
const int numStates = 4;
const int statePins[numStates] = {16,17,18,19}; //State LED pins, in order that makes sense
const int statePin = 11;
Bounce stateButton = Bounce(statePin,10); //Set up the state buttons as a bounce object
int state = 0; //Set the current state

//Set up the encoder
const int encoderButtonPin = 12;
Encoder volumeKnob(14,15); //Set up the encoder as an Encoder object
Bounce encoderButton = Bounce(encoderButtonPin,10); //Set up to the encoder button as a Bounce object
int timeLimit = 500; //Cut off time for play/pause or skip

int ledState = LOW;


void setup() {
  Serial.begin(9600);
  Keyboard.begin(); //Start the Keyboard object
  for(int i = 0; i < numButtons; i++){
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  pinMode(statePin, INPUT_PULLUP);
  pinMode(encoderButtonPin, INPUT_PULLUP);
  
  //Cycle through the state pins as a startup animation
  for(int i = 0; i < numStates; i++) {
    pinMode(statePins[i],OUTPUT);
    digitalWrite(statePins[i],HIGH);
    delay(250);
    digitalWrite(statePins[i],LOW);
  }
  digitalWrite(statePins[state],HIGH);

  
  //Turn the buildin LED off - used for debugging
  //pinMode(13,OUTPUT);
  //digitalWrite(13,ledState);
}

long positionLeft = -999;

void loop() {
  //check all buttons
  for(int j = 0; j < numButtons; j++){
    if(buttons[j].update()){
      if(buttons[j].fallingEdge()){
        //Toggle the builtin LED - used for ensuring buttons are working
        //ledState = !ledState;
        //Serial.write("Button");
        //digitalWrite(13,ledState);

        //use the current state and the button number to find the command needed and send it.
        switch (state) {
          case 0: //Layout 1
            switch (j) {
              case 0: //copy
                Keyboard.press(KEY_RIGHT_CTRL);
                Keyboard.press('c');
                delay(100);
                Keyboard.releaseAll();
                break;
              case 1: //paste
                Keyboard.press(KEY_RIGHT_CTRL);
                Keyboard.press('v');
                delay(100);
                Keyboard.releaseAll();
                break;
              case 2: 
                
                break;
              case 3: //all
                Keyboard.press(KEY_RIGHT_CTRL);
                Keyboard.press('a');
                delay(100);
                Keyboard.releaseAll();
                break;
              case 4: //
                break;
              case 5:
                break;
              case 6: // save
                Keyboard.press(KEY_RIGHT_CTRL);
                Keyboard.press('s');
                delay(100);
                Keyboard.releaseAll();
                break;
              case 7:
                break;
              case 8:
                break;
            }
            break;
          case 1: //Layout 2
            switch (j) {
              case 0: //undo
                Keyboard.press(KEY_RIGHT_CTRL);
                Keyboard.press('z');
                delay(100);
                Keyboard.releaseAll();
                break;
              case 1: 
                break;
              case 2:          
                break;
              case 3: 
                break;
              case 4: 
                break;
              case 5: 
                break;
              case 6: // save
                Keyboard.press(KEY_RIGHT_CTRL);
                Keyboard.press('s');
                delay(100);
                Keyboard.releaseAll();
                break;
              case 7:
                break;
              case 8:
                break;
            }
            break;
          case 2: //Layout 3
            switch (j) {
              case 0: // escape
                Keyboard.press(KEY_ESC);
                Keyboard.releaseAll();
                break; 
              case 1: 
                break;
              case 2: 
                break;
              case 3: 
                break;
              case 4: //
                break;
              case 5:
                break;
              case 6:
                break;
              case 7:
                break;
              case 8:
                break;
            }
            break;
          case 3: //Layout 4
            switch (j) {
              case 0: 
                break;
              case 1: 
                break;
              case 2: 
                break;
              case 3: 
                break;
              case 4: 
                break;
              case 5: 
                break;
              case 6:
                break;
              case 7: 
                break;
              case 8: 
                break;
            }
            break;
        }
      }
    } 
  }

  //state change button pressed
  if(stateButton.update()){
    if(stateButton.fallingEdge()){
      digitalWrite(statePins[state],LOW); //turn off current LED
      state++;
      if(state >= numStates) {
        state = 0;
      }
      digitalWrite(statePins[state],HIGH);
      Serial.print(state);
    }
  }

  //encoder play pause
  if(encoderButton.update()) {
    if(encoderButton.fallingEdge()) {
      int fall = millis();
      while(!encoderButton.update()){}
      if(encoderButton.risingEdge()){
        int rise = millis();
        Serial.println(rise - fall);
        if(rise - fall > timeLimit){
          Keyboard.press(KEY_MEDIA_NEXT_TRACK);
        } else {
          Keyboard.press(KEY_MEDIA_PLAY_PAUSE);
        }
      }
      Keyboard.releaseAll();
    }
  }
  
  //encoder volume changing
  long newLeft;
  newLeft = volumeKnob.read();
  if(newLeft != positionLeft){
    if((newLeft - positionLeft) > 0) {
      //volumeup
      Serial.println("volume up");
      Keyboard.press(KEY_MEDIA_VOLUME_INC);
    } else {
      //volumedown
      Serial.println("volume down");
      Keyboard.press(KEY_MEDIA_VOLUME_DEC);
    }
    Keyboard.releaseAll();
    delay(200);
    positionLeft = newLeft;
  }
}<br>
