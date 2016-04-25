#include "Sensor.h"

Sensor::Sensor() {
}

Sensor::Sensor(float x, float y, int pin, int numPad, Effect *effect) {
  this->boundingBox = ofRectangle(x, y, DEFAULT_SIZE, DEFAULT_SIZE);
  this->pin = pin;
  this->numPad = numPad;
  this->effect = effect;
  
  this->setup();
}

Sensor::~Sensor(){
}

void Sensor::setup(){
  this->active = false;
}

void Sensor::update(){

}

void Sensor::draw(){
  ofPushStyle();
  
  // half the width
  float halfWidth = boundingBox.width / 2.0;
  
  // draw sensor on/off
  ofFill();
  ofSetColor(active ? ofColor(255, 0, 255) : 40);
  ofDrawRectangle(boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height);
  
  // draw num and pin info
  ofSetColor(active ? ofColor(255, 0, 255) : 60);
  ofDrawBitmapString("num: " + to_string(numPad), boundingBox.x, boundingBox.y - 30.0);
  ofDrawBitmapString("pin: " + to_string(pin), boundingBox.x, boundingBox.y - 15.0);
  
  ofPopStyle();
}

void Sensor::activate(bool value) {
  if(value != active && value) effect->attack();
  if(value != active && !value) effect->release();
  
  active = value;
}

bool Sensor::isActive() {
  return active;
}