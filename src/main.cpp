#include "ofMain.h"
#include "testApp.h"

static GLfloat g_fMouseRotateMult = 0.5f; // Mouse 'Rotation' multiplier 
static GLfloat g_fMouseScaleMult  = 0.01f;  // Mouse 'Scale' multiplier

//========================================================================
int main( ){

	ofAppBaseWindow window;
	//ofAppGlutWindow window; // create a window
	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
	//ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);

	ofSetupOpenGL(1366,768,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new testApp());

}
