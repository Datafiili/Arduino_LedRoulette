// -------------------- Led Roulette -------------------- //
//Written By: Aarni Junkkala

//Libraries
#include <Adafruit_NeoPixel.h>

//Pins
const byte ButtonStart = 2;
int LedPin = 3;
int speaker = 4;

//Led setup
int NUMPIXELS = 12; // number of neopixels in strip
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LedPin, NEO_GRB + NEO_KHZ800);

//Time between ball moving from one led to another
int delayval = 25; // 10 is too fast, 50 too slow.
int delayvalReset = 0; //Resetvalue

//Color of the ball
int redColor = 0;
int greenColor = 0;
int blueColor = 255;

//Color of fader
int redFader = 50;
int greenFader = 50;
int blueFader = 50;

//Winning Color
int redWin = 255;
int blueWin = 0;
int greenWin = 0;

//Color of start Setup
int redStart = 100;
int greenStart = 0;
int blueStart = 100;

float clicking = 220; //Tone that speaker will make clicking happen

//If true, button needs to pressed one more time to stop the roulette
bool SecondButtonPress = false;

void setup() {
  Serial.begin(9600);
  delayvalReset = delayval;
  
  //Pin modes.
  pinMode(ButtonStart, INPUT_PULLUP);
  pinMode(speaker, OUTPUT);

  //Starts the led library
  pixels.begin();

  //Effect to show that code has started running and to set all leds off
  StartEffect();  
}

void loop()
{
  //Reads the ButtonStart. Holds the code untill button is pressed.
  while (digitalRead(ButtonStart) == LOW)
  {}
  
  //Resets stats that are required to be resetted.
  delayval = delayvalReset;
  
  //Resets the leds before spinning
  for (int i = 0; i < NUMPIXELS; i++){
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  }
  
  //When game starts rolls 3 rounds ignoring input
  for(int i = 0; i < 3; i++)
  {
    SpinBall();
  }

  //Repeats rounds for long as new ButtonStart input is detected
  while (SecondButtonPress == true and digitalRead(ButtonStart) == LOW){
    SpinBall();
  }
  
  //Last rounds before stopping, makes it fell more natural
  for(int i = 0; i < 5; i++)
  {
    SpinBall();
  }

  //Reset leds
  for(int i = 0; i < NUMPIXELS; i++)
  {
      pixels.setPixelColor(NUMPIXELS - i - 1, pixels.Color(0, 0, 0));
  }

  //Slower rounds.
  for(int k = 0; k < 2; k++)
  {
    SpinBallSlow();
  }
  
  //Final round
  SpinBallFinale();
}





void StartEffect(){
  //Resets all leds to be off.
  for(int j = 0; j < NUMPIXELS; j++)
  {
      pixels.setPixelColor(j, pixels.Color(0, 0, 0));
  }
  pixels.show();
  delay(50);

  //Starting color, where spins one rouond showing color of purple.
  for(int j = 0; j < NUMPIXELS; j++)
  {
      pixels.setPixelColor(j, pixels.Color(redStart, greenStart, blueStart));
      delay(25);
      pixels.show();

  }
  
  delay(50);

  //Reset leds to be off.
  for(int j = 0; j < NUMPIXELS; j++)
  {
      pixels.setPixelColor(j, pixels.Color(0, 0, 0));
      delay(25);
      pixels.show();
  }
}

//Spins the ball for a single round
void SpinBall(){ 
  for (int i=0; i < NUMPIXELS; i++)
  {
    //Resets the led that is 2 before current.
    pixels.setPixelColor((i + NUMPIXELS - 2) % NUMPIXELS, pixels.Color(0, 0, 0));
    //Sets the color of the ball
    pixels.setPixelColor(i, pixels.Color(redColor, greenColor, blueColor));
    //and Fader
    pixels.setPixelColor((i + NUMPIXELS - 1) % NUMPIXELS, pixels.Color(redFader, greenFader, blueFader));

    pixels.show();
    
    tone(speaker, clicking, 5);
    
    delay(delayval);
  }
}

//Spins the ball for a single round
void SpinBallSlow(){ 
  for (int i=0; i < NUMPIXELS; i++)
  {
    //Resets the led that is 2 before current.
    pixels.setPixelColor((i + NUMPIXELS - 1) % NUMPIXELS, pixels.Color(0, 0, 0));
    //Sets the color of the ball
    pixels.setPixelColor(i, pixels.Color(redColor, greenColor, blueColor));

    pixels.show();
    
    tone(speaker, clicking, 5);
    delayval += random(4,8);
    delay(delayval);
  }
}

void SpinBallFinale()
{
  //Precalculates the winning number
  int winNum = NUMPIXELS - random(1,NUMPIXELS); //Made this way, so if you have led ring of different amout of leds, you don't have change anything here.
  for (int i = 0; i < winNum; i++)
  {
    //Resetoi ledit
    for(int j = 0; j < NUMPIXELS; j++)
    {
        pixels.setPixelColor(j, pixels.Color(0, 0, 0));
    }
    
    pixels.setPixelColor(i, pixels.Color(redColor, greenColor, blueColor));
    
    if(i == winNum - 1){
      pixels.setPixelColor(i, pixels.Color(redWin, blueWin, greenWin));
      pixels.show();

      tone(speaker, 1479.98, 100);
      delay(100);
      tone(speaker, 1760.00, 300);
    }
    else{
      tone(speaker, clicking, 5);
       pixels.show();
    }
    //For the end delay increases
    delayval += random(20,45);
    // Delay for a period of time (in milliseconds).
    delay(delayval);
  }
}
