# ToyotaOdometer-79c36-Program
Reprogram the 79C36 EEPROM to fix an odometer on a Toyota Matrix using an ESP32 in Arduino (should work fine with a 3.3V Arduino device as well, and possibly a 5V Arduino, but I do not know. Without looking into it, I would say that there is risk of damage if using a 5V device. (I now wish I had measured the voltage when it was connected in the car)

## My 2003 Toyota Matrix odometer hit 299999km, and for reasons I cannot comprehend, they stop there. I wrote this quickly to reset it to zero(ish).
I got the basics from here: https://youtu.be/8wygJwWnFm4

## IMPORTANT: It is unknown what the ramifications of this going badly are. If you're not willing to risk having to replace the instrument cluster, don't try this. These "instructions" are not comprehensive.

### Summary:
Due to a bug in some Toyota vehicles (and related ones, such as the Pontiac Vibe), the odometer stops at 299999 (here in Canada, it was at 299999km, but from what I have read, the same thing happens at 299999 miles in the US).

When I read the EEPROM on mine, it showed that the mileage was still being counted in the 74C36 EEPROM (I could see that mine was at 300040 or so), but it still only showed 299999 on the display. Note that the video I linked to above, and other discussions I have seen found the same thing as me - that all of the digits are easy to read/write except the ones digit. I did a bit of experimenting with it but was unable to figure it out in a reasonable amount of time, and discarded it as unimportant.

### Before:
![Odo299999](https://user-images.githubusercontent.com/608740/161405113-106e5c41-9eb8-4635-8669-a66ffab16d9e.jpg)

### After*:
![Odo0](https://user-images.githubusercontent.com/608740/161405115-3db48562-8a86-4bc3-afd0-f845368343ac.jpg)
* Because of not knowing how to program the "ones" digit, I cheated and drove the car until it was at "10" and then reprogrammed it back to "0" again

## Basic summary:
This requires an ESP32 devkit of some sort, and uses this library: https://github.com/0xJoey/Arduino_93C46 (Thanks for that, 0xJoey!).

To program it, I took out the instrument cluster and the black plastic panel off of the back of it, which revealed the 74C36 EEPROM (the only 8 pin DIP package on mine). I was planning to remove it and put a socket in, but getting at the back of the board was looking to be harder than I hoped, so I simply soldered some dupont wires onto the chip:
 ![IMG_20220402_164955](https://user-images.githubusercontent.com/608740/161405314-f7f973ae-b682-48bd-97fd-8dafed1c78e6.jpg)
 ![IMG_20220402_164934](https://user-images.githubusercontent.com/608740/161405573-add54ec6-532c-4ed8-b6f8-928cde41552a.jpg)

 See the arduino program for the pinout for the serial wires, ground and power are pins 5 and 8 respectively.
In the sketch, make sure that `bool doWrite = false;` is indeed set to `false` for testing. Once things are connected and you can read the device, you can set the desired odometer reading with `char newOdometer[] = "00000";` noting that it does not include the last digit (the "ones" digit).

Also not that this code was a very quick hack - I doubt that my little car will make it another 300000km, so I likely won't use it again, but I thought it good to document it at least a bit! Good luck, and don't brick your car. ;)
