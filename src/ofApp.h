#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

class ofApp : public ofBaseApp
{
public:
  ofApp(int port);
  ofApp(std::string host, int port, std::string message, bool interactive = false, bool notBundled = false);
  void update();
  void sendMessage(std::string message, bool notBundled);
  
  ofxOscReceiver _receiver;
  ofxOscSender _sender;
  std::string _name = "ofOSCDebugger";
};
