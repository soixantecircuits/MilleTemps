#include "testApp.h"
//#define SHADER_RENDERING

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
	//dmx.connect("tty.usbserial-EN086808"); // use the name
	// to see the permanent path of your device: $ sudo udevadm info --query=all --name=ttyUSB0
	// use the "by-id" path, ie. serial/by-id/usb-9710_7720-if00-port0
  // if not found but works with sudo:
  // sudo usermod -a -G dialout minad
  // minad is the name of user you're running
  // then:
  // sudo chmod a+rw /dev/ttyUSB0
	
	dmx.connect("ttyUSB0"); // use the name
  dmx.setChannels(nbLedProjector*3);
  reconnectDmxDelay = 0;

  #ifdef SHADER_RENDERING
  //display.setup("imgs/sub", nbLedProjector);
  display.setup("imgs/realistes_cropped", nbLedProjector);
  #endif

  //ofSetWindowShape(base.getWidth(), base.getHeight());
  //ofSetWindowShape(800, 600);
  int width = 1327;
  int height = 747;
  ofSetWindowShape(1327, 747);

  ofEnableAlphaBlending();
  spots.resize(nbLedProjector);
  float step = 1./(nbLedProjector+1);
  for (int i = 0; i < (int) spots.size(); i++){
    spots[i].set(0);
  }
  dmxChannels.resize(nbLedProjector);
  for (int i = 0; i < (int) dmxChannels.size(); i++){
    dmxChannels[i] = 1 + i*3;
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
  movers.resize(16);
  for (unsigned int i = 0; i < movers.size(); i++){
    movers[i].setup();
    //movers[i].setMass(ofRandom(0.5, 2));
    movers[i].setMass(ofRandom(0.1, 4));
    //movers[i].setMass(1);
    movers[i].setLocation(ofRandom(-1,2)*width, 0);
  }
  yoff = ofRandom(0, 1000);
  yoff_inc = 0.005;
  yoff_inc = 0.3;
  float dim = 16; 
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
  float length = 320-xInit; 
	
  gui = new ofxUICanvas(0,0,length+xInit*2.0,height);     
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
    gui->addSpacer(length, 2); 
    gui->addWidgetDown(new ofxUILabel("TEMPERATURE", OFX_UI_FONT_MEDIUM));     
    gui->addSlider("temperature", -5, 40, &temperature, 95, dim);
    gui->addSlider("saturation", 0, 100, &colorSaturation, 95, dim);
    gui->addSpacer(length, 2); 
    gui->addWidgetDown(new ofxUILabel("ENSOLEILLEMENT", OFX_UI_FONT_MEDIUM));     
    gui->addSlider("pyranometre", 0, 1, &pyranometre, 95, dim);
    /*
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addSlider("BGG", 0, 255, backgroundColor.g, 95, dim);
    gui->addSlider("BGB", 0, 255, backgroundColor.b, 95, dim);
    */
  ofAddListener(gui->newGUIEvent,this,&testApp::guiEvent);	
  gui->loadSettings("GUI/guiSettings.xml"); 

  gui2 = new ofxUICanvas(width- (length+xInit*2.0),0,width, height);     
	gui2->addWidgetDown(new ofxUILabel("SIMULATION", OFX_UI_FONT_LARGE)); 

    gui2->addSpacer(length, 2); 
    gui2->addSlider("Play X", 1, 100, &simSpeed, 95, dim);
    gui2->addSlider("Pixel To World", 1*100000, 100*100000, &coefPixelToRealWorld, 95, dim);
    gui2->addSlider("Friction", 0.000, 0.01, &frictionCoef, 95, dim);
    //gui2->addSlider("Play X", 1, 100, &simSpeed, 95, dim);
    gui2->addLabelToggle("SENSORS", &bUseSensors, true);
    gui2->addLabelToggle("DMX", &bUseDmx, true);
  gui2->loadSettings("GUI/guiSettings2.xml"); 

  metakPro.setup();
	ofAddListener(metakPro.NEW_SENSORDATA,this,&testApp::onNewSensorData);
  metakPro.log();
  //metakPro.replay("test.log");
}

//--------------------------------------------------------------
void testApp::onNewSensorData(SensorData & s){
  cout <<  s.toString() << endl;
  if (bUseSensors){
    wind_speed = s.vitesse;// * 1000;
    wind.y = 90 + s.direction;
    if (wind.y > 360) wind.y -= 360;
    temperature = s.temperature;
    pyranometre = s.pyranometre;
  }
}

//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName(); 
	int kind = e.widget->getKind(); 
	
	if(name == "ECLAIRAGE PUBLIQUE")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget; 
    #ifdef SHADER_RENDERING
		display.setBaseColor(ofColor(slider->getScaledValue())); 
    #endif
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
  ofVec2f windForce(wind_speed * wind_speed * windCartesian/coefPixelToRealWorld);  
  ofVec2f gravity(0, 0.1);
  for (unsigned int i = 0; i < movers.size(); i++){
    //friction
    float c = frictionCoef;
    ofVec2f friction(movers[i].getVelocity());
    friction *= -1;
    friction.normalize();
    friction *= c;


    movers[i].applyForce(friction);
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
  for (int i = 0; i < simSpeed; i++){
    updatePerlinNoise();
    updateMovers();
    updateSpotFromMoversGaussian();
  }

  float hue = ofMap(temperature, -5, 32, 0, 260/360.);
  for (int i = 0; i < nbLedProjector; i++){
    spots[i].setHsb(hue,colorSaturation/100.,spots[i].getBrightness());
  }
  #ifdef SHADER_RENDERING
  display.update(spots);
  #endif

  updateDmx();
  
  ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::updateDmx(){
  if ( bUseDmx && dmx.isConnected()){
    for (int i = 0; i < nbLedProjector; i++){ 
      dmx.setLevel(dmxChannels[i], ofMap(spots[i].r, 0, 1,60,255,true));
      dmx.setLevel(dmxChannels[i]+1, ofMap(spots[i].g, 0, 1,59,255,true));
      dmx.setLevel(dmxChannels[i]+2, ofMap(spots[i].b, 0, 1,59,255,true));
    }
    /*
      dmx.setLevel(dmxChannels[0], 255);
      dmx.setLevel(dmxChannels[1], 255);
      dmx.setLevel(dmxChannels[2], 255);
      */
    cout << "Level: " << (int)dmx.getLevel(dmxChannels[0]) << "\t" << (int)dmx.getLevel(dmxChannels[1]) << "\t" << (int)dmx.getLevel(dmxChannels[2]) << endl;
    dmx.update();
  }

  if ( !dmx.isConnected() && reconnectDmxDelay < ofGetElapsedTimef()){
    dmx.connect("ttyUSB0"); // use the name
    reconnectDmxDelay = ofGetElapsedTimef() + 1000;
  }
}

//--------------------------------------------------------------
void testApp::draw(){
  //ofBackgroundGradient(ofColor::gray, ofColor::black);
  #ifdef SHADER_RENDERING
  ofSetColor(255);
  display.draw(0, 400);
  #endif

  ofSetColor(255);
  if (bDrawMovers){
    for (unsigned int i = 0; i < movers.size(); i++){
      movers[i].draw();
    }
  }
    ofSetColor(255);
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
      gui2->toggleVisible(); 
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
    gui2->saveSettings("GUI/guiSettings2.xml");
    delete gui; 
    delete gui2; 
}
