// myglwidget.cpp

#include <QGLWidget>
#include "VDRCOpenGLWidget.h"
#include "constForVDRCOpenGLWidget.h"
#include <iostream>

VDRCOpenGLWidget::VDRCOpenGLWidget(QWidget *parent)
	: QOpenGLWidget(parent),
localOrigin(0.0, 0.0, 0.0),
localX(1.0, 0.0, 0.0),
localY(0.0, 1.0, 0.0),
localZ(0.0, 0.0, 1.0)
{
	positionLight[0][0] = 20.0f;
	positionLight[0][1] = 20.0f;
	positionLight[0][2] = 20.0f;
	positionLight[0][3] = 0.0f;

	positionLight[1][0] = -positionLight[0][0];
	positionLight[1][1] = -positionLight[0][1];
	positionLight[1][2] = -positionLight[0][2];
	positionLight[1][3] = -positionLight[0][3];
	////////////
	positionLight[2][0] = positionLight[0][0];
	positionLight[2][1] = -positionLight[0][1];
	positionLight[2][2] = positionLight[0][2];
	positionLight[2][3] = -positionLight[0][3];
	/////////////
	specularLight[0] = 0.35f;
	specularLight[1] = 0.35f;
	specularLight[2] = 0.35f;
	specularLight[3] = 0.0f;

	diffuseLight[0] = 1.0f;
	diffuseLight[1] = 1.0f;
	diffuseLight[2] = 1.0f;
	diffuseLight[3] = 0.0f;

	ambientLight[0] = 0.5f;
	ambientLight[1] = 0.5f;
	ambientLight[2] = 0.5f;
	ambientLight[3] = 0.0f;

	ambientMaterial[0] = 0.1f;
	ambientMaterial[1] = 0.1f;
	ambientMaterial[2] = 0.1f;
	ambientMaterial[3] = 0.0f;

	specularMaterial[0] = 0.5f;
	specularMaterial[1] = 0.5f;
	specularMaterial[2] = 0.5f;
	specularMaterial[3] = 0.0f;

	emittedMaterial[0] = 0.0f;
	emittedMaterial[1] = 0.0f;
	emittedMaterial[2] = 0.0f;
	emittedMaterial[3] = 0.0f;
	shininess = 25.0f;

	m_bDisableRotation = false;
}

VDRCOpenGLWidget::~VDRCOpenGLWidget()
{
	gluDeleteQuadric(qObj);
}

QSize VDRCOpenGLWidget::minimumSizeHint() const
{
	return QSize(50, 50);
}

QSize VDRCOpenGLWidget::sizeHint() const
{
	return QSize(400, 400);
}

void VDRCOpenGLWidget::initializeGL()
{
	glClearColor(1, 1, 1, 0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearDepth(10000);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	static GLfloat lightPosition[4] = { 0, 0, 10, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	glEnable(GL_COLOR_MATERIAL);

	qObj = gluNewQuadric();
	gluQuadricNormals(qObj, GLU_SMOOTH);

	glLineStipple(STIPPLE_FACTOR, STIPPLE_PATTERN);
}

void VDRCOpenGLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	const rg_Point3D target(localOrigin);
	const rg_Point3D eyePt = localOrigin + localZ * m_eyeDistance;
	const rg_Point3D up(localY);

	gluLookAt(eyePt.getX(), eyePt.getY(), eyePt.getZ(),
		target.getX(), target.getY(), target.getZ(),
		up.getX(), up.getY(), up.getZ());

	glInitNames();
	glPushName(0);

	draw();

	glPopMatrix();
}

void VDRCOpenGLWidget::resizeGL(int width, int height)
{
	// select the full client area
	glViewport(0, 0, width, height);

	// compute the aspect ratio
	// this will keep all dimension scales equal
	float aspect_ratio = (GLdouble)width / (GLdouble)height;

	// select the projection matrix and clear it
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// select the viewing volume
	gluPerspective(45.0f, aspect_ratio, .1f, MAX_DEPTH);

	// switch back to the modelview matrix and clear it
	glMatrixMode(GL_MODELVIEW);
}



void VDRCOpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
	int dx = event->x() - lastPos.x();
	int dy = event->y() - lastPos.y();

	float angleScale = 0.01f;

	if (!m_bDisableRotation)
		rotate_eye_position(angleScale*-dy, angleScale*-dx, 0.0);

	lastPos = event->pos();
	update();
}



