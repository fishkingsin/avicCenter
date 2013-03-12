//
//  ofxYucolabCommander.h
//  Zone4Sub
//
//  Created by james KONG on 11/3/13.
//
//

#pragma once

#include "ofMain.h"
#include "ofxNetwork.h"

class ofxYucolabCommanderEvents {
public:
	ofEvent<ofMessage> messageUpdated;
};


class ofxYucolabCommander
{
public:
	void setup(string _host , int _port);
	ofxYucolabCommanderEvents events;
protected:
	void update(ofEventArgs& args);
	ofxTCPClient tcpClient;
	bool weConnected;
	float connectTime , deltaTime ;
	int port;
	string host;
	
};

