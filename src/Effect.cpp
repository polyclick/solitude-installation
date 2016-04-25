#include "Effect.h"
#include "ofApp.h"
#include "ofxTweenzor.h"

Effect::Effect(){
}

Effect::Effect(ofApp *app, EffectType effectType, float x, float y, float attackTime, float releaseTime, LedTube *ledTube){
  this->app = app;
  this->effectType = effectType;
  this->boundingBox = ofRectangle(x, y, DEFAULT_SIZE, DEFAULT_SIZE);
  this->attackTime = attackTime;
  this->releaseTime = releaseTime;
  this->ledTube = ledTube;
  
  this->setup();
}

Effect::~Effect(){
}

void Effect::setup(){
  this->dryWet = 0.0;
  this->lastDryWet = 0.0;
  this->active = false;
}

void Effect::update(){

  // check if we are moving from a sleeping (0.0) state
  //    to a more active state (> 0.0)
  if(lastDryWet == 0.0 && dryWet > 0.0) {
    activate();
  } else if(lastDryWet > 0.0 && dryWet == 0.0) {
    deactivate();
  }
  
  // direction
  if (dryWet < lastDryWet) {
    direction = -1;
  } else if(dryWet > lastDryWet) {
    direction = 1;
  } else {
    direction = 0;
  }
  
  // update the effect to reflect the progress
  updateEffect();
  
  lastDryWet = dryWet;
}

void Effect::draw(){
  ofPushStyle();
  
  // bounding box width
  float halfWidth = boundingBox.width / 2.0;
  
  // always draw placeholder
  ofFill();
  ofSetColor(25);
  ofSetCircleResolution(360);
  ofDrawCircle(boundingBox.x + halfWidth, boundingBox.y + halfWidth, halfWidth);
  
  // draw effect when active
  if(dryWet > 0.0) {
    ofPath arc;
    arc.setCircleResolution(360);
    arc.setColor(ofColor(0, 0, 255));
    arc.arc(boundingBox.x + halfWidth, boundingBox.y + halfWidth, halfWidth, halfWidth, 0.0, 360.0 * dryWet);
    arc.draw();
  }
  
  // draw effect label
  dryWet > 0.0 ? ofSetColor(100, 100, 255) : ofSetColor(60.0);
  switch(effectType) {
    case EFFECT_TYPE_HISS: {
      ofDrawBitmapString("HISS", boundingBox.x - 90, boundingBox.y + 2);
      break;
    }
    case EFFECT_TYPE_REVERB: {
      ofDrawBitmapString("REVERB", boundingBox.x - 90, boundingBox.y + 2);
      break;
    }
    case EFFECT_TYPE_CHOP: {
      ofDrawBitmapString("CHOP", boundingBox.x - 90, boundingBox.y + 2);
      break;
    }
    case EFFECT_TYPE_VOCODER: {
      ofDrawBitmapString("VOCODER", boundingBox.x - 90, boundingBox.y + 2);
      break;
    }
    case EFFECT_TYPE_LOWPASS: {
      ofDrawBitmapString("LPF", boundingBox.x - 90, boundingBox.y + 2);
      break;
    }
  }
  
  ofPopStyle();
}

// everytime somebody steps close to our led tube (sensor activated)
void Effect::attack(){

  // animate to 1.0 over time
  // if we were already attacking, calculate remainer percentage to travel
  float percentage = 1.0 - (dryWet / 1.0);
  Tweenzor::add(&dryWet, dryWet, 1.f, 0.f, attackTime * percentage);
  
  this->ledTube->overruled = true;
}

// every time someone leaves our led tube (sensor off)
void Effect::release(){
  
  // always keep same release time, we don't care where we are
  Tweenzor::add(&dryWet, dryWet, 0.f, 0.f, releaseTime);
  
  this->ledTube->overruled = false;
}

void Effect::activate() {
  if(active) return;
  active = true;
  activateActions();
}

void Effect::deactivate() {
  if(!active) return;
  active = false;
  deactivateActions();
}

bool Effect::getIsActive() {
  return active;
}