void VDRCOpenGLWidget::wheelEvent(QWheelEvent* event)
{
	int delta = event->delta();
	if (delta > 0)
	{
		for (int i = 0; i < delta; i++)
			m_eyeDistance /= ZOOM_RATIO;
	}
	else
	{
		for (int i = 0; i < -delta; i++)
			m_eyeDistance *= ZOOM_RATIO;
	}
	update();
}



bool VDRCOpenGLWidget::initialize_eye_position()
{
	const rg_Point3D target(localOrigin);
	const rg_Point3D eyePt = localOrigin + localZ * m_eyeDistance;
	const rg_Point3D up(localY);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	gluLookAt(eyePt.getX(), eyePt.getY(), eyePt.getZ(),
		target.getX(), target.getY(), target.getZ(),
		up.getX(), up.getY(), up.getZ());

	return true;
}

bool VDRCOpenGLWidget::rotate_eye_position(float angleX, float angleY, float angleZ)
{
	rg_TMatrix3D transformMat;
	transformMat.rotateArbitraryAxis(localX, angleX);
	transformMat.rotateArbitraryAxis(localY, angleY);
	transformMat.rotateArbitraryAxis(localZ, angleZ);

	localX = transformMat*localX;
	localY = transformMat*localY;
	localZ = transformMat*localZ;
	return TRUE;
}



void VDRCOpenGLWidget::set_eye_direction(rg_Point3D eyeDirection)
{
	localZ = eyeDirection.getUnitVector();
	localX = rg_Point3D(0, 1, 0).crossProduct(localZ).getUnitVector();
	localY = localZ.crossProduct(localX).getUnitVector();
}

/*
bool VDRCOpenGLWidget::rotate_eye_position_at_local_cener_point(float angleX, float angleY, float angleZ, rg_TMatrix3D& localCenterPt)
{
	m_transform.makeIdentity();

	m_transform.rotateArbitraryAxis(localX, angleX);
	m_transform.rotateArbitraryAxis(localY, angleY);
	m_transform.rotateArbitraryAxis(localZ, angleZ);

	localX = m_transform*localX;
	localY = m_transform*localY;
	localZ = m_transform*localZ;

	localCenterPt = m_transform;

	return TRUE;
}*/



