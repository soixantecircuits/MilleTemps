#include "ofxMetakPro.h"

//--------------------------------------------------------------
void ofxMetakPro::setup(){
}

//--------------------------------------------------------------
void ofxMetakPro::log(){
	serial.setup("/dev/ttyUSB0", 19200);
	serial.startContinuousRead(false);
	ofAddListener(serial.NEW_MESSAGE,this,&ofxMetakPro::onNewMessage);

	ofLogToFile("logfile.log", true); //set channel to log file. log file resides in the /bin/data folder
	ofSetLogLevel(OF_LOG_NOTICE);
	ofLogNotice() << "Start program" << endl;
}

void ofxMetakPro::onNewMessage(string & message)
{
	cout << "onNewMessage, message: " << message << "\n";
	ofLogNotice() << "MetakPro: " << ofGetTimestampString() << " " << message << endl;

  newData(message);
}

void ofxMetakPro::newData(string & message)
{
  SensorData s;
  s.set(message);
  ofNotifyEvent(NEW_SENSORDATA,s,this);
}

//--------------------------------------------------------------
void ofxMetakPro::replay(string filename){
  buffer = ofBufferFromFile(filename);
  nextLine = 0;
  ofAddListener(ofEvents().update,this,&ofxMetakPro::update);
}

//--------------------------------------------------------------
void ofxMetakPro::update(ofEventArgs & args){
	float elapsedTime = ofGetElapsedTimef();
	if(elapsedTime >= nextLine){
		nextLine = elapsedTime + 1; // triggerOne fires every second
    sendNextLine();
	}
}

//--------------------------------------------------------------
void ofxMetakPro::sendNextLine(){
  while(buffer.isLastLine() == false) {
    string line = buffer.getNextLine();
      // is it sensor data ?
      vector<string> input = ofSplitString(line, ",");
      if (input.size() >= 14){
        newData(line);
        break;
      }
  }
}


