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
  lcd.print("     (*^_^*)  ");
  lcd.setCursor(0,1);
  lcd.print("  ");
  irrecv.enableIRIn(); 
  Serial.begin(9600); 
  pinMode(ir, OUTPUT);
  //pinMode(led, OUTPUT);
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

void kolo(int shim)
{
  analogWrite(led, shim);
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
        lcd.setCursor(0, 0);
        lcd.print("                   ");
        lcd.setCursor(0,1);
        lcd.print("                   ");
        lcd.setCursor(0, 0);
        lcd.print("Temp:");
        lcd.setCursor(6, 0);
        lcd.print(GetTemp(),1);
        break;
    case 0xFF9867:  //strelochki
        lcd.setCursor(0, 0);
        lcd.print("                   ");
        lcd.setCursor(0,1);
        lcd.print("                   ");
        lcd.setCursor(0, 0);
        lcd.print("     RELOAD");
        delay(1000);
        resetFunc();
        break;
    case 0xFF906F:  //plus
        
        kolo(255);
        break;
    case 0xFFA857:  //minus
        
        kolo(0);
        break;
    case 0xFFE01F: //eq
        setup();
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

double GetTemp(void)
{
  unsigned int wADC;
  double t;

  // The internal temperature has to be used
  // with the internal reference of 1.1V.
  // Channel 8 can not be selected with
  // the analogRead function yet.

  // Set the internal reference and mux.
  ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
  ADCSRA |= _BV(ADEN);  // enable the ADC

  delay(20);            // wait for voltages to become stable.

  ADCSRA |= _BV(ADSC);  // Start the ADC

  // Detect end-of-conversion
  while (bit_is_set(ADCSRA,ADSC));

  // Reading register "ADCW" takes care of how to read ADCL and ADCH.
  wADC = ADCW;

  // The offset of 324.31 could be wrong. It is just an indication.
  t = (wADC - 324.31 ) / 1.22;

  // The returned temperature is in degrees Celsius.
  return (t);
}
