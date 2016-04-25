#include "ofApp.h"
#include "ofxTweenzor.h"

//--------------------------------------------------------------
void ofApp::setup() {
  
  // window
  ofSetWindowTitle("solitude");
  ofBackground(0, 0, 0);
  ofSetFrameRate(60);
  ofSetVerticalSync(true);
  ofDisableSmoothing();
  // ofSetLogLevel(OF_LOG_VERBOSE);
  
  // setup tweening lib
  Tweenzor::init();
  
  // setup midi
  midiIn.listPorts();
  midiIn.openPort(MIDI_PORT);
  midiIn.addListener(this);
  
  // setup syphon
  syphonServer.setName("solitude");
  
  // setup ableton
  live.setup(this, &ofApp::liveLoaded);
  
  // get arduino address
  arduinoIdentifier = "";
  serialDeviceList = serial.getDeviceList();
  for(auto &device: serialDeviceList) {
    cout << device.getDeviceName() << endl << device.getDeviceID() << endl << device.getDevicePath() << endl;
    if(device.getDeviceName().find("tty.usbmodem") != string::npos) {
      arduinoIdentifier = device.getDevicePath();
    }
  }
  
  // connect to arduino
  // ---
  // replace the string below with the serial port for your Arduino board
  // you can get this from the Arduino application or via command line
  // for OSX, in your terminal type "ls /dev/tty.*" to get a list of serial devices
  arduino.connect(arduinoIdentifier, 57600);
  
  // listen for EInitialized notification. this indicates that
  // the arduino is ready to receive commands and it is safe to
  // call setupArduino()
  //
  // fix: https://forum.openframeworks.cc/t/einitialized-never-fired-with-ofarduino/15704/9
  // for listener not getting called
  ofAddListener(arduino.EInitialized, this, &ofApp::arduinoLoaded);
  
  // structure
  ledTubes.push_back(LedTube(800.0, 570.0, MIDI_CONTROL_1));
  ledTubes.push_back(LedTube(475.0, 500.0, MIDI_CONTROL_2));
  ledTubes.push_back(LedTube(600.0, 300.0, MIDI_CONTROL_3));
  ledTubes.push_back(LedTube(740.0, 125.0, MIDI_CONTROL_4));
  ledTubes.push_back(LedTube(450.0,  40.0, MIDI_CONTROL_5));
  
  for(auto &ledTube: ledTubes) {
    ledTube.setAccentColor(ofColor(255, 0, 0));
  }

  const float EFFECT_W_OFFSET = 85;
  const float EFFECT_H_OFFSET = 118;
  
  effects.push_back(Effect(this, EFFECT_TYPE_HISS,    800.0 + EFFECT_W_OFFSET, 570.0 + EFFECT_H_OFFSET, 10.0, 2.0, &ledTubes[0]));
  effects.push_back(Effect(this, EFFECT_TYPE_VOCODER, 475.0 + EFFECT_W_OFFSET, 500.0 + EFFECT_H_OFFSET, 10.0, 2.0, &ledTubes[1]));
  effects.push_back(Effect(this, EFFECT_TYPE_LOWPASS, 600.0 + EFFECT_W_OFFSET, 300.0 + EFFECT_H_OFFSET, 10.0, 2.0, &ledTubes[2]));
  effects.push_back(Effect(this, EFFECT_TYPE_REVERB,  740.0 + EFFECT_W_OFFSET, 125.0 + EFFECT_H_OFFSET, 10.0, 2.0, &ledTubes[3]));
  effects.push_back(Effect(this, EFFECT_TYPE_CHOP,    450.0 + EFFECT_W_OFFSET, 40.0  + EFFECT_H_OFFSET, 10.0, 2.0, &ledTubes[4]));
  
  const float SENSOR_W_OFFSET = 118;
  const float SENSOR_H_OFFSET = 100;
  
  sensors.push_back(Sensor(800.0 + SENSOR_W_OFFSET, 570.0 + SENSOR_H_OFFSET, 2, 1, &effects[0]));
  sensors.push_back(Sensor(475.0 + SENSOR_W_OFFSET, 500.0 + SENSOR_H_OFFSET, 4, 2, &effects[1]));
  sensors.push_back(Sensor(600.0 + SENSOR_W_OFFSET, 300.0 + SENSOR_H_OFFSET, 7, 3, &effects[2]));
  sensors.push_back(Sensor(740.0 + SENSOR_W_OFFSET, 125.0 + SENSOR_H_OFFSET, 8, 4, &effects[3]));
  sensors.push_back(Sensor(450.0 + SENSOR_W_OFFSET, 40.0 + SENSOR_H_OFFSET, 12, 5, &effects[4]));
}

