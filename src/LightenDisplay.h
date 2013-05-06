#pragma once

#include "ofMain.h"
#include "ofxFX.h"

class LightenDisplay { 
public:
    void setup(string path, int nbImages);
    void update(vector<ofFloatColor> & colors);
    void draw(int x = 0, int y = 0);
    void setBaseColor(ofColor color){baseColor.set(color);};
    ofTexture getTextureReference();

    vector<ofImage> targets;
    vector<ofxBlend*> blends;
    ofImage base;
    int nbLedProjector;
    ofColor baseColor;
};
