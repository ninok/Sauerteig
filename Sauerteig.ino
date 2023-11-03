#include <rgb_lcd.h>

rgb_lcd lcd;

const int colorR = 255;
const int colorG = 255;
const int colorB = 0;

// Define the pin to which the temperature sensor is connected.
const int pinTemp = A0;
const int relayPin =  7;

// Define the B-value of the thermistor.
// This value is a property of the thermistor used in the Grove - Temperature Sensor,
// and used to convert from the analog value it measures and a temperature value.
const int B = 3975;

const float targetTemp = 26;
const float startHeatingTemp = 25.5;
const float stopHeatingTemp = 26.5;
bool heating = false;

void setup() 
{
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    
    lcd.setRGB(colorR, colorG, colorB);
    
    // Print a message to the LCD.
    lcd.print("Aktuelle Temp.:");

    // Configure the relay's pin for output signals.
    pinMode(relayPin, OUTPUT);

    delay(1000);
}

void startHeating() {
  heating = true;
  digitalWrite(relayPin, HIGH);
  lcd.setRGB(255, 0, 0);
}

void stopHeating() {
  heating = false;
  digitalWrite(relayPin, LOW);  
  lcd.setRGB(0, 255, 0);
}

// Schmitt trigger for controlling the heating
void checkHeating(float temperature) {
  if (heating) {
    if(temperature > stopHeatingTemp) {
      stopHeating();
    }
  } else {
    if (temperature < startHeatingTemp) {
      startHeating();
    }
  }
}

void loop() 
{
    // set the cursor to column 0, line 1
    // (note: line 1 is the second row, since counting begins with 0):
    lcd.setCursor(0, 1);

    // Get the (raw) value of the temperature sensor.
    int val = analogRead(pinTemp);

    // Determine the current resistance of the thermistor based on the sensor value.
    float resistance = (float)(1023-val)/val;

    // Calculate the temperature based on the resistance value.
    const float offset = 0; //offset for calibration
    float temperature = 1/(log(resistance)/B+1/298.15)-273.15 - offset;

    lcd.print(temperature);

    checkHeating(temperature);
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
