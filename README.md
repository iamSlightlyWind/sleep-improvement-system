## Smart sleep/wakeup system
- Set time process:
  + Use potentiometer to set time
  + Use buttons to set time category
  + Select choices with buttons based on cycle offset

- Set alarm sequence:
  + Calculate sleep cycle (90 mins per cycle)
  + Set phone to Do Not Disturb mode
  + Give user choices of when to wake up (based on time after last cycle):
    + < 30 mins: set Wake Up Sequence at 15 mins before last cycle
    + > 30 mins: set Wake Up Sequence at either 30 mins before alarm time or same as < 30 mins
    + None: set Wake Up Sequence at 30 mins before alarm time
    + No cycle: only set alarm at alarm time

- Wake up sequence:
  + Then play natural sounds (birds, waves, etc)
  + Gradually increase the volume of the sound (30 mins before wake up, 2% each to 60 percent max)
  + Then let the alarm go off at 100% volume after natural sounds

- Optional:
  + Use temperature sensor to read the temperature of the room, then adjust the AC accordingly
  + Use ultrasound sensor to detect if the user woke up before the alarm went off, then reset sleep cycle

## Hardware

- Required:
  + Arduino Uno R3
  + 16x2 LCD
  + Buttons
  + Bluetooth HC-05
  + Potentiometer

- Optional:
  + Continuous Rotation Servo
  + Temperature sensor
  + Ultrasound sensor
  + IR sensor
  + IR blaster

## Challenges

- [X] Play audio on phone
- [x] Set phone to Do Not Disturb mode
- [x] Update time in real time
- [x] Minimize Potentiometer noise
- [x] Blink
- [x] Minimize use of delay()
- [ ] Get current time using bluetooth