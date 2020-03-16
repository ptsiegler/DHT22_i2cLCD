# DHT22_i2cLCD
Combined DHT22 read and i2c 20x4 lcd print

Edit the following to match your setup:
-> line 2 of DHT22_Driver.cpp for your DHT22 pin number (the BCM pin number, not wiringPi or GPIO number)
-> lcd i2c address is set in whatever the main piece of code is, see example

Usefull stuff:
-> gpio readall -> when run in terminal tells you which pins are what
-> it2cdetect -y 1 -> visual list of i2c devices

compile with "g++ [whateverYouCallYourMainThing].cpp -lwiringPi -o [whateverYouCallYourOut].out"
  then run [whateverYouCallYourOut].out
  
for example mine is "g++ example.cpp -lwiringPi -o example.out" and I run "./example.out"

Credit to monotok for original LCD/i2c driver
https://github.com/monotok/RaspLCDDriver

Credit to danichoi for original dht22 driver
https://github.com/danichoi737/DHT22
