#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
  ofEnableAlphaBlending();
  ofEnableSmoothing();

  spot = 0;

  base.loadImage("imgs/00eclpub2crop.png");

  nbLedProjector = 35;
  targets.resize(nbLedProjector);
  targets[0].loadImage("imgs/01p.png");
  targets[1].loadImage("imgs/02p.png");
  targets[2].loadImage("imgs/03p.png");
  targets[3].loadImage("imgs/04+5p.png");
  targets[4].loadImage("imgs/06+7p.png");
  targets[5].loadImage("imgs/08p.png");
  targets[6].loadImage("imgs/09+10p.png");
  targets[7].loadImage("imgs/11p.png");
  targets[8].loadImage("imgs/12p.png");
  targets[9].loadImage("imgs/13p.png");
  targets[10].loadImage("imgs/14p.png");
  targets[11].loadImage("imgs/15p.png");
  targets[12].loadImage("imgs/16p.png");
  targets[13].loadImage("imgs/17p.png");
  targets[14].loadImage("imgs/18+19+20p+100.png");
  targets[15].loadImage("imgs/21+22+23p+100.png");
  targets[16].loadImage("imgs/24p+100.png");
  targets[17].loadImage("imgs/25p+200.png");
  targets[18].loadImage("imgs/26p+200.png");
  targets[19].loadImage("imgs/27p+200.png");
  targets[20].loadImage("imgs/28p+200.png");
  targets[21].loadImage("imgs/29p+200.png");
  targets[22].loadImage("imgs/30p+200.png");
  targets[23].loadImage("imgs/31p+200.png");
  targets[24].loadImage("imgs/32p+300.png");
  targets[25].loadImage("imgs/33+34p+300.png");
  targets[26].loadImage("imgs/35+36p+300.png");
  targets[27].loadImage("imgs/37+38p+300.png");
  targets[28].loadImage("imgs/39p+300.png");
  targets[29].loadImage("imgs/40p+300.png");
  targets[30].loadImage("imgs/41p+300.png");
  targets[31].loadImage("imgs/42p+300.png");
  targets[32].loadImage("imgs/43p+300.png");
  targets[33].loadImage("imgs/44p+300.png");
  targets[34].loadImage("imgs/45p+300.png");

  //ofSetWindowShape(base.getWidth(), base.getHeight());
  ofSetWindowShape(base.getWidth(), 600);
  ofSetWindowTitle(ofToString(ofGetFrameRate()));


  for (int i = 0; i < (int) targets.size(); i++){
    ofxBlend* b = new ofxBlend();

    b->setBlendMode(BLEND_LIGHTEN);
    b->allocate(base.getWidth(), base.getHeight());
    blends.push_back(b);
  }

  ofEnableAlphaBlending();
  spots.resize(nbLedProjector);
  float step = 1./(nbLedProjector+1);
  for (int i = 0; i < (int) spots.size(); i++){
    spots[i].dmxChannel = i+1;
    spots[i].color.set(0);
    spots[i].pos.set(step + i*step, 0.1);
  }
  ofBackground(0);

  gaussienne.resize(500);
  for (int i = 0; i < (int) gaussienne.size(); i++){
    gaussienne[i] = (i < (int)gaussienne.size()/2) ?
      255./(gaussienne.size()/2)*i :
      255./(gaussienne.size()/2)*(gaussienne.size()-i);
  }
  bFirst = false;
  bLast = false;
  movers.resize(18);
  for (unsigned int i = 0; i < movers.size(); i++){
    movers[i].setup();
    movers[i].setMass(ofRandom(0.1, 4));
    //movers[i].setMass(0.1);
    movers[i].setLocation(ofRandomWidth(), 0);
  }
  ofFloatColor c;
  c.set(0);
  c +=  150/255.;
  cout << "c: " << c.getBrightness()*255 << endl;
  c +=  150/255.;
  cout << "c: " << c.getBrightness()*255 << endl;
}


