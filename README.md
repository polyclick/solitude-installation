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

![fritzing diagram](https://raw.githubusercontent.com/polyclick/solitude-installation/master/readme-assets/fritzed_bb.png)


# openframeworks output

![OpenFrameworks output](https://raw.github.com/polyclick/solitude/master/readme-assets/openframeworks.png)
