#pragma once

#include "ofMain.h"
#include "Enums.h"

class LedTube {
public:
  LedTube(float x, float y, int trigger);
  ~LedTube();
  
  void setup();
  void update();
  void draw();
  void drawDebug();
  
  void setIntensity(float value);
  float getIntensity();
  
  void setAccentColor(ofColor value);
  void setNoiseRate(float value);

  bool overruled;
  
  int trigger;
  
  ofColor color;
  
private:
  const float DEFAULT_WIDTH = 100.0;    // width
  const float DEFAULT_HEIGHT = 100.0;   // height
  const float SMOOTHING_TIME = 0.025f;  // smoothing between instant intensity switches
  
  float intensity;
  float _noiseRate;
  
  ofRectangle boundingBox;
  
  ofColor _accentColor;
};