#pragma once

#include "ofMain.h"
#include "ofxSimpleSerial.h"
#include "SensorData.h"

class ofxMetakPro : public ofBaseApp{

  public:
    void setup();
    void log();
    void replay(string filename);
    ofEvent<SensorData> NEW_SENSORDATA;

  private:

    void onNewMessage(string & message);
    void newData(string & message);
    void update(ofEventArgs & args);
    void sendNextLine();

    ofxSimpleSerial	serial;
    float nextLine;
    ofBuffer buffer;
    float reconnectDelay;
    bool connect;

};