//--------------------------------------------------------------
void testApp::updateMovers(){
  ofVec2f wind(ofMap(mouseX, ofGetWidth()/2, ofGetWidth(), 0, 0.01), 0);
  ofVec2f gravity(0, 0.1);
  for (unsigned int i = 0; i < movers.size(); i++){
    //friction
    float c = 0.01;
    ofVec2f friction(movers[i].getVelocity());
    friction *= -1;
    friction.normalize();
    friction *= c;


    movers[i].applyForce(friction);
    movers[i].applyForce(wind);
    movers[i].applyForce(gravity);
    movers[i].update();
    movers[i].checkEdges();
  }
}

//--------------------------------------------------------------
void testApp::updateSpotFromMovers(){
  float interval = ofGetWidth()/nbLedProjector;
  for (int i = 0; i < (int) spots.size(); i++){
    spots[i].color.set(0);
  }
  for (int i = 0; i < (int) movers.size(); i++){
    float diameter = movers[i].getDiameter();
    float radius = diameter/2.;
    int start = floor((movers[i].getLocation().x-radius)/interval);
    int end = ceil((movers[i].getLocation().x+radius)/interval);
    start = ofClamp(start, 0, spots.size()-1);
    end = ofClamp(end, 0, spots.size()-1);
    ofRectangle ball(movers[i].getLocation().x-radius, movers[i].getLocation().y-radius, diameter, diameter);
    for (int j = start; j < end; j++){
      ofRectangle intervalRect(j*interval, 0, interval, ofGetHeight());
      ofRectangle intersect = intervalRect.getIntersection(ball);
      spots[j].color +=1/movers[i].getMass()*intersect.getArea()/400.;
    }
  }
}

//--------------------------------------------------------------
void testApp::updateMoving(){
  //testFade+=20;
  testFade+=1;
  testFade %= 100*(spots.size()+10);
  for (int i = 0; i < (int) spots.size(); i++){
    int indice = i*100-(testFade-gaussienne.size());
    if (indice > 0 && indice < (int)gaussienne.size()){
      spots[i].color.set(gaussienne[indice]);
    } else {
      spots[i].color.set(0);
    }
  }
  if (bFirst){
    spots[3].color.set(255);
  }
  if (bLast){
    spots[40].color.set(255);
  }
}

//--------------------------------------------------------------
void testApp::update(){
  updateMoving();
  updateMovers();
  updateSpotFromMovers();
  
  blends[0]->begin();
  ofSetColor(spots[0].color);
  targets[0].draw(0,0);
  blends[0]->end();
  blends[0]->beginBackground();
  base.draw(0,0);
  blends[0]->endBackground();
  blends[0]->update();

  for (int i = 0; i < (int) blends.size() - 1; i++){
    int x = 0;
    if (i+1 > 13) x += 100;
    if (i+1 > 16) x += 100;
    if (i+1 > 23) x += 100;

    blends[i+1]->begin();
    ofSetColor(spots[i+1].color);
    targets[i+1].draw(x,0);
    blends[i+1]->end();

    blends[i+1]->beginBackground();
    blends[i]->draw(0,0);
    blends[i+1]->endBackground();
    blends[i+1]->update();
  }
}

//--------------------------------------------------------------
void testApp::draw(){
  ofBackgroundGradient(ofColor::gray, ofColor::black);
  ofSetColor(255);
  if (bDrawMovers){
    for (unsigned int i = 0; i < movers.size(); i++){
      movers[i].draw();
    }
  }
  ofSetColor(255);
  blends[blends.size()-1]->draw(0,ofGetHeight()-100);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

  if (key == OF_KEY_RIGHT){
    spot++;
    if (spot > (int)targets.size()) spot = 0;
  }
  if (key == OF_KEY_LEFT){
    spot--;
    if (spot < 0) spot = targets.size()-1;
  }
  if (key == 'a'){
    bFirst = !bFirst;
  }
  else if (key == 'z'){
    bLast = !bLast;
  }
  else if (key == 'd'){
    bDrawMovers = !bDrawMovers;
  }

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
