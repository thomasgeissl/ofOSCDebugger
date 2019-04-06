#include "ofApp.h"
#include <iostream>
#include <regex>

ofApp::ofApp(int port, bool record, std::string outputPath) : _outputPath(outputPath)
{
    _receiver.setup(port);
		_record.set("record", record);
		// _parameters.add(_recording);
		_recording = ofJson::array();
}
ofApp::ofApp(std::string host, int port, std::string message, bool interactive, bool notBundled)
{
    _sender.setup(host, port);
    sendMessage(message, notBundled);

    if (interactive)
    {
        while (true)
        {
            ofLogNotice(_name) << "please enter a new message: address firstArg secondArg thirdArg";
            getline(cin, message);
            sendMessage(message, notBundled);
        }
    }
    else
    {
        ofExit(0);
    }
}

void ofApp::update()
{
    ofSetWindowTitle(ofToString((int)(ofGetFrameRate())));
    while (_receiver.hasWaitingMessages())
    {
			ofxOscMessage m;
			_receiver.getNextMessage(m);
			ofJson messageJson;
			messageJson["address"] = m.getAddress();
			// messageJson["args"] = ofJson.array();
			messageJson["timeStamp"] = ofGetElapsedTimeMillis();
        std::string msgString;
        msgString = m.getAddress();
				messageJson["address"] = m.getAddress();
        msgString += ":";
        for (size_t i = 0; i < m.getNumArgs(); i++)
        {
					ofJson argJson;
					argJson["type"] = m.getArgTypeName(i);
            // get the argument type
            msgString += " ";
            msgString += m.getArgTypeName(i);
            msgString += ":";

            // display the argument - make sure we get the right type
            if (m.getArgType(i) == OFXOSC_TYPE_INT32)
            {
                msgString += ofToString(m.getArgAsInt32(i));
								argJson["value"] = m.getArgAsInt32(i);
            }
            else if (m.getArgType(i) == OFXOSC_TYPE_FLOAT)
            {
                msgString += ofToString(m.getArgAsFloat(i));
								argJson["value"] = m.getArgAsFloat(i);
            }
            else if (m.getArgType(i) == OFXOSC_TYPE_TRUE)
            {
                msgString.pop_back();
            }
            else if (m.getArgType(i) == OFXOSC_TYPE_FALSE)
            {
                msgString.pop_back();
            }
            else if (m.getArgType(i) == OFXOSC_TYPE_STRING)
            {
                msgString += m.getArgAsString(i);
								argJson["value"] = m.getArgAsString(i);
            }
            else
            {
                msgString += "unhandled argument type " + m.getArgTypeName(i);
            }
						messageJson["args"].push_back(argJson);
        }
        ofLogNotice(_name) << msgString;
				if(_record){
					_recording.push_back(messageJson);
				}
    }
}

void ofApp::exit(){
	if(_record){
		ofSaveJson(_outputPath, _recording);
	}
}

void ofApp::sendMessage(std::string message, bool notBundled)
{
    auto parts = ofSplitString(message, " ");
    auto address = parts[0];
    parts.erase(parts.begin());

    ofxOscMessage msg;
    msg.setAddress(address);
    std::regex floatRegex{R"([+\-]?(?:0|[1-9]\d*)(?:\.\d*)?(?:[eE][+\-]?\d+)?)"};
    std::regex intRegex{R"(\d+)"};
    for (auto arg : parts)
    {
        if (std::regex_match(arg, intRegex))
        {
            istringstream inputStream(arg);
            int i;
            inputStream >> i;
            msg.addIntArg(i);
        }
        else if (std::regex_match(arg, floatRegex))
        {
            istringstream inputStream(arg);
            float f;
            inputStream >> f;
            msg.addFloatArg(f);
        }
        else
        {
            // TODO: blob, impulse, timetag
            if (arg == "TRUE" || arg == "true" || arg == "T")
            {
                msg.addBoolArg(true);
            }
            else if (arg == "FALSE" || arg == "false" || arg == "F")
            {
                msg.addBoolArg(false);
            }
            else
            {

                auto words = ofSplitString(arg, "---");
                if(words.size() > 1){
                    std::string multiWordArg = "";
                    for(auto word : words){
                        multiWordArg += word;
                        multiWordArg += " ";
                    }
                    multiWordArg = multiWordArg.substr(0, multiWordArg.size() - 1);
                    msg.addStringArg(multiWordArg);
                }else{
                    msg.addStringArg(arg);
                }
            }
        }
    }

    _sender.sendMessage(msg, !notBundled);
}