void Effect::activateActions() {
  
  // only manipulate ableton when connected
  if(!app->connectedToAbleton) return;
  
  switch(effectType) {
    case EFFECT_TYPE_HISS: {
      
      app->live.getTrack("FX-HISS")->getDevice(0)->printParameters();
      app->live.getTrack("FX-HISS")->getDevice(1)->printParameters();
      
      app->live.getTrack("FX-HISS")->getDevice(0)->setEnabled(true);
      app->live.getTrack("FX-HISS")->getDevice(1)->setEnabled(true);
      
      break;
    }
    case EFFECT_TYPE_REVERB: {

      app->live.getTrack("FX-REVERB")->getDevice(0)->printParameters();
      app->live.getTrack("FX-REVERB")->getDevice(1)->printParameters();

      app->live.getTrack("FX-REVERB")->getDevice(0)->setEnabled(true);
      app->live.getTrack("FX-REVERB")->getDevice(1)->setEnabled(true);
      
      break;
    }
    case EFFECT_TYPE_CHOP: {

      app->live.getTrack("FX-CHOP")->getDevice(0)->printParameters();
      app->live.getTrack("FX-CHOP")->getDevice(1)->printParameters();

      app->live.getTrack("FX-CHOP")->getDevice(0)->setEnabled(true);
      app->live.getTrack("FX-CHOP")->getDevice(1)->setEnabled(true);
      
      break;
    }
    case EFFECT_TYPE_VOCODER: {

      app->live.getTrack("FX-VOCODER")->getDevice(0)->printParameters();
      app->live.getTrack("FX-VOCODER")->getDevice(1)->printParameters();

      app->live.getTrack("FX-VOCODER")->getDevice(0)->setEnabled(true);
      app->live.getTrack("FX-VOCODER")->getDevice(1)->setEnabled(true);
      
      break;
    }
    case EFFECT_TYPE_LOWPASS: {
      
      app->live.getTrack("FX-LOWPASS")->getDevice(0)->printParameters();
      app->live.getTrack("FX-LOWPASS")->getDevice(1)->printParameters();

      app->live.getTrack("FX-LOWPASS")->getDevice(0)->setEnabled(true);
      app->live.getTrack("FX-LOWPASS")->getDevice(1)->setEnabled(true);
      
      break;
    }
  }
}

// here we can do stuff after the effect has fully degraded
// like cleaning up our effects, making sure everything is reset, ...
void Effect::deactivateActions() {
  
  // reset effect
  this->resetEffect();
  
  // ableton manipulations, when ableton available
  if(!app->connectedToAbleton) return;
  

  switch(effectType) {
    case EFFECT_TYPE_HISS: {
      break;
    }
    case EFFECT_TYPE_REVERB: {
      
      break;
    }
    case EFFECT_TYPE_CHOP: {
      
      break;
    }
    case EFFECT_TYPE_VOCODER: {
      
      break;
    }
    case EFFECT_TYPE_LOWPASS: {
      
      break;
    }
  }
}

void Effect::updateEffect() {
  
  // set tube noise intensity
  this->ledTube->setNoiseRate(1.0 - dryWet);
  
  // ableton manipulations, when ableton available
  if(!app->connectedToAbleton || !active) return;
  
  switch(effectType) {
    case EFFECT_TYPE_HISS: {
      
      app->live.getTrack("FX-HISS")
        ->getDevice("Crew Cut")
        ->getParameter("Dry/Wet")
        ->setValue(getValueForTrackA(0.0, 1.0, dryWet));
      
      app->live.getTrack("FX-HISS")
        ->getDevice("Hiss")
        ->getParameter("Amount")
        ->setValue(getValueForTrackB(0.0, 0.5, dryWet));
      
      break;
    }
    case EFFECT_TYPE_REVERB: {

      app->live.getTrack("FX-REVERB")
        ->getDevice("Drum Electrifier")
        ->getParameter("DryWet")
        ->setValue(getValueForTrackA(0.0, 1.0, dryWet));
      
      app->live.getTrack("FX-REVERB")
        ->getDevice("Warm Reverb Long")
        ->getParameter("Dry/Wet")
        ->setValue(getValueForTrackB(0.0, 1.0, dryWet));
      
      break;
    }
    case EFFECT_TYPE_CHOP: {
      
      app->live.getTrack("FX-CHOP")
        ->getDevice("Heavy")
        ->getParameter("Dry/Wet")
        ->setValue(getValueForTrackA(0.0, 1.0, dryWet));
      
      app->live.getTrack("FX-CHOP")
        ->getDevice("High Pass")
        ->getParameter("Frequency")
        ->setValue(getValueForTrackB(20.0, 35.0, dryWet));
      
      break;
    }
    case EFFECT_TYPE_VOCODER: {

      app->live.getTrack("FX-VOCODER")
        ->getDevice("Sharp Glass")
        ->getParameter("Dry/Wet")
        ->setValue(getValueForTrackA(0.0, 1.0, dryWet));    // 0% -> 100%
      
      app->live.getTrack("FX-VOCODER")
        ->getDevice("Chromatic")
        ->getParameter("Dry/Wet")
        ->setValue(getValueForTrackB(0.0, 0.5, dryWet));    // 0% -> 50%
      
      break;
    }
    case EFFECT_TYPE_LOWPASS: {

      app->live.getTrack("FX-LOWPASS")
        ->getDevice("Distort")
        ->getParameter("Dry/Wet")
        ->setValue(getValueForTrackA(0.0, 100.0, dryWet));    // 0% -> 100%
      
      app->live.getTrack("FX-LOWPASS")
        ->getDevice("Auto Filter")
        ->getParameter("Frequency")
        ->setValue(getValueForTrackB(135.0, 110.0, dryWet));    // 20 kHz -> 100 hz
      
      break;
    }
  }
}


