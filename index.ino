#include <PID_v1.h>
#include <max6675.h>

#define tc 2
#define outPin 3

double setpoint, input, output;
double Kp = 2, Ki = 5, Kd = 1;

int thermoDO = 4;
int thermoCS = 5;
int thermoCLK = 6;

PID myPID(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

void setup()
{
    Serial.begin(9600);
    pinMode(tc, INPUT);
    pinMode(output, OUTPUT);

    setpoint = 100;
    myPID.SetMode(AUTOMATIC);

    Serial.println("MAX6675 test");
    // wait for MAX chip to stabilize
    delay(500);
}

void loop()
{
    input = (double)thermocouple.readCelsius();
    // Serial.println(input);
    myPID.Compute();
    Serial.print("input:");
    Serial.println(input);
    Serial.print("output:");
    Serial.println(output);

    analogWrite(outPin, output);
    delay(1000);
}