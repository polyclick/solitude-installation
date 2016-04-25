#include "LedTube.h"
#include "ofxTweenzor.h"

LedTube::LedTube(float x, float y, int trigger) {
  this->boundingBox = ofRectangle(x, y, DEFAULT_WIDTH, DEFAULT_HEIGHT);
  this->trigger = trigger;
  
  this->setup();
}

LedTube::~LedTube() {
}

void LedTube::setup() {
  this->color = ofColor(255);
  this->intensity = 0.0;
  this->overruled = false;
  
  _accentColor = ofColor(255, 0, 0);    // default led accent color to red
}

void LedTube::update() {

}

void LedTube::draw() {
  ofPushStyle();
  
  if(overruled) {
    float frame = ofMap(_noiseRate, 0.0, 1.0, 450.0, 1.0);
    float intensity = sin(ofGetElapsedTimeMillis() / frame);
    if(_noiseRate <= 0.0) intensity = 0.15 + (abs(sin(ofGetElapsedTimeMillis() / 750.0) * 0.85));
    ofSetColor(_accentColor.r * intensity, _accentColor.g * intensity, _accentColor.b * intensity);
  } else {
    ofSetColor(color.r * intensity, color.g * intensity, color.b * intensity);
  }
  
  ofFill();
  ofDrawRectangle(boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height);
  
  ofPopStyle();
}

void LedTube::drawDebug() {
  ofPushStyle();
  
  // always draw rect borders
  ofNoFill();
  ofSetLineWidth(2);
  ofSetColor(100);
  ofDrawRectangle(boundingBox.x - 2, boundingBox.y - 2, boundingBox.width + 4, boundingBox.height + 4);
  
  ofPopStyle();
}

void LedTube::setIntensity(float value) {
  Tweenzor::add(&this->intensity, this->intensity, value, 0.f, SMOOTHING_TIME);
}

float LedTube::getIntensity() {
  return this->intensity;
}

void LedTube::setAccentColor(ofColor value) {
  _accentColor = value;
}

void LedTube::setNoiseRate(float value) {
  _noiseRate = value;
}