void Effect::resetEffect() {
  
  // set tube noise intensity
  this->ledTube->setNoiseRate(0.0);
  
  // ableton manipulations, when ableton available
  if(!app->connectedToAbleton) return;
  
  cout << "reset effect" << endl;
  
  switch(effectType) {
    case EFFECT_TYPE_HISS: {
      
      app->live.getTrack("FX-HISS")->getDevice(0)->setEnabled(false);
      app->live.getTrack("FX-HISS")->getDevice(1)->setEnabled(false);
      
      app->live.getTrack("FX-HISS")
        ->getDevice("Crew Cut")
        ->getParameter("Dry/Wet")
        ->setValue(0.0);                  // 0%
      
      app->live.getTrack("FX-HISS")
        ->getDevice("Hiss")
        ->getParameter("Amount")
        ->setValue(0.0);                  // 0%
      
      break;
    }
    case EFFECT_TYPE_REVERB: {
      
      app->live.getTrack("FX-REVERB")->getDevice(0)->setEnabled(false);
      app->live.getTrack("FX-REVERB")->getDevice(1)->setEnabled(false);
      
      app->live.getTrack("FX-REVERB")
        ->getDevice("Drum Electrifier")
        ->getParameter("DryWet")
        ->setValue(0.0);                  // 0%
      
      app->live.getTrack("FX-REVERB")
        ->getDevice("Warm Reverb Long")
        ->getParameter("Dry/Wet")
        ->setValue(0.0);                  // 0%
      
      break;
    }
    case EFFECT_TYPE_CHOP: {
      
      app->live.getTrack("FX-CHOP")->getDevice(0)->setEnabled(false);
      app->live.getTrack("FX-CHOP")->getDevice(1)->setEnabled(false);
      
      app->live.getTrack("FX-CHOP")
        ->getDevice("Heavy")
        ->getParameter("Dry/Wet")
        ->setValue(0.0);                  // 0%
      
      app->live.getTrack("FX-CHOP")
        ->getDevice("High Pass")
        ->getParameter("Frequency")
        ->setValue(20.0);                 // 20 hz
      
      break;
    }
    case EFFECT_TYPE_VOCODER: {
      
      app->live.getTrack("FX-VOCODER")->getDevice(0)->setEnabled(false);
      app->live.getTrack("FX-VOCODER")->getDevice(1)->setEnabled(false);
      
      app->live.getTrack("FX-VOCODER")
        ->getDevice("Sharp Glass")
        ->getParameter("Dry/Wet")
        ->setValue(0.0);                  // 0%
      
      app->live.getTrack("FX-VOCODER")
        ->getDevice("Chromatic")
        ->getParameter("Dry/Wet")
        ->setValue(0.0);                  // 0%
      
      break;
    }
    case EFFECT_TYPE_LOWPASS: {
      
      app->live.getTrack("FX-LOWPASS")->getDevice(0)->setEnabled(false);
      app->live.getTrack("FX-LOWPASS")->getDevice(1)->setEnabled(false);
      
      app->live.getTrack("FX-LOWPASS")
        ->getDevice("Distort")
        ->getParameter("Dry/Wet")
        ->setValue(0.0);                  // 0%
      
      app->live.getTrack("FX-LOWPASS")
      ->getDevice("Auto Filter")
      ->getParameter("Frequency")
      ->setValue(135.0);                  // 20 kHz
      
      break;
    }
  }
}


float Effect::getValueForTrackA(float min, float max, float dryWet) {
  
  // percentage to where the effect degrades
  // and degrades again
  const float knee = 0.15;
  
  // graph line, builds up for knee amount of time to max
  // then degrades again
  float valueA = ofMap(dryWet, 0.0, knee, min, max);
  if(dryWet > knee) valueA = ofMap(dryWet, knee, 1.0, max, min);
  if(direction == -1) return min;
    
  return valueA;
}



float Effect::getValueForTrackB(float min, float max, float dryWet) {
  return ofMap(dryWet, 0.0, 0.5, min, max);
}


