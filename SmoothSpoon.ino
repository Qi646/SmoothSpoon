#include <Servo.h>
#include <LiquidCrystal.h>

int sensorThumb, sensorWrist = 0;
Servo servoThumb, servoWrist;
const int x = 10;
int thumbValues[x], wristValues[x];
int index = 0;
LiquidCrystal lcd(13, 2, 3, 4, 11, 12);
int screen = 1;
const long interval = 1000;
unsigned long previousMillis = 0;
bool showWelcome = true; // Flag to track the welcome screen
bool loading = true; // Flag to track loading state
const int buttonPin = 7;
int buttonState = LOW;
int lastButtonState = LOW;
bool buttonPressed = false; // Flag to track if button is pressed
int redLedPin = 10;
int yellowLedPin = 9;
int greenLedPin = 8;
bool showGreenPin = true;
bool showWorking = true;
int flashState = 0;

double average(int a[], int n)
{
    // Find sum of array element
    int sum = 0;
    for (int i = 0; i < n; i++)
        sum += a[i];

    return (double)sum / n;
}

void setup()
{
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(buttonPin, INPUT); // Change button pin mode to INPUT
    pinMode(8, OUTPUT);
    pinMode(9 ,OUTPUT);
    pinMode(10 ,OUTPUT);
    servoThumb.attach(6);
    servoThumb.write(0);
    servoWrist.attach(5);
    servoWrist.write(0);
    lcd.begin(16, 2);
}

void loop()
{
    // Get sensor data
    sensorThumb = analogRead(A0);
    sensorWrist = analogRead(A1);

    if (index >= x)
    {
        index = 0;
    }
    thumbValues[index] = 90 - (sensorThumb - 128) * 0.71;
    wristValues[index] = 180 - (sensorWrist - 128) * 0.71;


    // Debugging data
    Serial.print("Thumb flex = ");
    Serial.println(sensorThumb);
    Serial.print("Wrist flex = ");
    Serial.println(sensorWrist);
    Serial.println(""); // Empty line for clarity

    double thumbAvg = average(thumbValues, x);
    double wristAvg = average(wristValues, x);

    servoThumb.write(thumbAvg);
    servoWrist.write(wristAvg);

    unsigned long currentMillis = millis(); // Get the current time

    if (screen == 1)
    {
        if (loading)
        {
            digitalWrite(redLedPin, LOW);
            digitalWrite(yellowLedPin, HIGH);
            digitalWrite(greenLedPin, LOW);
            lcd.clear();
            lcd.write("Loading...");
            if (currentMillis - previousMillis >= 1000) // Display loading for 1 second
            {
                loading = false;
                previousMillis = currentMillis;
            }
        }
        else
        {
            digitalWrite(redLedPin, LOW);
            digitalWrite(yellowLedPin, LOW);
            digitalWrite(greenLedPin, HIGH);
            if (showWelcome)
            {
                lcd.clear();
                lcd.write("Welcome to");
                lcd.setCursor(0, 1);
                lcd.write("SmoothSpoon");
            }
            else
            {
                lcd.clear();
                lcd.write("Press Button");
                lcd.setCursor(0, 1);
                lcd.write("to start");
            }
        }

        // Check for button press
        buttonState = digitalRead(buttonPin);
        if (buttonState == LOW && lastButtonState == HIGH)
        {
            buttonPressed = true; // Set button pressed flag
        }

        lastButtonState = buttonState;

        if (buttonPressed) { // If button is pressed, switch to the next screen
            delay(1000); // Delay before switching to the next screen
            screen++;
            loading = true; // Reset loading flag
        }
        else
        {
            if (!loading)
            {
                if (currentMillis - previousMillis >= 1000) // Flash every 1 second
                {
                    showWelcome = !showWelcome; // Toggle the welcome screen
                    previousMillis = currentMillis;
                }
            }
        }
    }
    else if (screen == 2)
    {
      if (showGreenPin)
      {
        digitalWrite(redLedPin, LOW);
        digitalWrite(yellowLedPin, LOW);
        digitalWrite(greenLedPin, HIGH);
      }
      else
      {
        digitalWrite(redLedPin, LOW);
        digitalWrite(yellowLedPin, LOW);
        digitalWrite(greenLedPin, LOW);
      }
      
      if (currentMillis - previousMillis >= 1000) // Flash every 1 second
      {
        showGreenPin = !showGreenPin;
        showWorking = !showWorking;
        previousMillis = currentMillis;
      }
      
      if (showWorking)
      {
        lcd.clear();
        lcd.write("Working...");
      }
      else
      {
        lcd.clear();
        lcd.write("Press Button");
        lcd.setCursor(0, 1);
        lcd.write("to stop");
      }
      
      buttonState = digitalRead(buttonPin);
      if (buttonState == LOW && lastButtonState == HIGH)
      {
        screen++;
        lastButtonState = LOW;
      }
      
      lastButtonState = buttonState;
    }
    else if (screen == 3)
    {
        digitalWrite(redLedPin, HIGH);
        digitalWrite(yellowLedPin, LOW);
        digitalWrite(greenLedPin, LOW);
      
        // Flashing text between "Thank you for using", "SmoothSpoon", and "Press Button to restart"
        if (currentMillis - previousMillis >= 1500) // Flash every 1.5 second
        {
            lcd.clear();
            switch (flashState)
            {
                case 0:
                    lcd.write("Thank you");
                    lcd.setCursor(0, 1);
                    lcd.write("for using");
                    break;
                case 1:
                    lcd.write("SmoothSpoon");
                    break;
                case 2:
                    lcd.write("Press Button");
                    lcd.setCursor(0, 1);
                    lcd.write("to restart");
                    break;
            }
            flashState = (flashState + 1) % 3; // Cycle through the states
            previousMillis = currentMillis;
        }   
      
        buttonState = digitalRead(buttonPin);
        if (buttonState == LOW && lastButtonState == HIGH)
        {
          screen = 1;
          buttonPressed = false;
          lastButtonState = LOW;
        }
      
        lastButtonState = buttonState;
    }

    index += 1;
    delay(100); // Update delay in milliseconds
}
