#include "testApp.h"
#include "ofAppGlutWindow.h"

//--------------------------------------------------------------
int main(){
	ofAppGlutWindow window; // create a window
	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
#if defined (TARGET_OSX)
	window.setGlutDisplayString("rgba double samples>=4 depth");
#endif
	ofSetupOpenGL(&window, 2048, 768, OF_WINDOW);
	ofRunApp(new testApp()); // start the app
}
