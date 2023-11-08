#include <LiquidCrystal.h>
#include <Servo.h>
#include <SoftwareSerial.h>

int current = 0, cycle = 0, remain = 0, wpStart = 0;
int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
int startLocation[] = {2, 5, 10, 13};
int buttonNext = 10, buttonPrev = 9;
int porentiometerMax = 970;
int porentiometerMin = 180;
int potentiometer = A0;

boolean wp = false;
bool isBlinking = false;
boolean alarmSet = false;
boolean actionSet = false;

long lastBlink = 0;
unsigned long lastBlinkTime = 0;
const unsigned long blinkDuration = 1000;  // blink duration in milliseconds

unsigned long previousMillis = 0;

String hour[] = {
    "00",
    "00"};  // 0 for current, 1 for alarm, 2 for time from current to alarm
String minute[] = {"00", "00", "00"};
String btdata;
String serialdata;
String line[2];

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
SoftwareSerial bluetooth(0, 1);

void setup() {
    pinMode(buttonNext, INPUT);
    digitalWrite(buttonNext, HIGH);
    pinMode(buttonPrev, INPUT);
    digitalWrite(buttonPrev, HIGH);

    lcd.begin(16, 2);
    Serial.begin(9600);
    bluetooth.begin(9600);

    delay(1000);
    Serial.println("2");
}

void loop() {
    if (bluetooth.available() != 0) {
        btdata = bluetooth.readString();
        Serial.println(btdata);
        Serial.println("");
    }

    process();
}

void process() {
    getInput();
    getTime();
    getCycle();
    updateTime();
    formatTime();
    alarmControl();

    print();

    delay(200);
}

void updateTime() {
    if (millis() - previousMillis >= 60000) {
        previousMillis = millis();
        int minuteValue = (minute[0].toInt() + 1) % 60;
        if (minuteValue == 0) hour[0] = String((hour[0].toInt() + 1) % 24);
        minute[0] = String(minuteValue);
    }
}

void getInput() {
    if (digitalRead(buttonNext) == LOW && current < 4) current++;
    if (digitalRead(buttonPrev) == LOW && current > 0) current--;
    delay(50);
}

void getTime() {
    //Serial.println(String(analogRead(potentiometer)));

    switch (current) {
        case 0:
            hour[0] = String(map(analogRead(potentiometer), porentiometerMin,
                                 porentiometerMax, 0, 24));
            break;
        case 1:
            minute[0] = String(map(analogRead(potentiometer), porentiometerMin,
                                   porentiometerMax, 0, 60));
            break;
        case 2:
            hour[1] = String(map(analogRead(potentiometer), porentiometerMin,
                                 porentiometerMax, 0, 24));
            break;
        case 3:
            minute[1] = String(map(analogRead(potentiometer), porentiometerMin,
                                   porentiometerMax, 0, 60));
            break;
        default:
            break;
    }
}

void getCycle() {
    int currentTime = hour[0].toInt() * 60 + minute[0].toInt();
    int alarmTime = hour[1].toInt() * 60 + minute[1].toInt();
    int toAlarmTime = currentTime > alarmTime ? 1440 - currentTime + alarmTime
                                              : alarmTime - currentTime;

    minute[2] = String(toAlarmTime);
    remain = toAlarmTime % 90;
    cycle = toAlarmTime / 90;
}

void formatTime() {
    for (int i = 0; i <= 2; i++) {
        hour[i] = hour[i].length() < 2 ? "0" + String(hour[i]) : hour[i];
        minute[i] =
            minute[i].length() < 2 ? "0" + String(minute[i]) : minute[i];
    }
}

void blink() {
    unsigned long currentTime = millis();
    if (currentTime - lastBlinkTime > blinkDuration) {
        isBlinking = !isBlinking;  // toggle blink status
        lastBlinkTime = currentTime;
    }

    if (isBlinking && current < 4) {
        lcd.setCursor(startLocation[current], 0);
        lcd.print("  ");
    }
}

void print() {
    line[0] = "C:" + String(hour[0]) + ":" + String(minute[0]) +
              " A:" + String(hour[1]) + ":" + String(minute[1]);
    line[1] = "C:" + String(cycle) + " WP:" + String(wpStart) +
              " R:" + String(remain);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(line[0]);
    lcd.setCursor(0, 1);
    lcd.print(line[1]);

    blink();
}

void alarmControl() {
    if (current == 4) {
        if (alarmSet) {
            if (wp) {
                if (minute[2].toInt() == wpStart) {
                    Serial.println("1");
                    current = 3;
                }
            } else {
                if (minute[2].toInt() == 0) {
                    Serial.println("2");
                    current = 3;
                }
            }
        } else {
            if (!actionSet) {
                if (cycle >= 1) {
                    wp = true;
                    if (remain < 30)
                        wpStart = (cycle * 90) - 15;
                    else if (remain > 30)
                        wpStart = getWpChoice();
                    else if (remain == 0)
                        wpStart = (cycle * 90) - 30;
                } else if (cycle == 0)
                    wp = false;
                actionSet = true;

                Serial.println("0"); //send do not disturb
            }
            delay(500);
        }
    } else {
        alarmSet = false;
        actionSet = false;
    }
}

int getWpChoice() {
    int choiceMade = 0;
    line[0] = "Up at last cycle?";
    line[1] = "  Other     Yes";

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(line[0]);
    lcd.setCursor(0, 1);
    lcd.print(line[1]);

    delay(500);

    while (choiceMade == 0) {
        if (digitalRead(buttonNext) == LOW) return (cycle * 90) - 15;
        if (digitalRead(buttonPrev) == LOW) choiceMade = 2;
        delay(50);
    }

    line[0] = "Up at alarm time?";
    line[1] = "  No        Yes";

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(line[0]);
    lcd.setCursor(0, 1);
    lcd.print(line[1]);

    delay(500);

    if (choiceMade == 2) {
        while (choiceMade == 2) {
            if (digitalRead(buttonNext) == LOW) return minute[2].toInt() - 30;
            if (digitalRead(buttonPrev) == LOW) current = 3;
            choiceMade = 3;
            delay(50);
        }
    }

    return 0;
}