#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
//	ofSetFullscreen(true);
	ofHideCursor();
	
	pano.setup();
	pano.init();
	commander.setup("127.0.0.1",2838);

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