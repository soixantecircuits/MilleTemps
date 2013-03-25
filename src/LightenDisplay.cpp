#include "LightenDisplay.h"

//--------------------------------------------------------------
void LightenDisplay::setup(string path, int nbImages){
  ofEnableAlphaBlending();

  //base.loadImage(path + "/base.png");
  base.loadImage(path + "/00eclpub.jpg");

  nbLedProjector = nbImages;
  targets.resize(nbLedProjector);
  for (int i = 0; i < (int) targets.size(); i++){
    //targets[i].loadImage(path +"/" + ofToString(i+1, 2, '0') + ".png");
    targets[i].loadImage(path +"/" + ofToString(i+1, 2, '0') + ".jpg");
  }

  for (int i = 0; i < (int) targets.size(); i++){
    ofxBlend* b = new ofxBlend();

    b->setBlendMode(BLEND_LIGHTEN);
    b->allocate(base.getWidth(), base.getHeight());
    blends.push_back(b);
  }

  baseColor.set(255);
}

//--------------------------------------------------------------
void LightenDisplay::update(vector<ofFloatColor> & spots){
  
  ofSetColor(255);
  blends[0]->begin();
  ofSetColor(spots[0]);
  targets[0].draw(0,0);
  blends[0]->end();
  blends[0]->beginBackground();
  ofSetColor(baseColor);
  base.draw(0,0);
  blends[0]->endBackground();
  blends[0]->update();

  for (int i = 0; i < (int) blends.size() - 1; i++){
    blends[i+1]->begin();
    ofSetColor(spots[i+1]);
    targets[i+1].draw(0,0);
    blends[i+1]->end();

    blends[i+1]->beginBackground();
    blends[i]->draw(0,0);
    blends[i+1]->endBackground();
    blends[i+1]->update();
  }
}

//--------------------------------------------------------------
void LightenDisplay::draw(int x, int y){
  ofSetColor(255);
  blends[blends.size()-1]->draw(x, y);
}

