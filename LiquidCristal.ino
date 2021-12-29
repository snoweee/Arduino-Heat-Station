#include <Wire.h>
#include <LiquidCrystal.h>
#include <DHT12.h>

const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;


LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
DHT12 sensor(&Wire);

const uint8_t backlight = 10;
const unsigned long sensorReadInterval = 5000;
void setup() {
  
  lcd.begin(16,2);
  lcd.clear();

  sensor.begin();
  
  pinMode(backlight, OUTPUT);
  digitalWrite(backlight, HIGH);
  
  delay(2000);


  lcd.setCursor(0,0);
  lcd.print("Temp:");
  lcd.setCursor(0,1);
  lcd.print("Humid:");
}


//verifica daca valorile s-au schimbat si da un boolean
boolean changed = true;
int compareValue = 0;
int oldCompareValue = 0;
float compareValueFloat = 0;
void checkChange(float temp, float humid)
{
  compareValueFloat = (temp + humid) * 100;
  compareValue = round(compareValueFloat);
  
  if(compareValue != oldCompareValue)
  {
    changed = true;
  }
  oldCompareValue = compareValue;
}



//valorile din loop
unsigned long currentTime = 0;
unsigned long lastLoopTime =0;
float humidity = 0;
float temperature = 0;

void loop() {



  //Ia de la senzor valorile o data la 5000 millis()
  unsigned long currentTime = millis();
  if(currentTime - lastLoopTime >= sensorReadInterval)
  {
    humidity = sensor.getHumidity();
   temperature = sensor.getTemperature();

    checkChange(temperature, humidity);
    
    lastLoopTime = currentTime;
  }

  //Daca valorile s-au schimbat, fa update la display
  if(changed)
  {
    lcd.setCursor(6,0);
    lcd.print(temperature);
    lcd.setCursor( 7,1);
    lcd.print(humidity);
    changed = false;
  }
}
