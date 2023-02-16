#include "DHT.h"
#include <Servo.h>

#define dhtType DHT11

uint8_t const leftMotorPin0 = 5;
uint8_t const leftMotorPin1 = 6;
uint8_t const rightMotorPin0 = 10;
uint8_t const rightMotorPin1 = 11;

uint8_t const buzzerPin = 7;
uint8_t const echoPin = 12;
uint8_t const triggerPin = 13;

uint8_t const dhtPin = 8;
uint8_t const servoPin = 9;

unsigned long const toneDuration_ms = 1000;
unsigned int const toneFrequency_hz = 200;

unsigned long pulseDuration_us;
float obstacleDistance_cm;
float temperature_C;
String command;
uint8_t leftMotor0 = 0;
uint8_t leftMotor1 = 0;

DHT dht(dhtPin, dhtType);
Servo myServo;

void readSurroundingTemperature();
float calculateDistanceToObstacle();

void setup()
{
    Serial.begin(9600);

    pinMode(leftMotorPin0, OUTPUT);
    pinMode(leftMotorPin1, OUTPUT);
    pinMode(rightMotorPin0, OUTPUT);
    pinMode(rightMotorPin1, OUTPUT);

    pinMode(buzzerPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(triggerPin, OUTPUT);

    dht.begin();

    myServo.attach(servoPin);
    myServo.write(110);
    
    delay(200);
}

void loop()
{
    obstacleDistance_cm = calculateDistanceToObstacle();
    if (0 < obstacleDistance_cm && obstacleDistance_cm < 30)
        tone(buzzerPin, toneFrequency_hz, toneDuration_ms);

    if (Serial.available())
    {
        command = Serial.readStringUntil('\n');
        Serial.println(command);

        if (command == "open")
        {
            myServo.write(130);
        }
        else if (command == "close")
        {
            myServo.write(105);
        }
        else if (command == "temperature")
        {
            readSurroundingTemperature();
        }
        else if (command == "forward")
        {
            analogWrite(leftMotorPin0, 50);
            analogWrite(leftMotorPin1, 0);
            analogWrite(rightMotorPin0, 0);
            analogWrite(rightMotorPin1, 35);
        }
        else if (command == "backward")
        {
            analogWrite(leftMotorPin0, 0);
            analogWrite(leftMotorPin1, 50);
            analogWrite(rightMotorPin0, 35);
            analogWrite(rightMotorPin1, 0);
        }
        else if (command == "left")
        {
            analogWrite(leftMotorPin0, 0);
            analogWrite(leftMotorPin1, 30);
            analogWrite(rightMotorPin0, 0);
            analogWrite(rightMotorPin1, 30);
        }
        else if (command == "right")
        {
            analogWrite(leftMotorPin0,30 );
            analogWrite(leftMotorPin1, 0);
            analogWrite(rightMotorPin0, 30);
            analogWrite(rightMotorPin1, 0);
        }
        else
        {
            analogWrite(leftMotorPin0, 0);
            analogWrite(leftMotorPin1, 0);
            analogWrite(rightMotorPin0, 0);
            analogWrite(rightMotorPin1, 0);
        }
    }
}

void readSurroundingTemperature()
{
    temperature_C = dht.readTemperature();
    if (isnan(temperature_C))
    {
        Serial.println("Unable to read temperatue!");
    }
    else
    {
        Serial.print("Temperature: ");
        Serial.print(String(temperature_C));
        Serial.println("*C");
        Serial.print("Distance: ");
        Serial.print(String(obstacleDistance_cm));
        Serial.println("cm");
    }
}

float calculateDistanceToObstacle()
{
    // send a ~10 us pulse to trigger the ultrasonic sensor
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(1);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);

    // read the duration of reflected pulse
    pulseDuration_us = pulseIn(echoPin, HIGH);

    // speed of sound in air is ~340m/s = 34000cm/s = 0.034cm/us
    return (pulseDuration_us > 0) ? 0.0340 * pulseDuration_us / 2 : (float)pulseDuration_us;
}
