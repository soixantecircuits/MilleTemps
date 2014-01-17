#pragma once

#include "ofMain.h"

class SensorData {

  public:
    void set(string & data){
      vector<string> input = ofSplitString(data, ",");

      if (input.size() >= 14){
        direction = ofToInt(input[1]);
        vitesse = ofToFloat(input[2])/1000.*60.*60.;
        pression = ofToFloat(input[3]);
        humidite = ofToFloat(input[4]);
        temperature = ofToFloat(input[5]);
        pointderose = ofToFloat(input[6]);
        pyranometre = ofToFloat(input[8]);
        tension = ofToFloat(input[11]);
      }
    }
    string toString(){
      ostringstream out;
      out << "Direction:" << direction << "\t"
        << "Vitesse:" << vitesse << "\t"
        << "Pression:" << pression << "\t"
        << "Humiditee:" << humidite << "\t"
        << "Temperature:" << temperature << "\t"
        << "Point de rosee:" << pointderose << "\t"
        << "Pyranometre:" << pyranometre << "\t"
        << "Tension:" << tension;
      return out.str();
    }
	

    float direction;
    float vitesse;
    float pression;
    float humidite;
    float temperature;
    float pointderose;
    float pyranometre;
    float tension;
};
