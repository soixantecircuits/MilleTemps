#pragma once

#include "ofMain.h"

class mover {

	public:
		void setup(){
      location.set(30, 30);
      velocity.set(0, 0);
      acceleration.set(0, 0);
      mass = 0.01;
      density = 1/16.;
    }

		void applyForce(const ofVec2f & force){
      ofVec2f f(force/mass);
      acceleration += f;
    }

		void update(){
      velocity += acceleration;
      location += velocity;
      acceleration *= 0;
    }

		void draw(){
      ofSetColor(175);
      ofEllipse(location, mass / density, mass /density);
    }

    void setMass(float m){
      mass = m;
    }
    void setDensity(float d){
      density = d;
    }

    void setLocation( float x, float y){
      location.set(x, y);
    }

    const ofVec2f& getVelocity() const{ return velocity;}
    const ofVec2f& getLocation() const{ return location;}
    float getMass() const{ return mass;}
    float getDiameter() const{ return mass/density;}

    void checkEdges(){

      if (location.x > ofGetWidth()*2){
        //velocity.x *= -1;
        //location.x = ofGetWidth();
        location.x = -ofGetWidth();
        velocity *= 0;
        //mass = ofRandom(0.1, 1);
      } else if (location.x < -ofGetWidth()){
        //velocity.x *= -1;
        //location.x = 0;
        location.x = ofGetWidth()*2;
        velocity *= 0;
        //mass = ofRandom(0.1, 4);
      }
      
      if (location.y > ofGetHeight()){
        velocity.y *= -1;
        location.y = ofGetHeight();
      } else if (location.y < 0){
        velocity.y *= -1;
        location.y = 0;
      }

    }
		
  private:
    ofVec2f location;
    ofVec2f velocity;
    ofVec2f acceleration;
    float mass;
    float density;
};
