## Smart sleep/wakeup system

- Set alarm sequence:
  + Calculate sleep cycle (offset to sleep time, 90 mins per cycle)
  + Set phone to Do Not Disturb mode

- Wake up sequence:
  + Gradually increase the brightness of the screen (30 mins before wake up)
  + Then play natural sounds (birds, waves, etc)
  + Then let the alarm go off after 10 mins of natural sounds

- Optional:
  + Use temperature sensor to read the temperature of the room, then adjust the AC accordingly
  + Use ultrasound sensor to detect if the user woke up before the alarm went off, then reset sleep cycle

## Hardware

- Required:
  + Arduino Uno R3
  + 16x2 LCD
  + Buttons
  + Bluetooth HC-05
  + Continuous Rotation Servo
  + Potentiometer

- Optional:
  + Temperature sensor
  + Ultrasound sensor
  + IR sensor
  + IR blaster

## Challenges

- [ ] Get current time
- [ ] Play audio on phone
- [ ] Set phone to Do Not Disturb mode
- [ ] Control servo from far away