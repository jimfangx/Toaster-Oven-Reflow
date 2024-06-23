#include <PID_v1.h>
#include <max6675.h>

#define tc 2
#define outPin 3

double setpoint, input, output;
double Kp = 2, Ki = 5, Kd = 1;

int thermoDO = 4;
int thermoCS = 5;
int thermoCLK = 6;
int time;
int position = 0;
double states[] = {150, 150, 255, 0};
int newtime = 0;


int state = 0;

PID myPID(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

void setup()
{
    Serial.begin(9600);
    pinMode(tc, INPUT);
    pinMode(output, OUTPUT);


    setpoint = states[position];
    myPID.SetMode(AUTOMATIC);

    Serial.println("MAX6675 test");
    // wait for MAX chip to stabilize
    delay(500);
}

void loop()
{
    if (state == 1)
    {
       if (millis() - newtime > 60000)
       {
          state += 1;
          setpoint = states[state];
       }
    }
    else if (input >= states[state])
    {
       state += 1;
       setpoint = states[state];
       newtime = 0;
    }

    input = (double)thermocouple.readCelsius();
    
    
    myPID.Compute();

    //printing position, input, and output
    Serial.print(" position: ");
    Serial.print(setpoint);
    Serial.print(" input: ");
    Serial.print(input);
    Serial.print(" output: ");
    Serial.println(output);

    //write it out
    analogWrite(outPin, output);
    delay(1000);

    //Notes: 30 degrees past, it will turn on - 10 degrees celsius is not a big deal - ideas: we can build an offset by a constant
}
