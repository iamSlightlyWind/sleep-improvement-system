#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <Servo.h>

int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

SoftwareSerial bluetooth(0, 1);

int buttonNext = 10, buttonPrev = 9;
int potentiometer = A0;

String hour[] = {"00", "00"}; // 0 for current, 1 for alarm, 2 for time from current to alarm
String minute[] = {"00", "00", "00"};

int current = 0, cycle = 0;

String btdata;
String serialdata;

void setup()
{
    pinMode(buttonNext, INPUT);
    digitalWrite(buttonNext, HIGH);
    pinMode(buttonPrev, INPUT);
    digitalWrite(buttonPrev, HIGH);

    lcd.begin(16, 2);
    Serial.begin(9600);
    bluetooth.begin(9600);
}

void loop()
{
    if (bluetooth.available() != 0)
    {
        btdata = bluetooth.readString();
        Serial.println(btdata);
        Serial.println("");
    }

    process();
}

void process()
{
    getInput();
    getTime();
    getCycle();
    updateTime();
    formatTime();
    alarmControl();

    print();

    delay(200);
}

bool alarmSet = false;

void alarmControl()
{
    if (current == 4)
    {
        if (!alarmSet)
        {
            alarmSet = true;
            // toAlarmMin = hour[2].toInt() * 60 + minute[2].toInt();
        }
    }
}

unsigned long previousMillis = 0;

void updateTime()
{
    if (millis() - previousMillis >= 60000)
    {
        previousMillis = millis();
        int minuteValue = (minute[0].toInt() + 1) % 60;
        if (minuteValue == 0) hour[0] = String((hour[0].toInt() + 1) % 24);
        minute[0] = String(minuteValue);
    }
}

void getInput()
{
    if (digitalRead(buttonNext) == LOW && current < 4) current++;
    if (digitalRead(buttonPrev) == LOW && current > 0) current--;
    delay(50);
}

int porentiometerMax = 965;
int porentiometerMin = 180;

void getTime()
{
    Serial.println(String(analogRead(potentiometer)));

    switch (current)
    {
    case 0:
        hour[0] = String(map(analogRead(potentiometer), porentiometerMin, porentiometerMax, 0, 24));
        break;
    case 1:
        minute[0] = String(map(analogRead(potentiometer), porentiometerMin, porentiometerMax, 0, 60));
        break;
    case 2:
        hour[1] = String(map(analogRead(potentiometer), porentiometerMin, porentiometerMax, 0, 24));
        break;
    case 3:
        minute[1] = String(map(analogRead(potentiometer), porentiometerMin, porentiometerMax, 0, 60));
        break;
    default:
        break;
    }
}

void getCycle()
{
    int currentTime = hour[0].toInt() * 60 + minute[0].toInt();
    int alarmTime = hour[1].toInt() * 60 + minute[1].toInt();
    int toAlarmTime = currentTime > alarmTime ? 1440 - currentTime + alarmTime : alarmTime - currentTime;

    minute[2] = String(toAlarmTime);
    cycle = toAlarmTime / 90;
}

void formatTime()
{
    for (int i = 0; i <= 2; i++)
    {
        hour[i] = hour[i].length() < 2 ? "0" + String(hour[i]) : hour[i];
        minute[i] = minute[i].length() < 2 ? "0" + String(minute[i]) : minute[i];
    }
}

long lastBlink = 0;
int startLocation[] = {2, 5, 10, 13};

bool isBlinking = false;
unsigned long lastBlinkTime = 0;
const unsigned long blinkDuration = 1000; // blink duration in milliseconds

void blink()
{
    unsigned long currentTime = millis();
    if (currentTime - lastBlinkTime > blinkDuration)
    {
        isBlinking = !isBlinking; // toggle blink status
        lastBlinkTime = currentTime;
    }

    if (isBlinking && current < 4)
    {
        lcd.setCursor(startLocation[current], 0);
        lcd.print("  ");
    }
}

void print()
{
    String line1 = "C:" + String(hour[0]) + ":" + String(minute[0]) + " A:" + String(hour[1]) + ":" + String(minute[1]);
    String line2 = "U:" + String(minute[2]);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);

    blink();
}