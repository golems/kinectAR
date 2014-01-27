#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fstream>

#include <pthread.h>

#if defined(__APPLE__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <sns.h>
#include <amino.h>
#include <ach.h>
#include <syslog.h>

#include <cv.h>
#include <cxcore.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/ModelCoefficients.h>
#include <pcl/filters/project_inliers.h>

#include <MarkerDetector.h>
#include <GlutViewer.h>

#include "kinectMarker.h"

struct SPoint
{
	double x,y,z;
};

struct tf_qv {
	double rotation[4];
	double translation[3];
};

struct sendMarker
{
	uint8_t counter;
	uint64_t visibleMask;
	struct tf_qv tfa[1];
	struct tf_qv tfk[1];
};

class KinectAR 
{
public:
	// constructor
	KinectAR();
	
	void DrawScene();
	void Keyboard(int key, int x, int y);
	
	void DetectMarkers();
	void CreatePointCloud();
	void OpenChannel(const char* channelName);
	void UpdateMarkerInfo();
	
	// ACH send message
	void SendMsg(size_t n);
	
public:
	pthread_t freenect_thread;
	volatile int die;
	uint16_t* depth;
	
	// for AR
	bool init=true;
	double marker_size;
	alvar::Camera cam;
	Drawable d[32];
	alvar::MarkerDetector<alvar::MarkerData> marker_detector;
	
	IplImage* image;
	
	// for ach
	ach_channel_t   channel;
	
	cv::Mat rgb;
	cv::Mat show; 
	cv::VideoCapture capture;
	cv::Mat depthMap;
	cv::Mat bgrImage;
	
	std::vector<KinectMarker> kinectMarkers;
	
};
