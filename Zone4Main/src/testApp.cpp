#include "testApp.h"
#include "ofxXmlSettings.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetFrameRate(60);
	ofEnableSmoothing();
	ofSetLogLevel(OF_LOG_VERBOSE);
	pano.setup();
	glEnable(GL_DEPTH_TEST);

	ofxXmlSettings xml;
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
			
			
			if(xml.getValue("AUTO_START", 0)>0)pano.init();
		}
	}
	
	setGUI1();
	setGUI2();
    setGUI3();
    setGUI4();
	gui1->loadSettings("GUI/GUI1_Settings.xml");
	gui2->loadSettings("GUI/GUI2_Settings.xml");
	gui3->loadSettings("GUI/GUI3_Settings.xml");
	gui4->loadSettings("GUI/GUI4_Settings.xml");
    gui1->setDrawBack(true);
    gui2->setDrawBack(true);
    gui3->setDrawBack(true);
    gui4->setDrawBack(true);
	#ifdef USE_SYPHON
	syphonServer.setName("Zone4Main");
	ofFbo::Settings settings;
	settings.width = 2048;
	settings.height = 768;
	settings.numSamples = 1;
	settings.internalformat = GL_RGBA;

	fbo.allocate(settings);
#endif
	commander.setup("127.0.0.1",2838);
	
	ofAddListener(commander.events.messageUpdated , this, &testApp::messageUpdated);
					ofHideCursor();
}
void testApp::exit()
{
	if(autoSave)
	{
		gui1->saveSettings("GUI/GUI1_Settings.xml");
		gui2->saveSettings("GUI/GUI2_Settings.xml");
		gui3->saveSettings("GUI/GUI3_Settings.xml");
		gui4->saveSettings("GUI/GUI4_Settings.xml");
	}
	delete gui1;
	delete gui2;
    delete gui3;
    delete gui4;
}
void testApp::messageUpdated(ofMessage &msg)
{
	if(msg.message=="LEFT")
	{
		pano.keyPressed(OF_KEY_LEFT);
	}
	else if(msg.message=="RIGHT")
	{
		pano.keyPressed(OF_KEY_RIGHT);
	}
	else if (msg.message.find("ZONE_4_MAIN_")!=string::npos)
	{
		int start_idx = msg.message.find("ZONE_4_MAIN_");
		string sub = msg.message.substr(string("ZONE_4_MAIN_").length(),string::npos);
		ofLogVerbose()<<sub;
		int page_ = atoi(sub.c_str());
		ofLogVerbose("messageUpdated") << "should go to page "<< page_;
		pano.gotoPage( page_);
	}
}
//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	int kind = e.widget->getKind();
	ofLogVerbose() << "got event from: " << name << " kind " << kind << endl;
	if(name=="AUTO_SAVE")
	{
		autoSave = ((ofxUIToggle*)e.widget)->getValue();
	}
}
//--------------------------------------------------------------
void testApp::update(){
	//	ofLogVerbose() << "lastframetime " << ofGetLastFrameTime();

	if(ofGetFrameNum()==100)
	{
		
		//		pano.init();
	}
	else{
		pano.update();
		
	}
	
	#ifdef USE_SYPHON
	ofPushStyle();
	ofSetColor(255);
	fbo.begin();
	ofClear(0);
	pano.draw();
	fbo.end();
	syphonServer.publishTexture(&fbo.getTextureReference());
	ofPopStyle();

#endif
}

