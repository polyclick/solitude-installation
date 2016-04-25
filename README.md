# solitude-installation
OpenFrameworks source code for the solitude interactive light installation.

![solitude](https://raw.githubusercontent.com/polyclick/solitude-installation/master/readme-assets/solitude-laptop.jpg)


# prerequisites

* OSX
* Ableton 9
* Apple XCode
* Arduino Uno Board
* Arduino runtime environment
* LiveOSC installed and configured
* (MadMapper)


# how to run

* Upload StandardFirmata runtime to your Arduino board (*)
* Open Ableton Solitude project
* Make sure LiveOSC is set as a control surface for Ableton (**)
* Setup up a virtual IAC driver so Ableton can send midi over this virtual port to OpenFrameworks (***)
* Clone this git repo
* Open XCode project
* Run & Rejoice!

```
(*) more info at: http://www.instructables.com/id/Arduino-Installing-Standard-Firmata/
(**) more info at: http://livecontrol.q3f.org/ableton-liveapi/liveosc/
(***) more info at: https://www.ableton.com/en/help/article/using-virtual-MIDI-buses-live/
```

# setup scheme

![overview scheme](https://raw.githubusercontent.com/polyclick/solitude-installation/master/readme-assets/overview.png)


# fritzing diagram

Fritzing diagram for the Arduino electric circuit. Connect your sensors to digital pins: 2, 4, 7, 8, 12.
These are the pins that are pre-configured in the OpenFrameworks app to accept digital input.
If you like to change these; edit the file `ofApp.cpp`, 3rd parameter of the `Sensor` constructor.

```
sensors.push_back(Sensor(800.0 + SENSOR_W_OFFSET, 570.0 + SENSOR_H_OFFSET, 2, 1, &effects[0]));
sensors.push_back(Sensor(475.0 + SENSOR_W_OFFSET, 500.0 + SENSOR_H_OFFSET, 4, 2, &effects[1]));
sensors.push_back(Sensor(600.0 + SENSOR_W_OFFSET, 300.0 + SENSOR_H_OFFSET, 7, 3, &effects[2]));
sensors.push_back(Sensor(740.0 + SENSOR_W_OFFSET, 125.0 + SENSOR_H_OFFSET, 8, 4, &effects[3]));
sensors.push_back(Sensor(450.0 + SENSOR_W_OFFSET, 40.0 + SENSOR_H_OFFSET, 12, 5, &effects[4]));
___________________________________________________________________________^
```


![fritzing diagram](https://raw.githubusercontent.com/polyclick/solitude-installation/master/readme-assets/fritzed_bb.png)


# openframeworks output

![OpenFrameworks output](https://raw.githubusercontent.com/polyclick/solitude-installation/master/readme-assets/openframeworks.png)