void VDRCOpenGLWidget::get_rotation(const rg_TMatrix3D& rotationMatrix, float*& rotationFactors) const
{
	float* angleX = new float[2];
	float* angleY = new float[2];
	float* angleZ = new float[2];


	rg_Matrix rotationForMovingSet = rotationMatrix;

	if (!rg_EQ(rotationForMovingSet.getElement(2, 0), 1.0) &&
		!rg_EQ(rotationForMovingSet.getElement(2, 0), -1.0)) {

		angleY[0] = -asin(rotationForMovingSet.getElement(2, 0));
		angleY[1] = rg_PI - angleY[0];

		angleX[0] = atan2((rotationForMovingSet.getElement(2, 1) / cos(angleY[0])),
			(rotationForMovingSet.getElement(2, 2) / cos(angleY[0])));
		angleX[1] = atan2((rotationForMovingSet.getElement(2, 1) / cos(angleY[1])),
			(rotationForMovingSet.getElement(2, 2) / cos(angleY[1])));

		angleZ[0] = atan2((rotationForMovingSet.getElement(1, 0) / cos(angleY[0])),
			(rotationForMovingSet.getElement(0, 0) / cos(angleY[0])));
		angleZ[1] = atan2((rotationForMovingSet.getElement(1, 0) / cos(angleY[1])),
			(rotationForMovingSet.getElement(0, 0) / cos(angleY[1])));
	}
	else {
		angleZ[0] = angleZ[1] = 0.0;

		float delta = atan2(rotationForMovingSet.getElement(0, 1), rotationForMovingSet.getElement(0, 2));

		if (rg_EQ(rotationForMovingSet.getElement(2, 0), -1.0)) {
			angleY[0] = angleY[1] = rg_PI / 2.0;

			angleX[0] = angleX[1] = angleZ[0] + delta;
		}
		else {
			angleY[0] = angleY[1] = -1 * (angleY[1] = rg_PI / 2.0);

			angleX[0] = angleX[1] = (-1 * angleZ[0]) + delta;
		}
	}

	if (rotationFactors != rg_NULL)
		delete[] rotationFactors;

	rotationFactors = new float[3];

	rotationFactors[0] = angleX[0];
	rotationFactors[1] = angleY[0];
	rotationFactors[2] = angleZ[0];

	delete[] angleX;
	delete[] angleY;
	delete[] angleZ;
}



void VDRCOpenGLWidget::draw_sphere(const rg_Point3D& center, const float& radius, const Color3f& color, const float& A /*= 1.0*/, const int& elementID /*= -1*/) const
{
	glPushMatrix();

	if (elementID != -1)
	{
		glLoadName(elementID);
	}

	glColor4f(color.getR(), color.getG(), color.getB(), A);
	glTranslated(center.getX(), center.getY(), center.getZ());
	gluSphere(qObj, radius, SPHERE_RESOLUTION, SPHERE_RESOLUTION);
	glPopMatrix();
}



void VDRCOpenGLWidget::draw_point(rg_Point3D& pt, const float& ptSize, const Color3f& color, const float& A /*= 1.0*/, const int& elementID /*= -1*/) const
{
	glDisable(GL_LIGHTING);

	if (elementID != -1)
	{
		glLoadName(elementID);
	}

	glPointSize(ptSize);

	glColor4f(color.getR(), color.getG(), color.getB(), A);
	glBegin(GL_POINTS);
	glVertex3f(pt.getX(), pt.getY(), pt.getZ());
	glEnd();

	glEnable(GL_LIGHTING);
}



void VDRCOpenGLWidget::draw_line(const rg_Point3D& pt1, const rg_Point3D& pt2, const float& width, const Color3f& color, const float& A /*= 1.0*/) const
{
	glDisable(GL_LIGHTING);
	glLineWidth(width);
	glColor4f(color.getR(), color.getG(), color.getB(), A);
	glBegin(GL_LINES);
	glVertex3f(pt1.getX(), pt1.getY(), pt1.getZ());
	glVertex3f(pt2.getX(), pt2.getY(), pt2.getZ());
	glEnd();
	glEnable(GL_LIGHTING);
}

void VDRCOpenGLWidget::draw_line_stipple(const rg_Point3D& pt1, const rg_Point3D& pt2, const float& thickness, const Color3f& color, const float& A /*= 1.0*/) const
{
	glEnable(GL_LINE_STIPPLE);
	draw_line(pt1, pt2, thickness, color, A);
	glDisable(GL_LINE_STIPPLE);
}

void VDRCOpenGLWidget::draw_face(const array<rg_Point3D, 3>& points, const rg_Point3D& normal, const Color3f& color, const float& A /*= 1.0*/, const int& elementID /*= -1*/) const
{
	if (elementID != -1)
		glLoadName(elementID);

	glColor4f(color.getR(), color.getG(), color.getB(), A);
	glBegin(GL_TRIANGLES);
	glNormal3f(normal.getX(), normal.getY(), normal.getZ());
	for (int i = 0; i < 3; i++)
		glVertex3f(points.at(i).getX(), points.at(i).getY(), points.at(i).getZ());
	glEnd();
	
	/*if (elementID != -1)
		glPopName();*/
}



