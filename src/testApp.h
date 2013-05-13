#pragma once

#include "ofMain.h"
#include "ofxFX.h"
#include "ofxUI.h"
#include "mover.h"
#include "LightenDisplay.h"

class ledProjector{
 public:
  int dmxChannel;
  ofVec2f pos;
  ofFloatColor color;
};

class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void exit();
    
    void updateMoving();
    void updateMovers();
    void updateSpotFromMovers();
    void updateSpotFromMoversGaussian();
    void updatePerlinNoise();
    int spot;
    int nbLedProjector;
    //vector<ledProjector> spots;
    vector<ofFloatColor> spots;
    LightenDisplay display;
    int testFade;
    vector<int> gaussienne;
    bool bFirst;
    bool bLast;
    vector<mover> movers;
    bool bDrawMovers;
    float yoff;
    float yoff_inc;
    float xoff_inc;
    float tourbillons_intensite;
    ofxUICanvas *gui;   	
    void guiEvent(ofxUIEventArgs &e);
    ofColor bgColor;
    ofPoint wind;
    float sd;
    float gaussian_intensite;
    float wind_speed;
    float temperature;
    float colorSaturation;

    ofxUICanvas *gui2;   	
    float simSpeed;
};
