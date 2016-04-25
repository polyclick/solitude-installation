#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxSyphon.h"
#include "ofxAbletonLive.h"
#include "Enums.h"
#include "LedTube.h"
#include "Sensor.h"
#include "Effect.h"

class ofApp : public ofBaseApp, public ofxMidiListener {
	
public:
	
	void setup();
	void update();
	void draw();
	void exit();
	
	void keyPressed(int key);
	void keyReleased(int key);
	
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased();

  bool connectedToArduino = false;
  bool connectedToAbleton = false;
  
  ofxAbletonLive live;
  
private:
  
  const int MIDI_PORT = 0;

  bool shouldDrawDebug = true;
  
  string arduinoIdentifier;
  
  vector<ofSerialDeviceInfo> serialDeviceList;   // all serial devices
  vector<LedTube> ledTubes;   // led tubes
  vector<Sensor> sensors;     // floor pressure sensors
  vector<Effect> effects;     // audiovisual effects
  
  ofSerial serial;
  ofArduino arduino;
  ofxSyphonServer syphonServer;
  ofxMidiIn midiIn;
  ofxMidiMessage midiMessage;
  
  stringstream text;

  void liveLoaded();
  void arduinoLoaded(const int & version);
  
  void drawDebug();
  
  void newMidiMessage(ofxMidiMessage & eventArgs);
  void digitalPinChanged(const int & pinNum);
};
