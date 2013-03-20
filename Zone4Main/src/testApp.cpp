#include "testApp.h"
#include "ofxXmlSettings.h"
void drawRuler()
{
	int length = 10;
	ofPushStyle();
	ofSetColor(125);
	ofRect(0,0,length,ofGetHeight());
	for(int y= 0; y < ofGetHeight() ; y+=5)
	{
		ofSetColor(0);
		ofLine(0, y, (y%10==0)?length:5, y);
	}
	ofSetColor(125);
	ofRect(0,0,ofGetWidth(),length);
	for(int x= 0; x < ofGetWidth() ; x+=5)
	{
		ofSetColor(0);
		ofLine(x, 0, x, (x%10==0)?length:5);
		
	}
	ofPopStyle();
}
//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofEnableSmoothing();
	ofSetLogLevel(OF_LOG_VERBOSE);
	pano.setup();
	glDisable(GL_DEPTH_TEST);
	
	ofxXmlSettings xml;
    string host = "127.0.0.1";
    int port = 2838;
    
#ifdef USE_PROJECTOR_BLEND
    int projector_width = PROJECTOR_WIDTH;
    int projector_height = PROJECTOR_HEIGHT;
    int projector_count = PROJECTOR_COUNT;
    int projector_overlap = PIXEL_OVERLAP;
#endif
	if(xml.loadFile("configs.xml"))
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
			if(xml.getValue("AUTO_START", 0)>0)pano.init();
#ifdef USE_RENDERMANAGER
			MN_SCREEN = xml.getValue("N_SCREEN",2);
			MWIDTH = xml.getValue("WIDTH",1024);
			MHEIGHT = xml.getValue("HEIGHT",768);
			
			rm.allocateForNScreens(MN_SCREEN,MWIDTH,MHEIGHT);
			rm.loadFromXml(xml.getValue("FBO_SETTING_PATH","fbo_settings.xml"));
			guiIn.set(xml.getValue("GUI_IN_X",200),
					  xml.getValue("GUI_IN_Y",200),
					  xml.getValue("GUI_IN_WIDTH",320),
					  xml.getValue("GUI_IN_HEIGHT",240));
			guiOut.set(xml.getValue("GUI_OUT_X",560),
					   xml.getValue("GUI_OUT_Y",200),
					   xml.getValue("GUI_OUT_WIDTH",320),
					   xml.getValue("GUI_OUT_HEIGHT",240));

			ofLogVerbose() <<guiIn.x <<" "<<  guiIn.y<<" "<<  guiIn.width<<" "<<  guiIn.height;
			ofLogVerbose() <<guiOut.x <<" "<<  guiOut.y<<" "<<  guiOut.width<<" "<<  guiOut.height;
			host = xml.getValue("IP", "127.0.0.1");
            port = xml.getValue("PORT", 2838);
#endif

#ifdef USE_PROJECTOR_BLEND
            projector_width = xml.getValue("PROJECTOR_WIDTH",PROJECTOR_WIDTH);
            projector_height = xml.getValue("PROJECTOR_HEIGHT",PROJECTOR_HEIGHT);
            projector_count = xml.getValue("PROJECTOR_COUNT",PROJECTOR_COUNT);
            projector_overlap = xml.getValue("PIXEL_OVERLAP",PIXEL_OVERLAP);
#endif

		}
	}
    else
    {
        ofLogError("config.xml")<< "error load config";
    }
#ifdef USE_PROJECTOR_BLEND
    ofEnableArbTex();
    blender.setup(projector_width,
                  projector_height,
                  projector_count,
                  projector_overlap);
//    blender.setShaderLocation("shaders/SmoothEdgeBlend");
    blender.gamma[0] = .5;
	blender.blendPower[0] = 1;
	blender.luminance[0] = 0;
    ofLogVerbose() << "canvas size: " << blender.getCanvasWidth() << " x " << blender.getCanvasHeight() << endl;
    ofLogVerbose() << "display size: " << blender.getDisplayWidth() << " x " << blender.getDisplayHeight() << endl;
#endif
	
	setGUI1();
	setGUI2();
    setGUI3();
    setGUI4();
	gui1->loadSettings("GUI/GUI1_Settings.xml");
	gui2->loadSettings("GUI/GUI2_Settings.xml");
	gui3->loadSettings("GUI/GUI3_Settings.xml");
	gui4->loadSettings("GUI/GUI4_Settings.xml");
    gui1->setDrawBack(false);
    gui2->setDrawBack(false);
    gui3->setDrawBack(false);
    gui4->setDrawBack(false);
	gui1->setVisible(false);
    gui2->setVisible(false);
    gui3->setVisible(false);
    gui4->setVisible(false);
	
