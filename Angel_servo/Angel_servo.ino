#include <Servo.h>
#include <Arduino.h>
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
bool inputFailedFlag = false;

Servo camera;
int CurPose = 0;
int desiredPose = 0;

void setup() {
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  
  camera.attach(9,700,2300);
  camera.write(90); // initialization
  CurPose = camera.read();// be aware of this function, it reads from the last input. (there is no actual feedback) 

  // initialize serial:
  Serial.begin(115200);
}

void loop() {
	
  // print the string when a newline arrives:
  if (stringComplete)
  {
    if ( 0 <= inputString.toInt() && inputString.toInt() <= 180)
    {
     
      Serial.print("spining to: ");
      Serial.println(desiredPose = inputString.toInt());
      while (CurPose != desiredPose)
      {
        delay(15);
        if (CurPose > desiredPose)
        {
          camera.write(CurPose--);
        }
        else if (CurPose < desiredPose)
        {
          camera.write(CurPose++);
        }
      }
    }
    else
    {
      Serial.println("Invalid angle. Please re-enter.");
    }
    // clear the string:
    inputString = "";
    stringComplete = false;
    Serial.println("Position reached");
  }
  else;
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent()
{
  while (Serial.available())
  {
    // get the new byte:
    char inChar = (char)Serial.read();

    // add it to the inputString:
    inputString += inChar;

    if ( ((!(48 <= inChar && inChar <= 57)) && !(inChar == 10)) && inputFailedFlag == false) // ascii code 0 ~ 9, new line
    {
      inputFailedFlag = true;
      //Serial.println("I check: ");
    }
    else if (inChar == '\n')
    {
      // if the incoming character is a newline, set a flag
      // so the main loop can do something about it:
      if (inputFailedFlag == false)
      {
        stringComplete = true;
      }
      else
      {
        Serial.print("Received: ");
        Serial.println(inputString);
        stringComplete = false;
        inputString = "";
        inputFailedFlag = false; //reset
        Serial.println("Invalid input. Please re-enter.");
      }
    }
    else;
  }
}
