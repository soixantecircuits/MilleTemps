#pragma once

#include "ofMain.h"
#include "ofxFX.h"
#include "ofxUI.h"
#include "mover.h"
#include "LightenDisplay.h"
#include "ofxMetakPro.h"
#include "ofxDmx.h"
#include "ofxOsc.h"

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
    void updateDmx();
    void updateOsc();
    void turnOn();
    void turnOff();
    int spot;
    int nbLedProjector;
    //vector<int> dmxChannels;
    vector< vector<int> > dmxChannels;
    vector<ofFloatColor> spots;
    LightenDisplay display;
    int testFade;
    vector<int> gaussienne;
    bool bFirst;
    bool bLast;
    vector< ofPtr<Mover> > movers;
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
    float last_wind_speed;
    float temperature;
    float colorSaturation;
    float pyranometre;
    float coefPixelToRealWorld;
    float frictionCoef;
    float speedIntensity;

    ofxUICanvas *gui2;   	
    float simSpeed;

    void onNewSensorData(SensorData & s);
    ofxMetakPro metakPro;
    bool bUseSensors;
    ofxDmx dmx;
    bool bUseDmx;
    float reconnectDmxDelay;

    ofxOscReceiver oscReceiver;
};