#ifdef USE_SYPHON
	syphonServer.setName("Zone4Main");
	ofFbo::Settings settings;
	settings.width = 2048;
	settings.height = 768;
	settings.numSamples = 1;
	settings.internalformat = GL_RGBA;
	
	fbo.allocate(settings);
#endif
	commander.setup(host,port);
	
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
	else if (msg.message.find("ZONE_4_MAIN_")!=string::npos || msg.message.find("RESET_ALL")!=string::npos)
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
	if (name == "SHOW_MOUSE")
	{
		if(((ofxUIToggle*)e.widget)->getValue())
		{
			ofShowCursor();
		}
		else{
			ofHideCursor();
		}
	}
	if(name=="AUTO_SAVE")
	{
		autoSave = ((ofxUIToggle*)e.widget)->getValue();
        
	}
#ifdef USE_RENDERMANAGER
	else if(name=="RM_SAVE")
	{
		rm.saveToXml();
	}
	else if(name=="RM_RELOAD")
	{
		rm.reloadFromXml();
	}else if(name=="RM_RESET")
	{
		rm.resetCoordinates();
	}
#endif
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
	
#ifdef USE_RENDERMANAGER
	rm.startOffscreenDraw();
	ofClear(0);
#endif


#ifdef USE_SYPHON
	if(gui1->isVisible())
		fbo.draw(0,0);
#else
	if(bShowContent)
	{
		ofPushStyle();
		ofSetColor(255);
		pano.draw();
		ofPopStyle();
	}
	ofPushStyle();
	ofSetColor(255,0,0);
	if(bShowContentGrid)drawGrid(gridX	, gridY);
	ofPopStyle();
#endif
#ifdef USE_RENDERMANAGER
	rm.endOffscreenDraw();
	
   
#endif


#ifdef USE_PROJECTOR_BLEND
    glPushMatrix();


    blender.begin();
    ofBackground(r, g, b);
    //light gray backaground
//	ofSetColor(100, 100, 100);
//	ofRect(0, 0, blender.getCanvasWidth(), blender.getCanvasHeight());
	
	//thick grid lines for blending
	ofPushStyle();
	if(bShowGrid)drawGrid(gridX,gridY);
	ofPopStyle();
    ofSetColor(255);
    rm.drawScreens();
    //call when you are finished drawing
	blender.end();
    
    ofPushStyle();
	ofSetColor(255);
    glPushMatrix();
    
//    glScalef(0.5, 0.5, 0.5);
    blender.draw(0,0);
    glPopMatrix();
    ofPopStyle();
    glPopMatrix();
#else
#ifdef USE_RENDERMANAGER
    ofBackground(r, g, b);
    ofPushStyle();
	if(bShowGrid)drawGrid(gridX,gridY);
	ofPopStyle();

    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255);
    rm.drawScreens();
    ofPopStyle();
#endif
#endif
    
    
#ifdef USE_RENDERMANAGER
	if(bDrawRM)
	{
		rm.drawInputDiagnostically(guiIn.x,guiIn.y,guiIn.width,guiIn.height);
		rm.drawOutputDiagnostically(guiOut.x, guiOut.y, guiOut.width,guiOut.height);
	}
#endif
	if(bShowRuler)
	{
		drawRuler();
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
#ifdef USE_RENDERMANAGER
	if(!rm.keyPressedOutputPoint(key))
	{
		if(rm.keyPressedInputPoint(key))
		{
			return ;
		}
	}
	else{
		return;
	}
#endif
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
			ofDirectory dir;
			if(dir.listDir("snapshots")<0)
			{
				dir.createDirectory("snapshots");
			}
			img.saveImage("snapshots/OFXUI "+ofToString(ofGetYear())+"-"+ofToString(ofGetMonth())+"-"+ofToString(ofGetDay())+" at "+ofToString(ofGetHours(),2)+"."+ofToString(ofGetMinutes(),2)+"."+ofToString(ofGetSeconds(),2) +hr +".png");
		}
			break;
			
		case 'f':
			ofToggleFullscreen();
			break;
			
		case '\t':
            gui1->toggleVisible();
            gui2->toggleVisible();
            gui3->toggleVisible();
            gui4->toggleVisible();
			break;
		case 'm':
			bShowCursor = !bShowCursor;
			if(bShowCursor)
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
#ifdef USE_RENDERMANAGER
	
	if(!rm.mouseDragInputPoint(guiIn, ofVec2f(x,y)))
	{
		rm.mouseDragOutputPoint(guiOut, ofVec2f(x,y));
		
	}
