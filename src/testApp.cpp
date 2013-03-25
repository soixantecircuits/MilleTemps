#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
  ofEnableAlphaBlending();
  ofEnableSmoothing();

  spot = 0;

  nbLedProjector = 45;
  //display.setup("imgs/sub", nbLedProjector);
  display.setup("imgs/realistes_cropped", nbLedProjector);
  //ofSetWindowShape(base.getWidth(), base.getHeight());
  //ofSetWindowShape(800, 600);
  ofSetWindowShape(1327, 747);
  ofSetWindowTitle(ofToString(ofGetFrameRate()));

  ofEnableAlphaBlending();
  spots.resize(nbLedProjector);
  float step = 1./(nbLedProjector+1);
  for (int i = 0; i < (int) spots.size(); i++){
    spots[i].set(0);
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
  movers.resize(1);
  for (unsigned int i = 0; i < movers.size(); i++){
    movers[i].setup();
    movers[i].setMass(ofRandom(0.1, 4));
    //movers[i].setMass(0.1);
    movers[i].setLocation(ofRandomWidth(), 0);
  }
  yoff = ofRandom(0, 1000);
  yoff_inc = 0.005;
  yoff_inc = 0.3;
  float dim = 16; 
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
  float length = 320-xInit; 
	
  gui = new ofxUICanvas(0,0,length+xInit*2.0,ofGetHeight());     
	gui->addWidgetDown(new ofxUILabel("MILLE TEMPS", OFX_UI_FONT_LARGE)); 

    gui->addSpacer(length, 2); 
    gui->addWidgetDown(new ofxUILabel("ASPECT", OFX_UI_FONT_MEDIUM));     
    gui->addSlider("ECLAIRAGE PUBLIQUE", 0, 255, 255, 95, dim);
    gui->addSpacer(length, 2); 
    gui->addWidgetDown(new ofxUILabel("TOURBILLONS", OFX_UI_FONT_MEDIUM));     
    gui->addSlider("vitesse", 0.001, 0.020, &yoff_inc, 95, dim);
    gui->addSlider("largeur", 0.3, 0.01, &xoff_inc, 95, dim);
    /*
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addSlider("BGG", 0, 255, backgroundColor.g, 95, dim);
    gui->addSlider("BGB", 0, 255, backgroundColor.b, 95, dim);
    */
  ofAddListener(gui->newGUIEvent,this,&testApp::guiEvent);	
  gui->loadSettings("GUI/guiSettings.xml"); 
 
}

//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName(); 
	int kind = e.widget->getKind(); 
	
	if(name == "ECLAIRAGE PUBLIQUE")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget; 
		display.setBaseColor(ofColor(slider->getScaledValue())); 
  }
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
  //for (int i = 0; i < (int) spots.size(); i++){
  //  spots[i].set(0);
  //}
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
      spots[j]+=1/movers[i].getMass()*intersect.getArea()/400.;
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
      spots[i].set(gaussienne[indice]);
    } else {
      spots[i].set(0);
    }
  }
  if (bFirst){
    spots[3].set(255);
  }
  if (bLast){
    spots[40].set(255);
  }
}

//--------------------------------------------------------------
void testApp::updatePerlinNoise(){
  yoff += yoff_inc;
  float xoff = 0;
  for (int i = 0; i < nbLedProjector; i++){
    xoff += xoff_inc;
    float value = ofNoise(xoff, yoff);
    spots[i].set(value);
  }
}

//--------------------------------------------------------------
void testApp::update(){
  //updateMoving();
  updatePerlinNoise();
  updateMovers();
  updateSpotFromMovers();

  for (int i = 0; i < nbLedProjector; i++){
    //spots[i].set(1);
  }
  
  ofSetColor(bgColor);
  display.update(spots);
  
}

//--------------------------------------------------------------
void testApp::draw(){
  ofBackgroundGradient(ofColor::gray, ofColor::black);
  ofSetColor(255);
  ofSetColor(bgColor);
  display.draw(0, 400);
  ofSetColor(255);
  if (bDrawMovers){
    for (unsigned int i = 0; i < movers.size(); i++){
      movers[i].draw();
    }
  }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

  if (key == 'a'){
    bFirst = !bFirst;
  }
  else if (key == 'z'){
    bLast = !bLast;
  }
  else if (key == 'd'){
    bDrawMovers = !bDrawMovers;
  }
  switch (key){            
    case 'g':
      gui->toggleVisible(); 
      break; 
    default:
      break;
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
  //yoff = ofRandom(0, 1000);
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

//--------------------------------------------------------------
void testApp::exit()
{
    gui->saveSettings("GUI/guiSettings.xml");
    delete gui; 
}
