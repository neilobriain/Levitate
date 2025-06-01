/**
Levitate
by Neil Ó Briain

Hold the button to levitate and don't get caught. Good luck!
**/

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

const int buttonPin = 2;  // the number of the pushbutton pin

// custom characters
byte playerStand[] = { B01110, B01110, B00100, B01110, B00100, B01010, B01010, B11011 };
byte playerJump[] = { B01110, B01110, B10101, B01110, B00100, B11111, B10001, B10001 };
byte dog[] = { B00000, B00000, B00000, B01000, B11001, B01110, B01110, B01010 };
byte plane[] = { B00100, B00100, B00101, B01111, B11111, B00100, B00100, B00100 };
byte car[] = { B00000, B00000, B00000, B00110, B01001, B11111, B11111, B01010 };
byte ufo[] = { B01010, B00100, B01010, B11111, B11111, B11111, B01010, B00100 };

int buttonState = 0;  // variable for reading the pushbutton status

// initial positions for enemies
byte dogX = 15;
byte planeX = 15;
byte carX = 10;
byte ufoX = 10;

int score = 0;

void setup()
{
  
  // Sets up the LCD and custom characters
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.createChar(0, playerStand);
  lcd.createChar(1, playerJump);
  lcd.createChar(2, dog);
  lcd.createChar(3, plane);
  lcd.createChar(4, car);
  lcd.createChar(5, ufo);
  lcd.clear();

  lcd.print("Don't get caught!");
  delay(1000);

  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  
}

void loop()
{
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // Button makes the player jump
  if (buttonState == HIGH){
    lcd.setCursor(3, 0);
    lcd.write(byte(1));
  } else {
    lcd.setCursor(3, 1);
    lcd.write(byte(0));
  }

  // Dog runs and resets
  if (dogX > 0){
    dogX--;
  } else{
    dogX = 15;
    score++; // adds 1 to score for each dog that regenerates
  }

  // Plane flys and resets
  if (planeX > 1){
    planeX -= 2;
  } else{
    planeX = 15;
    score++;
  }

  // Car drives and resets
  if (carX > 1){
    carX -= 2;
  } else{ // resetting new car position
    carX = (byte) random(8, 14);  //randomly reset car position
    if (carX == planeX) { //if same as plane, add 1 to car
      carX++;
    }
    if (carX %2 == 0) { // if even, add 1 to car (so colision with player is possible)
      carX++;
    }
    score++;
  }

  // UFO flys and resets
  if (ufoX > 0){
    ufoX--;
  } else{ // resetting new car position
    ufoX = (byte) random(7, 14);  //randomly reset ufo position
    if (ufoX == dogX) { //if same as dog, add 1 to ufo
      ufoX++;
    }
    score++;
  }

  // draw enemies
  lcd.setCursor(planeX, 0);
  lcd.write(byte(3)); // plane
  lcd.setCursor(ufoX, 0);
  lcd.write(byte(5)); // ufo
  lcd.setCursor(dogX, 1);
  lcd.write(byte(2)); // dog
  lcd.setCursor(carX, 1);
  lcd.write(byte(4)); // car
 
  // check if dog bites player
  if (dogX == 3 && buttonState == LOW){
    gameOver('D', score);
    score = 0; // reset score for next round
  }

  // check if ufo probes player
  if (ufoX == 3 && buttonState == HIGH){
    gameOver('U', score);
    score = 0;
  }

  // check if plane hits player
  if (planeX == 3 && buttonState == HIGH){
    gameOver('P', score);
    score = 0;
  }

  // check if car hits player
  if (carX == 3 && buttonState == LOW){
    gameOver('C', score);
    score = 0;
  }

  delay(500);
  lcd.clear();
 
}

void gameOver(char enemy, int score){
  // tell user what enemy got it
  lcd.clear();
  switch (enemy){
    case 'C':
    lcd.print("Caught by car!");
    break;
    case 'D':
    lcd.print("Bit by dog!");
    break;
    case 'P':
    lcd.print("Plane intercept!");
    break;
    case 'U':
    lcd.print("UFO probe!");
    break;
    default:
    lcd.print("Error");
    break;
  }
  delay(1000);

  // print score and highscore
  lcd.clear();
  lcd.print("Score: ");
  lcd.print(score);
  delay(1000);
  lcd.clear();
  lcd.print("Highscore: ");
  lcd.print(getHighscore(score));
  delay(1000);
}

int getHighscore(int score){
  static int highscore = 0;
  // update highscore if current score is larger
  if (score > highscore){
    highscore = score;
  }
  return highscore;
}