#endif
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
#ifdef USE_RENDERMANAGER
	if(!rm.mouseSelectInputPoint(guiIn, ofVec2f(x,y)))
	{
		rm.mouseSelectOutputPoint(guiOut, ofVec2f(x,y));
		
	}
#endif
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
#ifdef USE_RENDERMANAGER
	
    float w = MWIDTH;
    float h = MHEIGHT;
#else
	float w = ofGetWidth();
	float h = ofGetHeight();
#endif
    
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
	gui1->addToggle( "SHOW_MOUSE", false, dim, dim);
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
	gui1->addToggle("SHOW_RULER", &bShowRuler);
	gui1->addToggle("SHOW_GRID",&bShowGrid);
	gui1->addToggle("SHOW_CONTENT_GRID",&bShowContentGrid);
	gui1->addToggle("SHOW_CONTENT",&bShowContent);
    gridX = 20;
    gridY = 20;
	gui1->addSlider("GRID_X",10,50,	&gridX);
	gui1->addSlider("GRID_Y",10,50,	&gridY);
	gui1->addSlider("BG_RED", 0.0, 255.0, &r);
	gui1->addSlider("BG_GREEN", 0.0, 255.0, &g);
	gui1->addSlider("BG_BLUE", 0.0, 255.0, &b);
	gui1->addSlider("PARTICLE_DECAY", 0.9, 1.0, &pano.particleSystem.decay);
    
    
	ofAddListener(gui1->newGUIEvent,this,&testApp::guiEvent);
}

void testApp::setGUI2()
{
	float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;
    
	
    gui2 = new ofxUICanvas(length+xInit+2, 0, length+xInit, ofGetHeight());
	gui2->addWidgetDown(new ofxUILabel("PANEL 2: MAPPING", OFX_UI_FONT_LARGE));
#ifdef USE_RENDERMANAGER
	gui2->addSlider("GUI_IN_X",0,ofGetWidth(), &guiIn.x);
	gui2->addSlider("GUI_IN_Y",0,ofGetWidth(), &guiIn.y);
	gui2->addSlider("GUI_OUT_X",0,ofGetWidth(), &guiOut.x);
	gui2->addSlider("GUI_OUT_Y",0,ofGetWidth(), &guiOut.y);
	
	gui2->addToggle("SHOW_RM" , &bDrawRM);
	gui2->addToggle("SHOW_ALIGN",&rm.myOffscreenTexture.bDebug);
	gui2->addButton("RM_SAVE" , false);
	gui2->addButton("RM_RELOAD",false);
	gui2->addButton("RM_RESET",false);
#endif
#ifdef USE_PROJECTOR_BLEND
    gui2->addLabel("BLENDING");
    gui2->addToggle("Show Blend", &blender.showBlend);
    for(int i = 0 ; i < blender.gamma.size() ; i++ )
    {
        gui2->addSlider("Blend Power"+ofToString(i), 0.0, 4.0, &blender.blendPower[i]);
        gui2->addSlider("Gamma"+ofToString(i), 0.1, 4.0, &blender.gamma[i]);
        gui2->addSlider("Luminance"+ofToString(i), 0.0, 4.0, &blender.luminance[i]);
    }
#endif
	ofAddListener(gui2->newGUIEvent,this,&testApp::guiEvent);
}

void testApp::setGUI3()
{
	float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;
	gui3 = new ofxUICanvas(length*2+xInit*2+4, 0, length+xInit, ofGetHeight());
    gui3->addWidgetDown(new ofxUILabel("PANEL 3: SCREEN", OFX_UI_FONT_LARGE));
#ifdef USE_RENDERMANAGER
	for(int i = 0 ; i <MN_SCREEN ;i++)
	{
		gui3->addToggle("ENABLE_SCREEN_"+ofToString(i), &rm.ENABLE_SCREEN[i]);
	}
#endif
	
	ofAddListener(gui3->newGUIEvent,this,&testApp::guiEvent);
}

void testApp::setGUI4()
{
	float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;
	gui4 = new ofxUIScrollableCanvas(length*3+xInit*3+6, 0, length+xInit, ofGetHeight());
    gui4->addWidgetDown(new ofxUILabel("PANEL 4: CONTROL", OFX_UI_FONT_LARGE));
#ifdef USE_RENDERMANAGER
	for(int i = 0 ; i <MN_SCREEN ;i++)
	{
		gui4->addToggle("CONTROL_SCREEN_"+ofToString(i), &rm.CONTROL_SCREEN[i]);
	}
#endif
    
	ofAddListener(gui4->newGUIEvent,this,&testApp::guiEvent);
}