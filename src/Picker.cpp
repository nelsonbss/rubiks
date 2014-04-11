#include "Picker.h"

Picker::Picker(){
}

ofVec3f Picker::project(ofVec3f _pnt, ofRectangle* _vp){
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLdouble x;
	GLdouble y;
	GLdouble z;
	GLdouble modelView[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
	GLdouble projection[16];
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	GLint vp[4];
	glGetIntegerv(GL_VIEWPORT, vp);
	gluProject(_pnt.x, _pnt.y, _pnt.z, modelView, projection, vp, &x, &y, &z);
	y = ofGetHeight() - y;
	return ofVec3f(x, y, z);
}

ofVec3f Picker::unproject(ofVec3f _pnt, ofRectangle* _vp){
	cout << "unprojecting " << _pnt.x << ", " << _pnt.y << ", " << _pnt.z << endl;
	GLint vp[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, vp );
 
	for(int i = 0; i < 4; i++){
		cout << "vp " << i << " = " << vp[i] << endl;
	}

	ofVec3f unprojectedPoint;

    winX = (float) _pnt.x;
    winY = ofGetHeight() - _pnt.y;
	glReadPixels( _pnt.x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
 
    gluUnProject( winX, winY, winZ, modelview, projection, vp, &posX, &posY, &posZ);
 
	cout << "mouse position = " << posX << ", " << posY << ", " << posZ << endl;
	//cout << "cube postion = " << pos.x << ", " << pos.y << ", " << pos.z << endl;

	unprojectedPoint.set(posX, posY, posZ);
	return unprojectedPoint;
}