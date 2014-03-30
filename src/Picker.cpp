#include "Picker.h"

Picker::Picker(){
}

ofVec3f Picker::project(ofVec3f _pnt){
	ofVec3f r;
	return r;
}

ofVec3f Picker::unproject(ofVec3f _pnt){
	GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;
 
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );
 
	ofVec3f unprojectedPoint;

    winX = (float) _pnt.x;
    winY = (float)viewport[3] - _pnt.y;
    glReadPixels( _pnt.x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
 
    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
 
	cout << "mouse position = " << posX << ", " << posY << ", " << posZ << endl;
	//cout << "cube postion = " << pos.x << ", " << pos.y << ", " << pos.z << endl;

	unprojectedPoint.set(posX, posY, posZ);
	return unprojectedPoint;
}