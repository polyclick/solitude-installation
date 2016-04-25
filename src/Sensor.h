#pragma once

#include "ofMain.h"
#include "Effect.h"

class Sensor {

public:
  Sensor();
  Sensor(float x, float y, int pin, int numPad, Effect *effect);
  ~Sensor();
  
  void setup();
  void update();
  void draw();
  
  void activate(bool value);
  bool isActive();
  
  int pin;
  int numPad;
  
  Effect *effect;
  
private:
  const float DEFAULT_SIZE = 10.0;
  
  bool active;
  
  ofRectangle boundingBox;
};