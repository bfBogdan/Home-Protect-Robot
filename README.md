# Home Protect Robot
Replace all Carbon Monoxide (CO) sensors, Gas sensors, and flood detectors with a single robot.

## Smart features
- As an extra layer of security, Home Protect can announce you if it detects Carbon Monoxide, Butane Gas, or even flood, thanks to a Piezo Buzzer, and, if you are not at home, it can announce you through SMS notification.
- The main advantage of this project is that if you would want to protect all your house with those particular detectors, you would need to buy one of them in every room, but, with Home Protect, you can have just one sensor that moves in your house, avoiding all the obstacles.
- At the same time, it is capable of sending you an SMS if the battery is too low thanks to a voltage sensor module.

## Parts used
- Arduino UNO
- MQ2 (Gas sensor (Butane))
- MQ7 (Carbon Monoxide sensor)
- Servo Motor
- Ultrasonic Sensor (HC-SR07)
- Arduino flood sensor
- Piezo Buzzer
- Breadboard
- 11.1v 2200 mAh LiPo Battery
- 2 DC motors
- SIM800A GSM module
- Dual H-Bridge (L298N)
- Voltage sensor module
- 3.3v, 1A Step-Down Pololu Source
- ON/OFF button
- Wires <br>
Is not necessary to use the same parts, those are the ones I used.

## Libraries needed
- `Servo.h` library ( probably preinstalled on your Arduino IDE )
- `SoftwareSerial.h` library ( probably preinstalled on your Arduino IDE )

-------------
Developed by Bogdan Barbu.
