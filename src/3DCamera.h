#ifndef __3DCAMERA__
#define __3DCAMERA__

#include <math.h>

/////////////////////////////////////////////////////////////////////////////
// Type Definitions:
//

typedef GLdouble		Frustum[6][4];		// Holds The Current Frustum Plane Equations

/////////////////////////////////////////////////////////////////////////////
// C3dCamera

class C3dCamera
{
//Construction
public:
    C3dCamera();
    virtual ~C3dCamera();

	void  ReInit();
//Implimentation
public:
//	virtual void Serialize(CArchive& ar, int iVersion);

	void PositionCamera();
	void ResetView(int w=0, int h=0);
	void GetWorldCoord(int ix, int iy, GLdouble fz, SG_VECTOR& coord);
	void GetScreenCoord(GLdouble wX, GLdouble wY, GLdouble wZ,
							double& scrX, double& scrY, double& scrZ);

	void GetEyePos(GLdouble *x, GLdouble *y, GLdouble *z);
	void GetLookAtPos(GLdouble *x, GLdouble *y, GLdouble *z);
	void GetUpVector(GLdouble *x, GLdouble *y, GLdouble *z);
	GLdouble GetFocalLength();
	void SetFocalLength(GLdouble length, BOOL bLookAtPos=TRUE);
	void GetRotationAboutLookAt(GLdouble *x, GLdouble *y, GLdouble *z);
	void GetRotationAboutEye(GLdouble *x, GLdouble *y, GLdouble *z);
	void SetEyePos(GLdouble x, GLdouble y, GLdouble z);
	void SetLookAtPos(GLdouble x, GLdouble y, GLdouble z);
	void SetUpVector(GLdouble x, GLdouble y, GLdouble z);
	void SetRotationAboutLookAt(GLdouble x, GLdouble y, GLdouble z);
	void SetRotationAboutEye(GLdouble x, GLdouble y, GLdouble z);
	void SetFarClipPlane(GLdouble fFar);
	void SetNearClipPlane(GLdouble fNear);

	void CalculateFocalLength();
	void CalculateYawPitchRoll();
	void CalculateUpVector();
	void FitBounds(double minX, double minY, double minZ, 
		double maxX, double maxY,double maxZ);
	void ExtractFrustum();

	void GetRotationMatrix(sgCMatrix& XformMatrix);
	void GetInvRotationMatrix(sgCMatrix& XformMatrix);

	void GetVectorsForRayTracingCamera(SG_VECTOR& eye_loc,
		SG_VECTOR& eye_look_at,
		SG_VECTOR& eye_y,
		SG_VECTOR& eye_x);
//Attributes
protected:

public:
	BOOL			m_bBuildLists;
	BOOL			m_bResetClippingPlanes;
	unsigned int	m_iDisplayLists;
	int				m_bPerspective;
	GLsizei			m_iScreenWidth;
	GLsizei			m_iScreenHeight;

	GLdouble			m_fFovY;		// Y-Axis field of view
	GLdouble			m_fAspect;		// width(x) to height(y) aspect
	GLdouble			m_fLeft;
	GLdouble			m_fRight;
	GLdouble			m_fBottom;
	GLdouble			m_fTop;
	GLdouble			m_fNear;
	GLdouble			m_fFar;
	GLint			    m_iViewport[4];
	SG_POINT			m_fEyePos;
	SG_POINT			m_fLookAtPos;
	SG_VECTOR			m_fUpVector;
	GLdouble			m_fFocalLength;
	GLdouble		m_fPitch;				// Rotation about the X-Axis
	GLdouble		m_fRoll;				// Rotation about the Y-Axis
	GLdouble		m_fYaw;					// Rotation about the Z-Axis
	GLdouble		m_dModelViewMatrix[16];
	GLdouble		m_dProjectionMatrix[16];
	Frustum			m_fFrustum;

};
/////////////////////////////////////////////////////////////////////////////
#endif

//////