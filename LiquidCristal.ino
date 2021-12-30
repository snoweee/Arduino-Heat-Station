#include <Wire.h>
#include <LiquidCrystal.h>
#include <DHT12.h>
#include <EEPROM.h>


int highTempAddr = 0;

const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;


LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
DHT12 sensor;

byte degChar[] = {
  B01100,
  B10010,
  B10010,
  B01100,
  B00000,
  B00000,
  B00000,
  B00000
};

byte thermChar[] = {
  B01110,
  B10001,
  B10101,
  B10101,
  B10101,
  B10101,
  B11111,
  B11111
};

byte humidChar[] = {
  B00000,
  B00000,
  B00100,
  B01110,
  B11011,
  B11111,
  B11111,
  B01110
};

const uint8_t backlight = 10;
const unsigned long sensorReadInterval = 2500;
void setup() {
  
  lcd.begin(16,2);
  lcd.createChar(1, degChar);
  lcd.createChar(2, thermChar);
  lcd.createChar(3,humidChar);
  lcd.clear();

  sensor.begin();
  
  pinMode(backlight, OUTPUT);
  digitalWrite(backlight, HIGH);
  
  delay(2000);


  lcd.setCursor(0,0);
  lcd.write(2);
  lcd.setCursor(0,1);
  lcd.write(3);
}


//verifica daca valorile s-au schimbat si da un boolean
boolean changed = true;
int compareValue = 0;
int oldCompareValue = 0;
float compareValueFloat = 0;
float eepromTemp = 0;
void checkChange(float temp, float humid)
{
  compareValueFloat = (temp + humid) * 100;
  compareValue = round(compareValueFloat);
  
  if(compareValue != oldCompareValue)
  {
    changed = true;
    float f = 0;
    EEPROM.get(highTempAddr,f);
    eepromTemp = f;
    if( temp > f )
    {
      EEPROM.put(highTempAddr,temp);
      eepromTemp = f;
    }
  }
  oldCompareValue = compareValue;
}
void checkChange2(float temp, float humid)
{
  changed = true;
}


//valorile din loop
unsigned long currentTime = 0;
unsigned long lastLoopTime =0;
unsigned long lastLoopTime2 =0;
int humidity = 0;
float temperature = 0;

void loop() {



  //Ia de la senzor valorile o data la 5000 millis()
  unsigned long currentTime = millis();
  if(currentTime - lastLoopTime >= sensorReadInterval)
  {
    humidity = sensor.readHumidity();
   temperature = sensor.readTemperature();

    checkChange(temperature, humidity);
    
    lastLoopTime = currentTime;
  }

  if(currentTime - lastLoopTime2 >= 12000)
  {
    digitalWrite(backlight,LOW);
    lastLoopTime2 = currentTime;
  }
  if(analogRead(A0) < 900)
  {
    digitalWrite(backlight,HIGH);
    lastLoopTime2=currentTime;
  }

  //Daca valorile s-au schimbat, fa update la display
  if(changed)
  {
    lcd.setCursor(2,0);
    lcd.print(temperature);
    lcd.write(1);
    lcd.print("C");
    lcd.setCursor(11,0);
    lcd.print(eepromTemp);
    lcd.setCursor( 2,1);
    lcd.print(humidity);
    lcd.print("%");
    changed = false;
  }
}
