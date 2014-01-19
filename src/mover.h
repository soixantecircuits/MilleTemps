#pragma once

#include "ofMain.h"

class Mover {

	public:
		void setup(){
      location.set(30, 30);
      velocity.set(0, 0);
      acceleration.set(0, 0);
      mass = 0.01;
      density = 1/16.;
      yoff = ofRandom(0, 1000);
    }

		void applyForce(const ofVec2f & force){
      ofVec2f f(force/mass);
      acceleration += f;
    }

		void update(){
      velocity += acceleration;
      location += velocity;
      acceleration *= 0;

      lightning *= 0.9;
      if (ofRandomuf() < 0.01){
        //lightning = 1;
      }
    }

		void draw(){
      //ofEllipse(location, 4* mass / density, 4* mass /density);
      ofEllipse(location, 4* getDiameter(), 4* getDiameter());
    }

    void setMass(float m){
      mass = m;
    }
    void setDensity(float d){
      density = d;
    }
		void setVelocity(const ofVec2f & v){
      velocity = v;
      mass = 1/v.length();
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

    float getLightning(){
      return 1 + lightning;
    }
    static bool shouldRemoveOffScreen(ofPtr<Mover> circle){
      return circle.get()->getLocation().x < -500 || circle.get()->getLocation().x > ofGetWidth()+500 ;
    }
    float yoff;
		
  private:
    ofVec2f location;
    ofVec2f velocity;
    ofVec2f acceleration;
    float mass;
    float density;
    float lightning;
};
