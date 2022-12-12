# Information for Freenove Spider model

## Servo Channels

Servo channels are assigned

- 0 .. 15 to PCA9685 0x41
- 16 .. 31 to PCA9685 0x40

Channel 0: Head left/right
Channel 1: Head up/down

Each leg consists of 3 servos
Servo 1: leg left/right
Servo 2: leg middle up/down
Servo 3: leg end up/down

Settings for legs stretched out
Leg1: left front
Servo1: #15, 90°
Servo2: #14, 90°
Servo3: #13, 0°

Leg2: left middle
Servo1: #12, 90°
Servo2: #11, 90°
Servo3: #10, 0°

Leg3: left back
Servo1: #9, 90°
Servo2: #8, 90°
Servo3: #31, 0°

Leg4: right front
Servo1: #16, 90°
Servo2: #17, 90°
Servo3: #18, 180°

Leg5: right middle
Servo1: #19, 90°
Servo2: #20, 90°
Servo3: #21, 180°

Leg6: right back
Servo1: #22, 90°
Servo2: #23, 90°
Servo3: #27, 180°

## Leg length & coordinates

Inner leg 33 mm
Middle leg  90 mm
Outer leg 110 mm


## ADC information

8 channels, 0 .. 5 V

Channel 0: Power2 (Battery * 1/3)  -> Servos
Channel 1: Power Leg5
Channel 2: Power Leg4
Channel 3: Power Leg6
Channel 4: Power1 (Battery * 1/3)  -> PI
Channel 5: Power Leg2
Channel 6: Power Leg1
Channel 7: Power Leg3