//--------------------------------------------------------------
void testApp::draw(){
	ofEnableAlphaBlending();
	ofBackground(0);
	#ifdef USE_SYPHON
	if(gui1->isVisible())
		fbo.draw(0,0);
#else
	pano.draw();
#endif
	
		
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(gui2->hasKeyboardFocus())
    {
        return;
    }
	switch (key)
	{
		case '`':
		{
			string hr;
			ofImage img;
			img.grabScreen(0,0,ofGetWidth(), ofGetHeight());
			if(ofGetHours() < 12)
			{
				hr = " AM";
			}
			else
			{
				hr = " PM";
			}
			img.saveImage("snapshots/OFXUI "+ofToString(ofGetYear())+"-"+ofToString(ofGetMonth())+"-"+ofToString(ofGetDay())+" at "+ofToString(ofGetHours(),2)+"."+ofToString(ofGetMinutes(),2)+"."+ofToString(ofGetSeconds(),2) +hr +".png");
		}
			break;
			
		case 'f':
			ofToggleFullscreen();
			break;
			
		case 'h':
            gui1->toggleVisible();
            gui2->toggleVisible();
            gui3->toggleVisible();
            gui4->toggleVisible();
			if(gui1->isVisible())
			{
				ofShowCursor();
			}
			else{
				ofHideCursor();
			}
			break;
			
		case 'p':
			bdrawPadding = !bdrawPadding;
			gui1->setDrawWidgetPaddingOutline(bdrawPadding);
			gui2->setDrawWidgetPaddingOutline(bdrawPadding);
			gui3->setDrawWidgetPaddingOutline(bdrawPadding);
			gui4->setDrawWidgetPaddingOutline(bdrawPadding);
			break;
			
		case '[':
			gui1->setDrawWidgetPadding(false);
			gui2->setDrawWidgetPadding(false);
			gui3->setDrawWidgetPadding(false);
			gui4->setDrawWidgetPadding(false);
			break;
			
		case ']':
			gui1->setDrawWidgetPadding(true);
			gui2->setDrawWidgetPadding(true);
			gui3->setDrawWidgetPadding(true);
			gui4->setDrawWidgetPadding(true);
			break;
			
        case '1':
            gui1->toggleVisible();
            break;
			
        case '2':
            gui2->toggleVisible();
            break;
			
        case '3':
            gui3->toggleVisible();
            break;
			
        case '4':
            gui4->toggleVisible();
            break;
		case 's':
			gui1->saveSettings("GUI/GUI1_Settings.xml");
			gui2->saveSettings("GUI/GUI2_Settings.xml");
			gui3->saveSettings("GUI/GUI3_Settings.xml");
			gui4->saveSettings("GUI/GUI4_Settings.xml");
			break;
		default:
			break;
	}
	pano.keyPressed(key);
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
	
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
//--------------------------------------------------------------
void testApp::drawGrid(float x, float y)
{
    float w = ofGetWidth();
    float h = ofGetHeight();
    
    for(int i = 0; i < h; i+=y)
    {
        ofLine(0,i,w,i);
    }
    
    for(int j = 0; j < w; j+=x)
    {
        ofLine(j,0,j,h);
    }
}
//--------------------------------------------------------------
void testApp::setGUI1()
{

	
	float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;
	hideGUI = false;
	
	gui1 = new ofxUICanvas(0, 0, length+xInit, ofGetHeight());
	gui1->addWidgetDown(new ofxUILabel("PANEL 1: BASICS", OFX_UI_FONT_LARGE));
    gui1->addWidgetDown(new ofxUILabel("Press 'h' to Hide GUIs", OFX_UI_FONT_LARGE));
	
	gui1->addSpacer(length-xInit, 2);
    gui1->addWidgetDown(new ofxUILabel("FPS LABEL", OFX_UI_FONT_MEDIUM));
    gui1->addWidgetDown(new ofxUIFPS(OFX_UI_FONT_MEDIUM));
	
	gui1->addSpacer(length-xInit, 2);
	gui1->addToggle( "AUTO_SAVE", false, dim, dim);
	gui1->addSpacer(length-xInit, 2);
	vector<string> items;
    items.push_back("VERBOSE");
	items.push_back("NOTICE");
	items.push_back("WARNING");
	items.push_back("ERROR");
	items.push_back("FATAL_ERROR");
	items.push_back("SILENT");
    
    gui1->addDropDownList("LOG_LEVEL", items, 200);
	
	ofAddListener(gui1->newGUIEvent,this,&testApp::guiEvent);
}

void testApp::setGUI2()
{
	float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;
    
	
    gui2 = new ofxUICanvas(length+xInit+2, 0, length+xInit, ofGetHeight());
	gui2->addWidgetDown(new ofxUILabel("PANEL 2: RESERVED", OFX_UI_FONT_LARGE));
	
	ofAddListener(gui2->newGUIEvent,this,&testApp::guiEvent);
}

void testApp::setGUI3()
{
	float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;
	gui3 = new ofxUICanvas(length*2+xInit*2+4, 0, length+xInit, ofGetHeight());
    gui3->addWidgetDown(new ofxUILabel("PANEL 3: RESERVED", OFX_UI_FONT_LARGE));
	
	
	ofAddListener(gui3->newGUIEvent,this,&testApp::guiEvent);
}

void testApp::setGUI4()
{
	float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;
	gui4 = new ofxUIScrollableCanvas(length*3+xInit*3+6, 0, length+xInit, ofGetHeight());
    gui4->addWidgetDown(new ofxUILabel("PANEL 4: SCROLLABLE", OFX_UI_FONT_LARGE));
    
    
    
	ofAddListener(gui4->newGUIEvent,this,&testApp::guiEvent);
}