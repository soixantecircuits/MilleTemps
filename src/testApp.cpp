#include "testApp.h"
float gaussian(float x, float mean, float variance) {  
  float dx = x - mean;  
  //return (1.f / sqrtf(TWO_PI * variance)) * expf(-(dx * dx) / (2 * variance));  
  return expf(-(dx * dx) / (2 * variance));  
}  

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
  movers.resize(32);
  for (unsigned int i = 0; i < movers.size(); i++){
    movers[i].setup();
    //movers[i].setMass(ofRandom(0.5, 2));
    movers[i].setMass(ofRandom(0.1, 4));
    //movers[i].setMass(1);
    movers[i].setLocation(ofRandom(-1,2)*ofGetWidth(), 0);
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
    gui->addSlider("intensite", 0, 100, &tourbillons_intensite, 95, dim);
    gui->addSpacer(length, 2); 
    gui->addWidgetDown(new ofxUILabel("NUAGES", OFX_UI_FONT_MEDIUM));     
    gui->addSlider("largeur_", 0.1, 20, &sd, 95, dim);
    gui->addSlider("intensite_", 0, 100, &gaussian_intensite, 95, dim);
    gui->addSpacer(length, 2); 
    gui->addWidgetDown(new ofxUILabel("VENT", OFX_UI_FONT_MEDIUM));     
    gui->addWidgetDown(new ofxUIRotaryCircleSlider("R2SLIDERCIRCLEROTARY", ofPoint(0,60), ofPoint(0,360), &wind, dim*8));
    gui->addSlider("vitesse (en km/h)", 0, 200, &wind_speed, 200, dim);
    /*
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addSlider("BGG", 0, 255, backgroundColor.g, 95, dim);
    gui->addSlider("BGB", 0, 255, backgroundColor.b, 95, dim);
    */
  ofAddListener(gui->newGUIEvent,this,&testApp::guiEvent);	
  gui->loadSettings("GUI/guiSettings.xml"); 

  // record
  sampleRate = 44100;
  channels = 2;

  ofSetFrameRate(60);
  ofSetLogLevel(OF_LOG_VERBOSE);
  fileName = "testMovie";
  fileExt = ".mov"; // ffmpeg uses the extension to determine the container type. run 'ffmpeg -formats' to see supported formats

  // override the default codecs if you like
  // run 'ffmpeg -codecs' to find out what your implementation supports (or -formats on some older versions)
  vidRecorder.setVideoCodec("mpeg4"); 
  vidRecorder.setVideoBitrate("800k");
  //vidRecorder.setAudioCodec("mp3");
  //vidRecorder.setAudioBitrate("192k");

  bRecording = false;
  
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
  else if(name == "R2SLIDERCIRCLEROTARY")
	{
		ofxUIRotaryCircleSlider *rotaryslider = (ofxUIRotaryCircleSlider *) e.widget; 
    ofxUISlider *slider = (ofxUISlider *) gui->getWidget("vitesse (en km/h)");
    slider->setValue(rotaryslider->getScaledValue().x);
  }
  else if(name == "vitesse (en km/h)")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget; 
    ofxUIRotaryCircleSlider* rotaryslider = (ofxUIRotaryCircleSlider *) gui->getWidget("R2SLIDERCIRCLEROTARY");
    rotaryslider->setValue(ofPoint(slider->getScaledValue(), rotaryslider->getScaledValue().y));
  }
}

//--------------------------------------------------------------
void testApp::updateMovers(){
  //ofVec2f wind(ofMap(mouseX, ofGetWidth()/2, ofGetWidth(), 0, 0.01), 0);
  ofVec2f windCartesian(-sin(ofDegToRad(wind.y)), cos(ofDegToRad(wind.y)));
  float coef = 0.0001/6./10.;
  ofVec2f windForce(coef * wind_speed * wind_speed * windCartesian);  
  ofVec2f gravity(0, 0.1);
  for (unsigned int i = 0; i < movers.size(); i++){
    //friction
    float c = 0.01;
    ofVec2f friction(movers[i].getVelocity());
    friction *= -1;
    friction.normalize();
    friction *= c;


    //movers[i].applyForce(friction);
    movers[i].applyForce(windForce);
    //movers[i].applyForce(gravity);
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
void testApp::updateSpotFromMoversGaussian(){
  float interval = ofGetWidth()/nbLedProjector;
  for (int i = 0; i < (int) movers.size(); i++){
    float diameter = movers[i].getDiameter();
    float radius = diameter/2.;
    float where = (movers[i].getLocation().x-radius)/interval;
     for (int i = 0; i < nbLedProjector; i++){
      float value = gaussian(i, where, sd)*gaussian_intensite/100;
      spots[i]+=value;
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
    float value = ofNoise(xoff, yoff)*tourbillons_intensite/100;
    spots[i].set(value);
  }
}

//--------------------------------------------------------------
void testApp::update(){
  //updateMoving();
  updatePerlinNoise();
  updateMovers();
  updateSpotFromMoversGaussian();

  for (int i = 0; i < nbLedProjector; i++){
    //spots[i].set(1);
  }
  
  ofSetColor(bgColor);
  display.update(spots);
  
  if(bRecording){
    ofPixels pixels;
    display.getTextureReference().readToPixels(pixels);
    vidRecorder.addFrame(pixels);
  }
}

//--------------------------------------------------------------
void testApp::draw(){
  //ofBackgroundGradient(ofColor::gray, ofColor::black);
  ofSetColor(255);
  ofSetColor(bgColor);
  display.draw(0, 400);
  ofSetColor(255);
  if (bDrawMovers){
    for (unsigned int i = 0; i < movers.size(); i++){
      movers[i].draw();
    }
  }
  if(bRecording){
    ofSetColor(255, 0, 0);
    ofCircle(ofGetWidth() - 20, 20, 5);
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
  if(key=='r'){
    bRecording = !bRecording;
    if(bRecording && !vidRecorder.isInitialized()) {
      vidRecorder.setup(fileName+ofGetTimestampString()+fileExt, ofGetWidth(), ofGetHeight(), 30, sampleRate, channels);
      //          vidRecorder.setup(fileName+ofGetTimestampString()+fileExt, vidGrabber.getWidth(), vidGrabber.getHeight(), 30); // no audio
      //            vidRecorder.setup(fileName+ofGetTimestampString()+fileExt, 0,0,0, sampleRate, channels); // no video
      //          vidRecorder.setupCustomOutput(vidGrabber.getWidth(), vidGrabber.getHeight(), 30, sampleRate, channels, "-vcodec mpeg4 -b 1600k -acodec mp2 -ab 128k -f mpegts udp://localhost:1234"); // for custom ffmpeg output string (streaming, etc)
    }
  }
  if(key=='c'){
    bRecording = false;
    vidRecorder.close();
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
    vidRecorder.close();
}
