#pragma once

#include "ofMain.h"
#include "ofxFX.h"
#include "mover.h"

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
    
    void updateMoving();
    void updateMovers();
    void updateSpotFromMovers();
    vector<ofImage> targets;
    vector<ofxBlend*> blends;
    ofImage base;
    int spot;
    int nbLedProjector;
    vector<ledProjector> spots;
    int testFade;
    vector<int> gaussienne;
    bool bFirst;
    bool bLast;
    vector<mover> movers;
    bool bDrawMovers;
};