void VDRCOpenGLWidget::draw_triangle(const array<rg_Point3D, 3>& points, const Color3f& color, const float& A /*= 1.0*/) const
{
	glColor4f(color.getR(), color.getG(), color.getB(), A);
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 3; i++)
	{
		glVertex3f(points.at(i).getX(), points.at(i).getY(), points.at(i).getZ());
	}
	glEnd();
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
}



void VDRCOpenGLWidget::draw_octagonal_cone(const rg_Point3D& base, const rg_Point3D& tip, const float& radius, const Color3f& color, const float& A) const
{
	rg_Point3D nVector = tip - base;
	float orthY = radius*sqrt(1 / (1 + pow(nVector.getY(), 2) / pow(nVector.getZ(), 2)));
	float orthZ = -nVector.getY() / nVector.getZ()*orthY;
	rg_Point3D vVector(0, orthY, orthZ);
	rg_Point3D uVector = nVector.crossProduct(vVector);
	uVector.normalize();
	uVector*radius;
	rg_Point3D baseOctagonPt[8];
	baseOctagonPt[0] = base + vVector;
	baseOctagonPt[1] = base + 0.5*vVector + 0.5*uVector;
	baseOctagonPt[2] = base + uVector;
	baseOctagonPt[3] = base - 0.5*vVector + 0.5*uVector;
	baseOctagonPt[4] = base - vVector;
	baseOctagonPt[5] = base - 0.5*vVector - 0.5*uVector;
	baseOctagonPt[6] = base - uVector;
	baseOctagonPt[7] = base + 0.5*vVector - 0.5*uVector;

	for (int i = 0; i < 7; i++)
	{
		array<rg_Point3D, 3> bevelPoints = { baseOctagonPt[i], baseOctagonPt[i + 1], tip };
		array<rg_Point3D, 3> basePoints = { baseOctagonPt[i + 1], baseOctagonPt[i], base};
		draw_triangle(bevelPoints, color, A);
		draw_triangle(basePoints, color, A);
	}
	array<rg_Point3D, 3> bevelPoints = { baseOctagonPt[7], baseOctagonPt[0], tip };
	array<rg_Point3D, 3> basePoints = { baseOctagonPt[0], baseOctagonPt[7], base };
	draw_triangle(bevelPoints, color, A);
	draw_triangle(basePoints, color, A);
}



void VDRCOpenGLWidget::pick_object(int x, int y)
{
	GLuint selectBuff[64];                                // <1>
	GLint viewport[4];                              // <2>

	glSelectBuffer(64, selectBuff);                       // <3>
	glGetIntegerv(GL_VIEWPORT, viewport);                 // <4>
	glMatrixMode(GL_PROJECTION);                          // <5>
	glPushMatrix();                                       // <6>
	glRenderMode(GL_SELECT);                              // <7>
	glLoadIdentity();                                     // <8>

	QSize widgetSize = size();
	float aspect_ratio = (GLdouble)widgetSize.width() / (GLdouble)widgetSize.height();

	float xRatio = (float)viewport[2] / (float)widgetSize.width();
	float yRatio = (float)viewport[3] / (float)widgetSize.height();

	float x_trans = (float)x *xRatio;
	float y_trans = (float)y *yRatio;


	gluPickMatrix(x_trans, viewport[3] - y_trans, 1, 1, viewport);
	gluPerspective(45.0f, aspect_ratio, .1f, MAX_DEPTH);

	paintGL(); 

	int hits = glRenderMode(GL_RENDER); 
	glMatrixMode(GL_PROJECTION);
	glPopMatrix(); 
	glMatrixMode(GL_MODELVIEW);

	process_picking(hits, selectBuff);
}