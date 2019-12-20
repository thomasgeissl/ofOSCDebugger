#include "ofApp.h"
#include <iostream>
#include <regex>

ofApp::ofApp(int port, bool record, std::string outputPath) : _outputPath(outputPath)
{
    _receiver.setup(port);
    _record.set("record", record);
    _parameters.add(_record);
    _messages = ofJson::array();
}
ofApp::ofApp(int port, std::vector<std::string> hosts, std::vector<int> outputPorts)
{
    _receiver.setup(port);
    _senders.resize(outputPorts.size());
    if (hosts.size() != outputPorts.size())
    {
        ofLogNotice() << "number of hosts and ports does not match";
        ofExit();
    }
    for (auto i = 0; i < outputPorts.size(); i++)
    {
        _senders[i].setup(hosts[i], outputPorts[i]);
    }
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
ofApp::ofApp(std::string host, int port, std::string inputPath)
{
    _sender.setup(host, port);
    ofFile file(inputPath);
    if (file.exists())
    {
        int lastTimeStamp = 0;
        file >> _messages;
        for (auto &message : _messages)
        {
            ofxOscMessage msg;
            msg.setAddress(message["address"]);
            for (auto &arg : message["args"])
            {
                if (arg["type"] == "i")
                {
                    int value = arg["value"];
                    msg.addIntArg(value);
                }
                else if (arg["type"] == "f")
                {
                    float value = arg["value"];
                    msg.addFloatArg(value);
                }
                else if (arg["type"] == "s")
                {
                    std::string value = arg["value"];
                    msg.addStringArg(value);
                }
                else if (arg["type"] == "F")
                {
                    msg.addBoolArg(false);
                }
                else if (arg["type"] == "T")
                {
                    msg.addBoolArg(true);
                }
            }

            int timeStamp = message["timeStamp"];
            ofLogNotice() << "waiting for " << timeStamp - lastTimeStamp << "ms and then sending message";
            ofSleepMillis(timeStamp - lastTimeStamp);
            _sender.sendMessage(msg, true);
            lastTimeStamp = message["timeStamp"];
        }
        ofExit(0);
    }
    else
    {
        ofLogError(_name) << "file does not exist: " << inputPath;
        ofExit(-1);
    }
}

void ofApp::update()
{
    ofSetWindowTitle(ofToString((int)(ofGetFrameRate())));
    while (_receiver.hasWaitingMessages())
    {
        ofxOscMessage m;
        _receiver.getNextMessage(m);
        if (_senders.size())
        {
            for (auto &sender : _senders)
            {
                sender.sendMessage(m);
            }
        }
        ofJson messageJson;
        messageJson["address"] = m.getAddress();
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
        if (_record)
        {
            _messages.push_back(messageJson);
        }
    }
}

void ofApp::exit()
{
    if (_record)
    {
        ofSaveJson(_outputPath, _messages);
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
                if (words.size() > 1)
                {
                    std::string multiWordArg = "";
                    for (auto word : words)
                    {
                        multiWordArg += word;
                        multiWordArg += " ";
                    }
                    multiWordArg = multiWordArg.substr(0, multiWordArg.size() - 1);
                    msg.addStringArg(multiWordArg);
                }
                else
                {
                    msg.addStringArg(arg);
                }
            }
        }
    }

    _sender.sendMessage(msg, !notBundled);
}
