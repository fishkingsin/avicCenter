#include "testApp.h"
#include "ofxXmlSettings.h"
//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
//	ofSetFullscreen(true);
	ofHideCursor();
	
	pano.setup();
	ofxXmlSettings xml;
    string host = "127.0.0.1";
    int port = 2838;
	if(xml.loadFile("configs.xml"));
	{
		string str;
		xml.copyXmlToString(str);
		
		if(xml.pushTag("DATA"))
		{
			if (xml.getValue("FULLSCREEN", 0)>0) {
				
				ofSetFullscreen(true);
			}
			ofSetLogLevel((ofLogLevel)xml.getValue("LOG_LEVEL", 0));
			

			pano.duration = xml.getValue("ANIMATION_DURATION", DEFAULT_DURATION);
            host = xml.getValue("IP", "127.0.0.1");
            port = xml.getValue("PORT", 2838);
		}
	}
	pano.init();
	commander.setup(host,port);

	ofAddListener(commander.events.messageUpdated , this, &testApp::messageUpdated);
}
void testApp::messageUpdated(ofMessage &msg)
{
	ofLogVerbose("messageUpdated") << "MEssage: " << msg.message;
	if(msg.message=="LEFT")
	{
		pano.keyPressed(OF_KEY_LEFT);
	}
	else if(msg.message=="RIGHT")
	{
		pano.keyPressed(OF_KEY_RIGHT);
	}
	else if (msg.message.find("ZONE_4_MAIN_")!=string::npos || msg.message.find("RESET_ALL")!=string::npos)
	{
		int start_idx = msg.message.find("ZONE_4_MAIN_");
		string sub = msg.message.substr(string("ZONE_4_MAIN_").length(),string::npos);
		ofLogVerbose()<<sub;
		int page_ = atoi(sub.c_str());
		ofLogVerbose("messageUpdated") << "should go to page "<< page_;
		pano.gotoPage( page_);
	}
    else if (msg.message.find("RESET_ALL")>0)
	{
		
		pano.gotoPage(0);
	}
}

//--------------------------------------------------------------
void testApp::update(){
	pano.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	pano.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	switch(key)
	{
		case 'f':
			ofToggleFullscreen();
			break;
	}
	pano.keyPressed(key);
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