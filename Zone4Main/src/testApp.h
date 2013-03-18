#pragma once

#include "ofMain.h"
#include "PanoApp.h"

#include "ofxUI.h"
//#define USE_SYPHON
#ifdef USE_SYPHON
#include "ofxSyphon.h"
#endif
#include "ofxYucolabCommander.h"
#define USE_RENDERMANAGER
#ifdef USE_RENDERMANAGER
#include "ofxRenderManager.h"
#endif
class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void exit();
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
	PanoApp pano;
	void drawGrid(float x, float y);
    
	void setGUI1();
	void setGUI2();
	void setGUI3();
	void setGUI4();
	
	ofxUICanvas *gui1;
	ofxUICanvas *gui2;
	ofxUICanvas *gui3;
    ofxUIScrollableCanvas *gui4;
	
    
	bool hideGUI;
	bool autoSave;
	bool bdrawGrid;
	bool bdrawPadding;
	
	void guiEvent(ofxUIEventArgs &e);
#ifdef USE_SYPHON
	ofxSyphonServer syphonServer;
	ofFbo fbo;
#endif
	ofxYucolabCommander commander;
	void messageUpdated(ofMessage &msg);
	int currentPage;
#ifdef USE_RENDERMANAGER
	ofxRenderManager rm;
	bool bDrawRM;
	int MWIDTH,MHEIGHT,MN_SCREEN;
	
	ofRectangle guiIn,guiOut;
#endif
	bool bShowRuler;
	bool bShowGrid;
	bool bShowCursor;
	float r,g,b;
	float gridX	, gridY;
	bool bShowContentGrid;
};
