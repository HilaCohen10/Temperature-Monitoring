
## Temperature Monitoring Project
* The project is implemented on FreeRTOS using C++
### This project is an implementation of a temperature monitoring sensor.
Link to video of demonstration: https://drive.google.com/file/d/1wWYfHHxN_ZlazU8_9vSsBEedna9RX77u/view?usp=sharing
#### components:
+ STM32 Nucleo 64
+ SDCard
+ Buzzer
+ Momentary button
+ Red led
+ RTC
+ Temperature Sensor - DHT11
#### What Does it do?
+ The sensor constantly check (1s) the current temperature 
+ The sensor save measured temperature every 1 minute in file log.txt
+ It has 2 thresholds: warning and critical temperature thresholds.
+ It has a hysteresis parameter to prevent bouncing around the threshold levels.
+ When the temperature increases above the warning threshold it should switch on the red LED and write the event into the events log in file.
+ When the temperature increases above the critical threshold it should start blink the red LED, give constant sound signal, and write the event into the events log in file
+ When the temperature decreases the threshold, it should disable corresponding notifications: LED, sound, and write the event into the event log in file.
+ The log record should have the following data: current date and time, the log severity, and the message
+ The user could disable the buzzer signal by pressing the button
+ The use should be able to perform the following commands:
  - Set date and time
  - Set warning threshold
  - Set critical threshold
  - Print log
  - Clear log
+ The monitor sensor after boot should use latest settings (store them in flash)
+ Files for event and temperature log shall be stored on SDCARD.
+ It is possible to remove the SD card and read the logs in windows (just by putting the SD card in a computer). Use FATFS for that purpose.

Relevant information I've used to add metods to write, print and clear files from SD-Card : https://01001000.xyz/2020-08-09-Tutorial-STM32CubeIDE-SD-card/
