Fireball: custom dice ball thing powered by Arduino

We use the [Adafruit LSM303 library](https://github.com/adafruit/Adafruit_LSM303) for reading information from the accelerometer.

To run:

1. Download this repository.

2. Connect the right pins!  SCL > SCL and SDA > SDA.  For an Uno, these are pins A5 and A4, respectively.  For a Pro Micro, they are pins 3 and 2.  (Check out [this link] (http://arduino.cc/en/Reference/Wire) for info for other boards.)

3. Add the folder Adafruit_LSM303 as a library in your Arduino environment. (Sketch > Import Library > Add Library)

4. Upload and run!
