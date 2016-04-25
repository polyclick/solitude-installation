#pragma once

#include "ofMain.h"
#include "Enums.h"
#include "LedTube.h"

enum EffectType {
  EFFECT_TYPE_HISS = 0,
  EFFECT_TYPE_REVERB = 1,
  EFFECT_TYPE_CHOP = 2,
  EFFECT_TYPE_VOCODER = 3,
  EFFECT_TYPE_LOWPASS = 4
};

class ofApp;

class Effect {
  
public:
  Effect();
  Effect(ofApp *app, EffectType effectType, float x, float y, float attackTime, float releaseTime,  LedTube *ledTube);
  ~Effect();
  
  void setup();
  void update();
  void draw();
  
  void attack();
  void release();
  
  void resetEffect();
  
  bool getIsActive();
  
  EffectType effectType;
  
private:
  void activate();
  void deactivate();
  void activateActions();
  void deactivateActions();
  
  void updateEffect();
  
  float getValueForTrackA(float min, float max, float dryWet);
  float getValueForTrackB(float min, float max, float dryWet);
  
  const float DEFAULT_SIZE = 40.0;
  
  bool active;
  int direction; // -1: going down, 0: steady, 1: going up
  
  float attackTime = 5.0;
  float releaseTime = 10.0;
  float dryWet = 0.0;
  float lastDryWet = 0.0;
  
  ofRectangle boundingBox;
  
  ofApp *app;
  
  LedTube *ledTube;
};