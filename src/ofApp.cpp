#include "ofApp.h"
#include <iostream>
#include <regex>

ofApp::ofApp(int port)
{
    _receiver.setup(port);
}
ofApp::ofApp(std::string host, int port, std::string message, bool interactive)
{
    _sender.setup(host, port);
    sendMessage(message);

    if (interactive)
    {
        while (true)
        {
            ofLogNotice(_name) << "please enter a new message: address firstArg secondArg thirdArg";
            getline(cin, message);
            sendMessage(message);
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
        std::string msgString;
        msgString = m.getAddress();
        msgString += ":";
        for (size_t i = 0; i < m.getNumArgs(); i++)
        {
            // get the argument type
            msgString += " ";
            msgString += m.getArgTypeName(i);
            msgString += ":";

            // display the argument - make sure we get the right type
            if (m.getArgType(i) == OFXOSC_TYPE_INT32)
            {
                msgString += ofToString(m.getArgAsInt32(i));
            }
            else if (m.getArgType(i) == OFXOSC_TYPE_FLOAT)
            {
                msgString += ofToString(m.getArgAsFloat(i));
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
            }
            else
            {
                msgString += "unhandled argument type " + m.getArgTypeName(i);
            }
        }
        ofLogNotice(_name) << msgString;
    }
}

void ofApp::sendMessage(std::string message)
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
                msg.addStringArg(arg);
            }
        }
    }

    _sender.sendMessage(msg);
}
