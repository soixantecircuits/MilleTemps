#pragma once

#include "ofMain.h"

class mover {

	public:
		void setup(){
      location.set(30, 30);
      velocity.set(0, 0);
      acceleration.set(0, 0);
      mass = 1;
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
      ofEllipse(location, mass * 16, mass * 16);
    }

    void setMass(float m){
      mass = m;
    }

    void setLocation( float x, float y){
      location.set(x, y);
    }

    const ofVec2f& getVelocity() const{ return velocity;}
    const ofVec2f& getLocation() const{ return location;}

    void checkEdges(){

      if (location.x > ofGetWidth()){
        //velocity.x *= -1;
        //location.x = ofGetWidth();
        location.x = 0;
      } else if (location.x < 0){
        //velocity.x *= -1;
        //location.x = 0;
        location.x = ofGetWidth();
      }
      
      if (location.y > ofGetHeight() - 100){
        velocity.y *= -1;
        location.y = ofGetHeight() - 100;
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
};
