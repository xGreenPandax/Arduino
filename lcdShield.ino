#include <EEPROM.h>
#include <Wire.h> // добавляем необходимые библиотеки
#include <LiquidCrystal.h>
#include "IRremote.h"

IRrecv irrecv(3);
LiquidCrystal lcd(8, 9, 4, 5, 6, 7 );
int led = 11;

decode_results results;
int k=0;
int ir=A5;

// Нажатые кнопки
int button;
const int BUTTON_NONE   = 0;
const int BUTTON_RIGHT  = 1;
const int BUTTON_UP     = 2;
const int BUTTON_DOWN   = 3;
const int BUTTON_LEFT   = 4;
const int BUTTON_SELECT = 5;
const int chipSelect = 3;

int getPressedButton()
{
  int buttonValue = analogRead(0); // считываем значения с аналогового входа(A0) 
  if (buttonValue < 100) {
    return BUTTON_RIGHT;  
  }
  else if (buttonValue < 200) {
    return BUTTON_UP;
  }
  else if (buttonValue < 400){
    return BUTTON_DOWN;
  }
  else if (buttonValue < 600){
    return BUTTON_LEFT;
  }
  else if (buttonValue < 800){
    return BUTTON_SELECT;
  }
  return BUTTON_NONE;
}

void setup()
{
  lcd.begin(16, 2); 
  delay(700);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Ne hodite deti");
  lcd.setCursor(0,1);
  lcd.print("   v shkolu");
  irrecv.enableIRIn(); 
  Serial.begin(9600); 
  pinMode(ir, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
  }
  
void leds(int timer)
{
      for(int i = 0; i<20;i++)
      {
        digitalWrite(led, HIGH);
        delay(timer);
        digitalWrite(led, LOW);
        delay(timer);
      }
}

void(* resetFunc) (void) = 0;

void loop()
{
  int r1 = analogRead(A4);
  int r2 = analogRead(A5);
  
  
  if ( irrecv.decode( &results )) { // если данные пришли
    switch ( results.value ) {
    case 0xFFA25D:
        if(k==0){
          digitalWrite( ir, LOW );
          k++;
        } else{
          digitalWrite( ir, HIGH );
          k--;
        }
        break;
    case 0xFF629D:
        leds(100);
        break;
    case 0xFF9867:
        lcd.setCursor(0, 0);
        lcd.print("                   ");
        lcd.setCursor(0,1);
        lcd.print("                   ");
        lcd.setCursor(0, 0);
        lcd.print("     RELOAD");
        delay(1000);
        resetFunc();
        break;
    }    
    irrecv.resume(); // принимаем следующую команду
  }
  
  
  button = getPressedButton();
  switch (button)
  {
    case BUTTON_RIGHT: // при нажатии кнопки выводим следующий текст
       lcd.setCursor(0, 0); 
       lcd.print("                 "); 
       lcd.setCursor(0, 0);
       lcd.print("RIGHT, Carl!");
       leds(200);
       break;
    case BUTTON_LEFT:
       lcd.setCursor(0, 0); 
       lcd.print("                   "); 
       lcd.setCursor(0, 0);
       lcd.print("LEFT, Carl!");
       leds(500);
       break;
    case BUTTON_UP:
       lcd.setCursor(0, 0); 
       lcd.print("                  "); 
       lcd.setCursor(0, 0);
       leds(50);
       lcd.print("Vverh, Carl!");
       break;
    case BUTTON_DOWN:
       lcd.setCursor(0, 0); 
       lcd.print("                "); 
       lcd.setCursor(0, 0);
       lcd.print("9, Carl!>_< 9!!!");
       break;
    case BUTTON_SELECT:
       lcd.setCursor(0,0);
  lcd.print("R1: ");
  lcd.print(analogRead(A4));
  lcd.setCursor(0,1);
  lcd.print("R2: ");
  lcd.print(analogRead(A5));
      
       break;
  }
}
