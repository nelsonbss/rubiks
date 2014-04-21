#include "Picker.h"

Picker::Picker(){
}

ofVec3f Picker::project(ofVec3f _pnt){
	ofVec3f r;
	return r;
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
    //if(_vp == NULL){
		glGetIntegerv( GL_VIEWPORT, vp );
	//} else {
	//	vp[0] = (GLint)_vp->x;
	//	vp[1] = (GLint)_vp->y;
	//	vp[2] = (GLint)_vp->width;
	//	vp[3] = (GLint)_vp->height;
	//}
 
	for(int i = 0; i < 4; i++){
		cout << "vp " << i << " = " << vp[i] << endl;
	}

	ofVec3f unprojectedPoint;

    winX = (float) _pnt.x;
    //winY = (float)vp[3] - _pnt.y;
    winY = ofGetHeight() - _pnt.y;
	glReadPixels( _pnt.x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
 
    gluUnProject( winX, winY, winZ, modelview, projection, vp, &posX, &posY, &posZ);
 
	cout << "mouse position = " << posX << ", " << posY << ", " << posZ << endl;
	//cout << "cube postion = " << pos.x << ", " << pos.y << ", " << pos.z << endl;

	unprojectedPoint.set(posX, posY, posZ);
	return unprojectedPoint;
}