#include <Stepper.h>

#define LED 2
#define TRIG 3
#define ECHO 4

const uint16_t stepsPerRevolution = 2038;
Stepper stepperMotor = Stepper(stepsPerRevolution, 8, 10, 9, 11);  // IN1, IN3, IN2, IN4

void setup() {
    Serial.begin(9600);
    pinMode(LED, OUTPUT);
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
}


void ultrasonicSensorMeasurement() {
    digitalWrite(TRIG, LOW);
    delay(150);
    digitalWrite(TRIG, HIGH);
    delay(300);
    digitalWrite(TRIG, LOW);
    double t = pulseIn(ECHO, HIGH);
    double distance = t * 0.017015;
    Serial.print("distance: ");
    Serial.println(distance);
    if (distance < 7.8)   // there's garbage in the way, therefore the can is full
        digitalWrite(LED, HIGH);
    else
        digitalWrite(LED, LOW);
}

bool opened = false;

void irSensorAndStepper() {
    stepperMotor.setSpeed(15);
    uint16_t irOutput = analogRead(A0);
    Serial.print("irOutput: ");
    Serial.println(irOutput);
    delay(100);
    if (opened == false && irOutput > 630) {   // the can's lid opens
        stepperMotor.step(7000);
        opened = true;
        delay(1500);
    }
    else if (opened == true && irOutput <= 440) {   // the can's lid closes
        stepperMotor.step(-7000);
        opened = false;
    }
    else {}   // the can's lid doesn't move
}


void loop() {
    ultrasonicSensorMeasurement();
    irSensorAndStepper();
}