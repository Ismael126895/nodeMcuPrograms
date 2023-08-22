#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

float firstNum;
float secNum;
float answer;
char operand;
void setup() {
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
}

void loop() {
  lcd.setCursor(0,0);
  lcd.print("Input 1st Number");
  while (Serial.available()==0){
    
  }
  firstNum=Serial.parseFloat();
  Serial.println(firstNum);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Input 2nd Number");
  Serial.read();
  while (Serial.available()==0){
    
  }
  secNum=Serial.parseFloat();
  Serial.println(firstNum);
  Serial.println(secNum);
  lcd.clear();
  lcd.setCursor(0,0);
  
  lcd.print("Input Operator(+,-,*,/)");
  Serial.read();
  while (Serial.available()==0){
    
  }
  operand=Serial.read();
  switch (operand){
  case '+':
  answer=firstNum+secNum;
  break;
  case '-':
  answer=firstNum+secNum;
  break;
  case '*':
  answer=firstNum+secNum;
  break;
  case '/':
  answer=firstNum/secNum;
  break;
  }
  
  
  Serial.println(answer);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(firstNum);
  lcd.print(operand);
  lcd.print(secNum);
  lcd.setCursor(0,1);
  lcd.print(" = ");
  lcd.print(answer);
  delay(10000);
  lcd.clear();
}