//--------------------------------------------------------------
void ofApp::liveLoaded() {
  cout << "Ableton Live loaded." << endl;
  connectedToAbleton = true;
  
  // reset all effects once
  for(auto &effect: effects) {
    effect.resetEffect();
  }
}

//--------------------------------------------------------------
void ofApp::arduinoLoaded(const int & version) {
  
  // remove listener because we don't need it anymore
  ofRemoveListener(arduino.EInitialized, this, &ofApp::arduinoLoaded);
  
  // Note: pins A0 - A5 can be used as digital input and output.
  // Refer to them as pins 14 - 19 if using StandardFirmata from Arduino 1.0.
  // If using Arduino 0022 or older, then use 16 - 21.
  // Firmata pin numbering changed in version 2.3 (which is included in Arduino 1.0)
  
  // set pins we accept input from
  for(auto &sensor: sensors) {
    arduino.sendDigitalPinMode(sensor.pin, ARD_INPUT);
  }
  
  // Listen for changes on the digital and analog pins
  ofAddListener(arduino.EDigitalPinChanged, this, &ofApp::digitalPinChanged);
  
  // print firmware name and version to the console
  cout << arduino.getFirmwareName() << endl;
  cout << "Firmata v" << arduino.getMajorFirmwareVersion() << "." << arduino.getMinorFirmwareVersion() << endl;
  
  // set statys
  connectedToArduino = true;
}

//--------------------------------------------------------------
void ofApp::update() {
  
  // update tweening lib
  Tweenzor::update(ofGetElapsedTimeMillis());
  
 
  // update the arduino, get any data or messages.
  // the call to arduino.update() is required
  arduino.update();

  // give app some time to breath
  if(ofGetElapsedTimeMillis() > 1000) {
 
    // update live
    live.update();
    
    // update led tubes, sensors, effects
    for(auto &ledTube: ledTubes) ledTube.update();
    for(auto &sensor: sensors) sensor.update();
    for(auto &effect: effects) effect.update();
  }
}

//--------------------------------------------------------------
void ofApp::draw() {

  // draw led tubes
  for(auto &ledTube: ledTubes) ledTube.draw();
  
  // publish screen to syphon
  syphonServer.publishScreen();
  
  // ----------
  // everything AFTER this line won't get sent over syphon
  // ----------
  
  // draw debug
  if(shouldDrawDebug)
    this->drawDebug();
}

//--------------------------------------------------------------
void ofApp::drawDebug() {
  
  // draw statusses
  if(!connectedToAbleton) {
    ofPushStyle();
    ofFill();
    ofSetColor(255, 0, 0);
    ofDrawRectangle(ofGetWidth() - 70.0, 4.0, 65.0, 16.0);
    ofSetColor(255);
    ofDrawBitmapString("Ableton", ofGetWidth() - 65.0, 16.0);
    ofPopStyle();
  }

  if(!connectedToArduino) {
    ofPushStyle();
    ofFill();
    ofSetColor(255, 0, 0);
    ofDrawRectangle(ofGetWidth() - 70.0, 24.0, 65.0, 16.0);
    ofSetColor(255);
    ofDrawBitmapString("Arduino", ofGetWidth() - 65.0, 36.0);
    ofPopStyle();
  }
  
  // list arduino devices
  ofPushStyle();
  int deviceIndex = 0;
  ofDrawBitmapString("Detected serial devices: ", 20, 260.0);
  for(auto &device: serialDeviceList){
    ofSetColor(device.getDevicePath().compare(arduinoIdentifier) == 0 ? 255 : 75);
    ofDrawBitmapString(device.getDevicePath(), 20, 285.0 + (deviceIndex * 20.0));
    deviceIndex++;
  }
  ofPopStyle();
    
  // draw led tubes debug info, sensors, effects
  for(auto &ledTube: ledTubes) ledTube.drawDebug();
  for(auto &sensor: sensors) sensor.draw();
  for(auto &effect: effects) effect.draw();
  
  // draw midi debug
  ofSetColor(75);
  
  const float MAX_BAR_LENGTH = 150.0;
  const float MAX_BAR_HEIGHT = 10.0;
  
  // draw the last recieved message contents to the screen
  text << "Received: " << ofxMidiMessage::getStatusString(midiMessage.status);
  ofDrawBitmapString(text.str(), 20, 20);
  text.str(""); // clear
  
  text << "channel: " << midiMessage.channel;
  ofDrawBitmapString(text.str(), 20, 34);
  text.str(""); // clear
  
  text << "pitch: " << midiMessage.pitch;
  ofDrawBitmapString(text.str(), 20, 48);
  text.str(""); // clear
  ofDrawRectangle(20, 58, ofMap(midiMessage.pitch, 0, 127, 0, MAX_BAR_LENGTH), MAX_BAR_HEIGHT);
  
  text << "velocity: " << midiMessage.velocity;
  ofDrawBitmapString(text.str(), 20, 86);
  text.str(""); // clear
  ofDrawRectangle(20, 95, ofMap(midiMessage.velocity, 0, 127, 0, MAX_BAR_LENGTH), MAX_BAR_HEIGHT);
  
  text << "control: " << midiMessage.control;
  ofDrawBitmapString(text.str(), 20, 124);
  text.str(""); // clear
  ofDrawRectangle(20, 134, ofMap(midiMessage.control, 0, 127, 0, MAX_BAR_LENGTH), MAX_BAR_HEIGHT);
  
  text << "value: " << midiMessage.value;
  ofDrawBitmapString(text.str(), 20, 162);
  text.str(""); // clear
  if(midiMessage.status == MIDI_PITCH_BEND) {
    ofDrawRectangle(20, 172, ofMap(midiMessage.value, 0, MIDI_MAX_BEND, 0, MAX_BAR_LENGTH), MAX_BAR_HEIGHT);
  }
  else {
    ofDrawRectangle(20, 172, ofMap(midiMessage.value, 0, 127, 0, MAX_BAR_LENGTH), MAX_BAR_HEIGHT);
  }
  
  text << "delta: " << midiMessage.deltatime;
  ofDrawBitmapString(text.str(), 20, 200);
  text.str(""); // clear
}

//--------------------------------------------------------------
void ofApp::exit() {

  midiIn.closePort();
	midiIn.removeListener(this);
  
  arduino.disconnect();
}

//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage& message) {

	// make a copy of the latest message
	midiMessage = message;
  
  // find led tube that matches this midi note
  auto ledTube = find_if(ledTubes.begin(), ledTubes.end(), [message] (const LedTube& ledTube) {
    return ledTube.trigger == message.control;
  });
  
  // found a tube that matches?
  if (ledTube != ledTubes.end()) {

    // set intensity of the tube to the velocity of the note
    if(message.status == MIDI_CONTROL_CHANGE && !ledTube->overruled) {
      ledTube->setIntensity(ofMap(message.value, 0, 100, 0.0, 1.0));
    }
    
  } else {
    
    // not recognized
    cout << "Received Midi Note we don't recognize: " << message.pitch << endl;
  }
}

//--------------------------------------------------------------
void ofApp::digitalPinChanged(const int & pinNum) {
  
  // find sensor connected to this pin
  auto sensor = find_if(sensors.begin(), sensors.end(), [pinNum] (const Sensor & sensor) {
    return sensor.pin == pinNum;
  });
  
  // check if the pin is linked to a valid sensor
  if (sensor != sensors.end()) {
    
    // the value for the digital pin
    bool value = arduino.getDigital(pinNum);
    sensor->activate(value);
    
  } else {

    // pin change not recognized
    cout << "Pin Change we don't recognize: " << pinNum << endl;
  }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
  for(auto &sensor: sensors) {
    if(sensor.numPad == key - 48)
      sensor.activate(true);
  }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
  switch(key) {
    case 'l':
      midiIn.listPorts();
      break;
    case 'k':
      live.printTracks();
      break;
    case 'd':
      shouldDrawDebug = !shouldDrawDebug;
      break;
  }
  
  for(auto &sensor: sensors) {
    if(sensor.numPad == key - 48)
      sensor.activate(false);
  }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseReleased() {
}
