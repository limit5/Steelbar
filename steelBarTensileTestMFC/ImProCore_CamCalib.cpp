#include "stdafx.h"
#include <stdlib.h>
#include <sstream>
#include <string>

#include "ImProCore_CamCalib.h"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/legacy/legacy.hpp"

#include "gettime.h"

#ifdef _WIN32
#define SPRINTF sprintf_s 
#else
#define SPRINTF sprintf
#endif

using namespace std;

CamCalib::CamCalib(int nCamera)
{
  this->nCam            = nCamera;
  nImage				= 0;
  nFound				= 0;
  calibrated            = false; 
  patternSize.width     = 0;
  patternSize.height    = 0;
  cellWidth             = 0.0;
  cellHeight            = 0.0; 
  calibFlag             = 0;
  calibImgSize_0.width  = 0;
  calibImgSize_0.height = 0;
  calibImgSize_1.width  = 0;
  calibImgSize_1.height = 0;  
  debugLevel            = 1;
  drawChessCorners      = true; 
  drawnCornersSuffix    = "";
  reSize.width          = 0;
  reSize.height         = 0;

  // check nCam
  if ( this->nCam < 1 || this->nCam > 2 ) {
    //if ( debugLevel >= 1 ) {
    //  cerr << "Error-CamCalib: Invalid # of cameras: " << nCam << endl;
    //}
    SPRINTF(errorMessage, "Error: CamCalib: Invalid # of cameras: %d", nCam); 
  }
}

CamCalib::~CamCalib(void)
{
}

int CamCalib::addCalibImg(string file, int camId)
{
	MessageBoxA(NULL, "addCalibImg: start...", "msg", NULL);
	//MessageBoxA(NULL, "check4", "checkbox4", NULL);
  // check if pattern size is assigned.
  if ( patternSize.width < 1 || patternSize.height < 1 ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Warning: Assign CamCalib pattern size before adding file.\n";
    SPRINTF(errorMessage, "Warning: CamCalib: Please assign pattern size before adding file."); 
    return -1; 
  }
  // check if camId is valid
  if ( camId < 0 || camId >= this->nCam ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Warning: Invalid camId . Taking it as 0 (left cam).\n";
    // camId = 0;
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camera ID: %d.", camId); 
    return -1; 
  }
  //MessageBoxA(NULL, "check5", "checkbox5", NULL);
  // add the image file name into a file-name vector
  if ( camId == 0 ) 
    calibImgFiles_0.push_back(file);
  else
    calibImgFiles_1.push_back(file);

  // open the image file
  double t0 = getWallTime();
  cv::Mat myCalibImg = cv::imread(file, CV_LOAD_IMAGE_GRAYSCALE );
  //if ( debugLevel >= 4 ) {
  //  cout << "File access time: " 
  //       << (clock()-t0)/CLOCKS_PER_SEC << " sec.\n";
  //}
  // Check image size. If zero, it fails to load image.
  //MessageBoxA(NULL, "check6", "checkbox6", NULL);
  if ( myCalibImg.cols == 0 || myCalibImg.rows == 0 ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Warning: Could not open " << file << endl;
    SPRINTF(errorMessage, "Warning: CamCalib: Could not open %s.", file.c_str()); 
    return -1;
  }
  //MessageBoxA(NULL, "check7", "checkbox7", NULL);
  //cv::Size Temp2;
  //double scaleRatio;
  //if (myCalibImg.cols > 1500){
	 // Temp2.width = 1500;
	 // scaleRatio = (double)Temp2.width / (double)myCalibImg.cols;
	 // Temp2.height = (double)(myCalibImg.rows * scaleRatio);
  //}
  //char StringTest[256];
  //SPRINTF(StringTest, "%d %d", Temp2.width, Temp2.height);
  //MessageBoxA(NULL, StringTest, "checkbox8", NULL);
  //cv::Mat Temp;
  //cv::resize(myCalibImg, myCalibImg, cv::Size(Temp2.width, Temp2.height), 0, 0, CV_INTER_CUBIC);
  //reSize.width = Temp2.width;
  //reSize.height = Temp2.height;
  //myCalibImg.rows = reSize.width;
  //myCalibImg.cols = reSize.height;
  //Temp = myCalibImg;

  // Check calib. image size.
  if ( camId == 0) {
    // if it is the first image of this cam, remember its image size.
    // else, check the image sizes are consistent 
    if ( calibImgSize_0.width == 0 && calibImgSize_0.height == 0 ) {
      // for the first image of this cam.
      calibImgSize_0.width  = myCalibImg.cols;
      calibImgSize_0.height = myCalibImg.rows;
    } else {
      // check if image size is consistent to the first one
      if ( calibImgSize_0.width  != myCalibImg.cols || 
           calibImgSize_0.height != myCalibImg.rows    ) {
        //if ( debugLevel >= 1 ) {
        //  cerr << "Error: calib. image sizes are not consistent.\n";
        //  return -1;
        //}
        SPRINTF(errorMessage, 
          "Error: CamCalib: Cam %d: Image sizes are not consistent (Previous:%dx%d. This:%dx%d).",
          camId, calibImgSize_0.height, calibImgSize_0.width, myCalibImg.rows, myCalibImg.cols); 
        return -1; 
      }
    }
  } 
  if ( camId == 1) {
    // if it is the first image of this cam, remember its image size.
    // else, check the image sizes are consistent 
    if ( calibImgSize_1.width == 0 && calibImgSize_1.height == 0 ) {
      // for the first image of this cam.
      calibImgSize_1.width  = myCalibImg.cols;
      calibImgSize_1.height = myCalibImg.rows;
    } else {
      // check if image size is consistent to the first one
      if ( calibImgSize_1.width  != myCalibImg.cols || 
           calibImgSize_1.height != myCalibImg.rows    ) {
        //if ( debugLevel >= 1 ) {
        //  cerr << "Error: calib. image sizes are not consistent.\n";
        //  return -1;
        //}
        SPRINTF(errorMessage, 
          "Error: CamCalib: Cam %d: Image sizes are not consistent (Previous:%dx%d. This:%dx%d).",
          camId, calibImgSize_1.height, calibImgSize_1.width, myCalibImg.rows, myCalibImg.cols); 
        return -1; 
      }
    }
  } 

  
  //MessageBoxA(NULL, "check9", "checkbox9", NULL);
  /*cv::Mat temp;
  cv::GaussianBlur(myCalibImg, temp, cv::Size(0, 0), 215);
  cv::addWeighted(myCalibImg, 1.8, temp, -0.8, 0, myCalibImg);*/

  std::cout << "waiting for find chessboard..." << std::endl;
  SPRINTF(errorMessage, "waiting for find chessboard...");
  //MessageBox(NULL, L"waiting for find chessboard...", L"wait...", NULL);
  // find chessboard corners
  bool found; 
  cv::Mat corners( patternSize.width, patternSize.height, CV_32FC2);
  t0 = getWallTime();
  //found = cv::findChessboardCorners( myCalibImg, patternSize, corners, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE  );
  //found = cv::findChessboardCorners( myCalibImg, patternSize, corners, CV_CALIB_CB_ADAPTIVE_THRESH + CV_CALIB_CB_FILTER_QUADS + CV_CALIB_CB_NORMALIZE_IMAGE + CV_CALIB_CB_FAST_CHECK);
  //MessageBoxA(NULL, "check10", "checkbox10", NULL);
  //found = cv::findChessboardCorners( Temp, patternSize, corners, CV_CALIB_CB_ADAPTIVE_THRESH + CV_CALIB_CB_FILTER_QUADS + CV_CALIB_CB_NORMALIZE_IMAGE + CV_CALIB_CB_FAST_CHECK);
  //MessageBoxA(NULL, "check11", "checkbox11", NULL);
  //calibImgSize_0 = Temp2;
  found = findChessboardCorners_coarsenAndSubpix(myCalibImg, patternSize, corners, 0);
  std::cout << "found chessboard!" << std::endl;
  SPRINTF(errorMessage, "found chessboard!");
  char temps[256];
  SPRINTF(temps, "found chessboard! %d", nImage);
  

  if ( found == true ) {
	MessageBoxA(NULL, temps, "wait...", NULL);
    //if ( debugLevel >= 2 ) {
    //  cout << "Found corners of image " << file << endl;
    //}
	//for (int i = 0; i < corners.rows; i++)
	//{
	//	for (int j = 0; j < corners.cols; j++)
	//	{
	//		corners.at<double>(i, j) /=  scaleRatio;
	//	}
	//}

   // cv::cornerSubPix( Temp, corners, cv::Size(5,5), cv::Size(-1,-1), 
    //      cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 100, 0.01));
    //if ( debugLevel >= 3 ) {
    //  cout << "Finding corners time:" 
    //       << (clock()-t0)/CLOCKS_PER_SEC << " sec.\n";
    //}
    // output drawn corners
    if ( this->drawChessCorners == true ) {
//      t0 = clock();
      cv::drawChessboardCorners( myCalibImg, patternSize, corners, found );
      //cv::imwrite( file + this->drawnCornersSuffix, myCalibImg );
	  
	  char tempName[260];
	  SPRINTF(tempName, "chessboardImage\\drawChessboard_%03d_%03d.JPG", nImage, nFound);
	  bool wrote = cv::imwrite(tempName, myCalibImg);
	  if (!wrote)
	  {
		  MessageBox(NULL, L"can not write chessboard image...", L"ERROR", NULL);
	  }
      //if ( debugLevel >= 3 ) {
      //    cout << "Drawing & writing corners time: " 
      //         << (clock()-t0)/CLOCKS_PER_SEC << " sec.\n";
      //}
    }
	nFound++;
	nImage++;
  }
  else
  {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Can not find corners of image " << file << endl;
	nImage++;
    SPRINTF(errorMessage, "Error: CamCalib: Cannot find corners in this image.");
    return -1; 
  }  
  if ( camId == 0 ) {
    cornersVec_0.push_back(corners);
  } else {
    cornersVec_1.push_back(corners);
  }
  
  return 0;
}

int CamCalib::setPatternSize(int cols, int rows) 
{
  // check data
  if (cols <=0 || rows <= 0) {
    SPRINTF(errorMessage, "Error: CamCalib: pattern sizes have to be positive.");
    return -1; 
  }
  patternSize.width  = cols; 
  patternSize.height = rows; 
  return 0; 
}

int CamCalib::setCellSize(double w, double h) 
{
  // check cell size.
  if ( w <= 0.0 || h <= 0.0 ) 
  {
    //if ( debugLevel >= 1 ) {
    //  cerr << "Invalid cell size: " 
    //       << w << " " << h << "\n";
    //}
    SPRINTF(errorMessage, "Error: CamCalib: Invalid cell size %f %f.", w, h); 
    return -1; 
  }
  cellWidth = w; 
  cellHeight = h;
  return 0; 
}

// independent function 
//int chessboardObjectPoints(const  cv::Size patternSize,  // # of inner corners per a chessboard row and column
//                            int    nViews,                // # of views (images) of chessboard
//                            double squareSizeX,           // width of square (x distance between corners)
//                            double squareSizeY,           // height of square (y distance between corners)
//                            vector<vector<cv::Point3f>> & object_points) // object points of chessboard corners
//{
//    int iView, iY, iX, nY, nX, ic;
//    nY = patternSize.height; 
//    nX = patternSize.width;
//    // resize the object_points object
//    object_points.clear();
//    object_points.resize(nViews);
//    for (iView = 0; iView < nViews; iView++) {
//        object_points[iView].resize(nY*nX);
//    }
//
//	// debug check
//	if (nViews <= 0) return -1;
//	if (nX <= 0) return -2;
//	if (nY <= 0) return -3;
//	if (object_points.size() != nViews) return -4;
//	for (auto each : object_points) if (each.size() != (nY * nX)) return -5;
//
//    // fill the object points
//    for (iView = 0; iView < nViews; iView++) {
//        ic = 0;
//        for (iY = nY - 1; iY >= 0; iY--) {
//            for (iX = 0; iX < nX; iX++) {
//                object_points[iView][ic].x = (float)(iX * squareSizeX);
//                object_points[iView][ic].y = (float)(iY * squareSizeY);
//                object_points[iView][ic].z = (float) 0.0;
//                ic++;
//            }
//        }
//    }
//	if (object_points.size() == 0) return -6;
//    return 0; 
//}

std::string itos(int num)
{
	std::stringstream ss;
	ss << num;
	return ss.str();
}

double CamCalib::calibrate() 
{
  // Check if there are sufficient images
  if ( calibImgFiles_0.size() < 1 ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Error: Insufficient calibration image.\n";
    SPRINTF(errorMessage, "Error: CamCalib: no calibration image."); 
    return -1.0;
  }

  // Check cell size
  if ( cellWidth <= 0.0 || cellHeight <= 0.0 ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Error: chessboard cell size is not assigned yet.\n";
    SPRINTF(errorMessage, "Error: CamCalib: cell sizes are not set yet."); 
    return -1.0;
  }

  // Check if left and right cams have same # of cal. images.
  if ( nCam == 2 ) {
    // check 1: # of images 
    if ( calibImgFiles_0.size() != calibImgFiles_1.size() ) {
      //if ( debugLevel >= 1 ) 
      //  cerr << "Error: Cam 0 and Cam 1 have different numbers of images.\n";
      SPRINTF(errorMessage, 
        "Error: CamCalib: Left/right cams image numbers are not matched (L:%d. R:%d)",
        calibImgFiles_0.size(), calibImgFiles_1.size() ); 
      return -1.0;
    }
  }
  MessageBoxA(NULL, "step 1", "Step check", NULL);
  // generate object points of chessboard
  vector<vector<cv::Point3f>> object_points;

  objectPoints.clear();
  objectPoints.resize(cornersVec_0.size());
  for (int k = 0; k < (int)cornersVec_0.size(); k++)
  {
	  objectPoints[k].clear();
	  for (int j = 0; j < patternSize.height; j++)
		  for (int i = 0; i < patternSize.width; i++)
			  objectPoints[k].push_back(cv::Point3f(cellWidth * (float)j, cellHeight * (float)i, 0.0));
  }

  //int val = chessboardObjectPoints( patternSize, (int) cornersVec_0.size(), 
  //                        cellWidth, cellHeight, 
  //                        object_points );

    int iView, iY, iX, nY, nX, ic;
    nY = patternSize.height; 
    nX = patternSize.width;
    // resize the object_points object
    object_points.clear();
    object_points.resize(cornersVec_0.size());
    for (iView = 0; iView < cornersVec_0.size(); iView++) {
        object_points[iView].resize(nY*nX);
    }

	// debug check
	if (cornersVec_0.size() <= 0) return -1; // check if corners size is not exist
	if (nX <= 0) return -2; // check if nX is not exist
	if (nY <= 0) return -3; // check if nY is not exist
	if (object_points.size() != cornersVec_0.size()) return -4; // check if object points can not generate successful
	for (auto each : object_points) if (each.size() != (nY * nX)) return -5; // check each object point's space is exist or not

    // fill the object points
    for (iView = 0; iView < cornersVec_0.size(); iView++) {
        ic = 0;
        for (iY = nY - 1; iY >= 0; iY--) {
            for (iX = 0; iX < nX; iX++) {
                object_points[iView][ic].x = (float)(iX * cellWidth);
                object_points[iView][ic].y = (float)(iY * cellHeight);
                object_points[iView][ic].z = (float) 0.0;
                ic++;
            }
        }
    }
	if (object_points.size() == 0) return -6; // check if object points is exist or not

  cv::FileStorage fsc("corner.yml", cv::FileStorage::WRITE);
  fsc << "corner" << cornersVec_0;
  std::string sc = "sc: ";
  
  for (auto each : cornersVec_0) sc += itos(each.size()) + " ";
  MessageBoxA(NULL, sc.c_str(), "var", NULL);
  cv::FileStorage fso("objectpoint.yml", cv::FileStorage::WRITE);
  fso << "objPoints" << objectPoints;
  std::string so = "so: ";
  for (auto each : objectPoints) so += itos(each.size()) + " ";
  MessageBoxA(NULL, so.c_str(), "var", NULL);

  char s1[256];
  SPRINTF(s1, "nImage = %d, cornersVec.size = %d", nImage, cornersVec_0.size());
  MessageBoxA(NULL, s1, "var", NULL);
  MessageBoxA(NULL, "step 2", "Step check", NULL);
  int fs = calibImgFiles_0.size();
  int os = objectPoints.size();
  int cs = cornersVec_0.size();
  char ss[256];
  SPRINTF(ss, "fs = %d, os = %d, cs = %d", fs, os, cs);
  MessageBoxA(NULL, ss, "var", NULL);
  for (auto each : objectPoints) if (each.size() == 0) MessageBoxA(NULL,"size is zero", "each", NULL); 
  // calibrate camera 0 (left or single)
//  vector<vector<double>> rvecs_0, tvecs_0;
  this->camCalibErr_0 = cv::calibrateCamera( object_points, 
                        cornersVec_0, 
                        calibImgSize_0, 
                        camMat_0, 
                        disCoe_0, 
                        rvecs_0, tvecs_0, 
                        calibFlag, 
                        cv::TermCriteria( cv::TermCriteria::COUNT+cv::TermCriteria::EPS, 30, DBL_EPSILON) ); 
  MessageBoxA(NULL, "step 3", "Step check", NULL);
  //if ( debugLevel >= 3 ) {
  //  cout << "Cam 0 calibration error: " << this->camCalibErr_0 << endl;
  //}

  if ( nCam <= 1 ) {
	  MessageBoxA(NULL, "step 4", "Step check", NULL);
	  return this->camCalibErr_0;
  }

  // calibrate camera 1 (right)
//  vector<vector<double>> rvecs_1, tvecs_1;
  this->camCalibErr_1 = cv::calibrateCamera( object_points, 
                        cornersVec_1, 
                        calibImgSize_1, 
                        camMat_1, 
                        disCoe_1, 
                        rvecs_1, tvecs_1, 
                        calibFlag, 
                        cv::TermCriteria( cv::TermCriteria::COUNT+cv::TermCriteria::EPS, 30, DBL_EPSILON) ); 
//  cout << "Cam 1 calibration error: " << this->camCalibErr_1 << endl;

  // stereo calibration 
  this->stereoCalibErr = cv::stereoCalibrate(  object_points, 
                                cornersVec_0, 
                                cornersVec_1, 
                                camMat_0, 
                                disCoe_0, 
                                camMat_1, 
                                disCoe_1,
                                calibImgSize_0, 
                                R, T, E, F, 
                                cv::TermCriteria(cv::TermCriteria::COUNT+cv::TermCriteria::EPS, 30, 1e-6), 
                                calibFlag   ); 
  // 
  calibrated = true; 
  return stereoCalibErr; 
}

cv::Mat CamCalib::getR    () 
{
  // Check if nCam >= 2
  if ( nCam < 2 ) {
    //if ( debugLevel >= 2 ) {
    //  cerr << "Warning: R is not defined for single cam.\n";
    //}
    SPRINTF(errorMessage, "Error: CamCalib: getR() is only for nCam == 2.\n");
  }
  if ( calibrated == false ) {   
    this->calibrate();
  }
  return R; 
}

cv::Mat CamCalib::getT    () 
{
  // Check if nCam >= 2
  if ( nCam < 2 ) {
    //if ( debugLevel >= 2 ) {
    //  cerr << "Warning: T is not defined for single cam.\n";
    //}
    SPRINTF(errorMessage, "Error: CamCalib: getT() is only for nCam == 2.\n");
  }
  if ( calibrated == false ) {   
    this->calibrate();
  }
  return T; 
}

cv::Mat CamCalib::getF    () 
{
  // Check if nCam >= 2
  if ( nCam < 2 ) {
    //if ( debugLevel >= 2 ) {
    //  cerr << "Warning: F is not defined for single cam.\n";
    //}
    SPRINTF(errorMessage, "Error: CamCalib: getF() is only for nCam == 2.\n");
  }
  if ( calibrated == false ) {   
    this->calibrate();
  }
  return F; 
}

cv::Mat CamCalib::getE    () 
{
  // Check if nCam >= 2
  if ( nCam < 2 ) {
    //if ( debugLevel >= 2 ) {
    //  cerr << "Warning: E is not defined for single cam.\n";
    //}
    SPRINTF(errorMessage, "Error: CamCalib: getE() is only for nCam == 2.\n");
  }
  if ( calibrated == false ) {   
    this->calibrate();
  }
  return E; 
}

cv::Mat CamCalib::getRodriguesOfR    () 
{
  // Check if nCam >= 2
  if ( nCam < 2 ) {
    //if ( debugLevel >= 2 ) {
    //  cerr << "Warning: E is not defined for single cam.\n";
    //}
    SPRINTF(errorMessage, "Error: CamCalib: getRodriguesOfR() is only for nCam == 2.\n");
  }
  if ( calibrated == false ) {   
    this->calibrate();
  }
  cv::Mat vecRodgiguesOfR;
  cv::Rodrigues( this->R, vecRodgiguesOfR );
  return vecRodgiguesOfR; 
}


cv::Mat CamCalib::getCamMatrix (int camId) 
{
  // check if camId is valid
  if ( camId < 0 || camId >= this->nCam ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Warning: Invalid camId . Taking it as 0 (left cam).\n";
    // camId = 0;
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", camId); 
  }
  // run calibration if it is not done yet.
  if ( calibrated == false ) {   
    this->calibrate();
  }
  if ( camId == 0 ) 
    return camMat_0; 
  else 
    return camMat_1;
}

cv::Mat CamCalib::getDisCoeffs (int camId) 
{
  // check if camId is valid
  if ( camId < 0 || camId >= this->nCam ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Warning: Invalid camId . Taking it as 0 (left cam).\n";
    // camId = 0;
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", camId); 
  }
  // run calibration if it is not done yet.
  if ( calibrated == false ) {   
    this->calibrate();
  }
  if ( camId == 0 ) 
    return disCoe_0; 
  else 
    return disCoe_1;
}

int     CamCalib::getChessboardRVecs(int imageId, cv::Mat & rvec, int camId) {
  // check data 
  if (camId >= this->nCam) {
    // camId is too large, not a valid camera Id. 
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", camId); 
    return -1;
  }
  if (camId == 0 && imageId >= (int) calibImgFiles_0.size() ) {
    // imageId is too large, not a valid image Id.
    SPRINTF(errorMessage, "Error: CamCalib: Invalid image %d", imageId); 
    return -1;
  }
  if (camId == 1 && imageId >= calibImgFiles_1.size() ) {
    // imageId is too large, not a valid image Id.
    SPRINTF(errorMessage, "Error: CamCalib: Invalid image %d", imageId); 
    return -1;
  }
  // Get the R (rotation) vector of a chessboard (imageId) wrt a camera (camId)
  if (camId == 0) {        // wrt left camera
    rvec = this->rvecs_0[imageId]; 
    return 0;
  } else if (camId == 1) { // wrt right camera
    rvec = this->rvecs_1[imageId]; 
    return 0;
  } else { 
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", camId); 
    return -1; 
  }
  return 0; 
}

int     CamCalib::getChessboardTVecs(int imageId, cv::Mat & tvec, int camId) {
  // check data 
  if (camId >= this->nCam) {
    // camId is too large, not a valid camera Id. 
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", camId); 
    return -1;
  }
  if (camId == 0 && imageId >= calibImgFiles_0.size() ) {
    // imageId is too large, not a valid image Id.
    SPRINTF(errorMessage, "Error: CamCalib: Invalid image %d", imageId); 
    return -1;
  }
  if (camId == 1 && imageId >= calibImgFiles_1.size() ) {
    // imageId is too large, not a valid image Id.
    SPRINTF(errorMessage, "Error: CamCalib: Invalid image %d", imageId); 
    return -1;
  }
  // Get the T (translation) vector of a chessboard (imageId) wrt a camera (camId)
  if (camId == 0) {        // wrt left camera
    tvec = this->tvecs_0[imageId]; 
    return 0;
  } else if (camId == 1) { // wrt right camera
    tvec = this->tvecs_1[imageId]; 
    return 0;
  } else { 
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", camId); 
    return -1; 
  }
  return 0; 
}

int     CamCalib::getCornersImagePoints(int imageId, vector<cv::Point2f> & imagePoints, int camId)
{
  // check data 
  if (camId >= this->nCam) {
    // camId is too large, not a valid camera Id. 
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", camId); 
    return -1;
  }
  if (camId == 0 && imageId >= calibImgFiles_0.size() ) {
    // imageId is too large, not a valid image Id.
    SPRINTF(errorMessage, "Error: CamCalib: Invalid image %d", imageId); 
    return -1;
  }
  if (camId == 1 && imageId >= calibImgFiles_1.size() ) {
    // imageId is too large, not a valid image Id.
    SPRINTF(errorMessage, "Error: CamCalib: Invalid image %d", imageId); 
    return -1;
  }
  // Get the image points of corners of a chessboard (imageId) wrt a camera (camId)
  if (camId == 0) {        // wrt left camera
    imagePoints = this->cornersVec_0[imageId]; 
    return 0;
  } else if (camId == 1) { // wrt right camera
    imagePoints = this->cornersVec_1[imageId]; 
    return 0;
  } else { 
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", camId); 
    return -1; 
  }
  return 0; 
}

int     CamCalib::getPhotoSize(cv::Size & imgsize, int camId)
{
  // check data 
  if (camId >= this->nCam) {
    // camId is too large, not a valid camera Id. 
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", camId); 
    return -1;
  }
  if (camId == 0) {
	  imgsize = this->calibImgSize_0;
  } else if (camId == 1) {
	  imgsize = this->calibImgSize_1;
  } else {
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", camId); 
	  return -1;
  }
  return 0; 
}


double  CamCalib::getCamCalibErr(int camId) 
{
  // check if camId is valid
  if ( camId < 0 || camId >= this->nCam ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Warning: Invalid camId . Taking it as 0 (left cam).\n";
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", camId); 
    return -1;
  }
  // run calibration if it is not done yet.
  if ( calibrated == false ) {   
    this->calibrate();
  }
  if ( camId == 0 ) 
    return camCalibErr_0; 
  else 
    return camCalibErr_1;
}

double  CamCalib::getStereoCalibErr() 
{
  // Check if nCam >= 2
  if ( nCam < 2 ) {
    //if ( debugLevel >= 2 ) {
    //  cerr << "Warning: Stereo err is not defined for single cam.\n";
    //}
    SPRINTF(errorMessage, "Error: CamCalib: getStereoCalibErr is only for nCam ==2"); 
  }
  if ( calibrated == false ) {   
    this->calibrate();
  }
  return stereoCalibErr; 
}

int     CamCalib::nImagePairs () const 
{
  return (int) calibImgFiles_0.size();
}

double  CamCalib::getFx(int camId) 
{
  // check if camId is valid
  if ( camId < 0 || camId >= this->nCam ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Warning: Invalid camId . Taking it as 0 (left cam).\n";
    // camId = 0;
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", camId); 
    return -1; 
  }
  // run calibration if it is not done yet.
  if ( calibrated == false ) {   
    this->calibrate();
  }
  if ( camId == 0 ) 
    return camMat_0.at<double>(0,0) ; 
  else 
    return camMat_1.at<double>(0,0) ;
}

double  CamCalib::getFy(int camId) 
{
  // check if camId is valid
  if ( camId < 0 || camId >= this->nCam ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Warning: Invalid camId . Taking it as 0 (left cam).\n";
    // camId = 0;
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", camId); 
    return -1; 
  }
  // run calibration if it is not done yet.
  if ( calibrated == false ) {   
    this->calibrate();
  }
  if ( camId == 0 ) 
    return camMat_0.at<double>(1,1) ; 
  else 
    return camMat_1.at<double>(1,1) ;
}

double  CamCalib::getCx(int camId) 
{
  // check if camId is valid
  if ( camId < 0 || camId >= this->nCam ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Warning: Invalid camId . Taking it as 0 (left cam).\n";
    // camId = 0;
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", camId); 
  }
  // run calibration if it is not done yet.
  if ( calibrated == false ) {   
    this->calibrate();
  }
  if ( camId == 0 ) 
    return camMat_0.at<double>(0,2) ; 
  else 
    return camMat_1.at<double>(0,2) ;
}

double  CamCalib::getCy(int camId) 
{
  // check if camId is valid
  if ( camId < 0 || camId >= this->nCam ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Warning: Invalid camId . Taking it as 0 (left cam).\n";
    //camId = 0;
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", camId); 
    return -1; 
  }
  // run calibration if it is not done yet.
  if ( calibrated == false ) {   
    this->calibrate();
  }
  if ( camId == 0 ) 
    return camMat_0.at<double>(1,2) ; 
  else 
    return camMat_1.at<double>(1,2) ;
}

double  CamCalib::getK1(int camId)
{
  // check if camId is valid
  if ( camId < 0 || camId >= this->nCam ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Warning: Invalid camId . Taking it as 0 (left cam).\n";
    //camId = 0;
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", camId); 
    return -1; 
  }
  // run calibration if it is not done yet.
  if ( calibrated == false ) {   
    this->calibrate();
  }
  if ( camId == 0 ) 
    return disCoe_0.at<double>(0);
  else 
    return disCoe_1.at<double>(0);
}

double  CamCalib::getK2(int camId)
{
  // check if camId is valid
  if ( camId < 0 || camId >= this->nCam ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Warning: Invalid camId . Taking it as 0 (left cam).\n";
    //camId = 0;
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", camId); 
    return -1; 
  }
  // run calibration if it is not done yet.
  if ( calibrated == false ) {   
    this->calibrate();
  }
  if ( camId == 0 ) 
    return disCoe_0.at<double>(1);
  else 
    return disCoe_1.at<double>(1);
}

double  CamCalib::getP1(int camId)
{
  // check if camId is valid
  if ( camId < 0 || camId >= this->nCam ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Warning: Invalid camId . Taking it as 0 (left cam).\n";
    //camId = 0;
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", camId); 
    return -1; 
  }
  // run calibration if it is not done yet.
  if ( calibrated == false ) {   
    this->calibrate();
  }
  if ( camId == 0 ) 
    return disCoe_0.at<double>(2);
  else 
    return disCoe_1.at<double>(2);
}

double  CamCalib::getP2(int camId)
{
  // check if camId is valid
  if ( camId < 0 || camId >= this->nCam ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Warning: Invalid camId . Taking it as 0 (left cam).\n";
    //camId = 0;
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", camId); 
    return -1; 
  }
  // run calibration if it is not done yet.
  if ( calibrated == false ) {   
    this->calibrate();
  }
  if ( camId == 0 ) 
    return disCoe_0.at<double>(3);
  else 
    return disCoe_1.at<double>(3);
}

double  CamCalib::getK3(int camId)
{
  // check if camId is valid
  if ( camId < 0 || camId >= this->nCam ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Warning: Invalid camId . Taking it as 0 (left cam).\n";
    //camId = 0;
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", camId); 
    return -1; 
  }
  // run calibration if it is not done yet.
  if ( calibrated == false ) {   
    this->calibrate();
  }
  if ( camId == 0 ) 
    return disCoe_0.at<double>(4);
  else 
    return disCoe_1.at<double>(4);
}

double  CamCalib::getK4(int camId)
{
  // check if camId is valid
  if ( camId < 0 || camId >= this->nCam ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Warning: Invalid camId . Taking it as 0 (left cam).\n";
    //camId = 0;
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", camId); 
    return -1; 
  }
  // run calibration if it is not done yet.
  if ( calibrated == false ) {   
    this->calibrate();
  }
  if ( camId == 0 ) 
    if ( disCoe_0.cols <= 5 ) 
      return 0.0;
    else 
      return disCoe_1.at<double>(5);
  else 
    if ( disCoe_1.cols <= 5 ) 
      return 0.0;
    else 
      return disCoe_1.at<double>(5);
}

double  CamCalib::getK5(int camId)
{
  // check if camId is valid
  if ( camId < 0 || camId >= this->nCam ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Warning: Invalid camId . Taking it as 0 (left cam).\n";
    //camId = 0;
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", camId); 
    return -1; 
  }
  // run calibration if it is not done yet.
  if ( calibrated == false ) {   
    this->calibrate();
  }
  if ( camId == 0 ) 
    if ( disCoe_0.cols <= 6 ) 
      return 0.0;
    else 
      return disCoe_0.at<double>(6);
  else 
    if ( disCoe_1.cols <= 6 ) 
      return 0.0;
    else 
      return disCoe_1.at<double>(6);
}

double  CamCalib::getK6(int camId)
{
  // check if camId is valid
  if ( camId < 0 || camId >= this->nCam ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Warning: Invalid camId . Taking it as 0 (left cam).\n";
    //camId = 0;
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", camId); 
    return -1; 
  }
  // run calibration if it is not done yet.
  if ( calibrated == false ) {   
    this->calibrate();
  }
  if ( camId == 0 ) 
    if ( disCoe_0.cols <= 7 ) 
      return 0.0;
    else 
      return disCoe_1.at<double>(7);
  else 
    if ( disCoe_1.cols <= 7 ) 
      return 0.0;
    else 
      return disCoe_1.at<double>(7);
}

void    CamCalib::setFlag_UseIntrinsicGuess(bool set)
{
  if (set) 
    calibFlag = calibFlag | CV_CALIB_USE_INTRINSIC_GUESS;
  else
    calibFlag = calibFlag ^ CV_CALIB_USE_INTRINSIC_GUESS;
}

void    CamCalib::setFlag_FixPrincipalPoint(bool set)
{
  if (set) 
    calibFlag = calibFlag | CV_CALIB_FIX_PRINCIPAL_POINT;
  else
    calibFlag = calibFlag ^ CV_CALIB_FIX_PRINCIPAL_POINT;
}

void    CamCalib::setFlag_FixAspectRatio(bool set)
{
  if (set) 
    calibFlag = calibFlag | CV_CALIB_FIX_ASPECT_RATIO;
  else
    calibFlag = calibFlag ^ CV_CALIB_FIX_ASPECT_RATIO;
}

void    CamCalib::setFlag_ZeroTangentDist(bool set)
{
  if (set) 
    calibFlag = calibFlag | CV_CALIB_ZERO_TANGENT_DIST;  
  else
    calibFlag = calibFlag ^ CV_CALIB_ZERO_TANGENT_DIST; 
}

void    CamCalib::setFlag_FixFocalLength(bool set)
{
  if (set) 
    calibFlag = calibFlag | CV_CALIB_FIX_FOCAL_LENGTH;  
  else
    calibFlag = calibFlag ^ CV_CALIB_FIX_FOCAL_LENGTH; 
}

void    CamCalib::setFlag_FixK1(bool set)
{
  if (set) 
    calibFlag = calibFlag | CV_CALIB_FIX_K1;
  else
    calibFlag = calibFlag ^ CV_CALIB_FIX_K1;
}

void    CamCalib::setFlag_FixK2(bool set)
{
  if (set) 
    calibFlag = calibFlag | CV_CALIB_FIX_K2;
  else
    calibFlag = calibFlag ^ CV_CALIB_FIX_K2;
}

void    CamCalib::setFlag_FixK3(bool set)
{
  if (set) 
    calibFlag = calibFlag | CV_CALIB_FIX_K3;
  else
    calibFlag = calibFlag ^ CV_CALIB_FIX_K3;
}

void    CamCalib::setFlag_FixIntrinsic(bool set)
{
  if (set) 
    calibFlag = calibFlag | CV_CALIB_FIX_INTRINSIC;
  else
    calibFlag = calibFlag ^ CV_CALIB_FIX_INTRINSIC;
}

void    CamCalib::setFlag_SameFocalLength(bool set)
{
  if (set) 
    calibFlag = calibFlag | CV_CALIB_SAME_FOCAL_LENGTH;
  else
    calibFlag = calibFlag ^ CV_CALIB_SAME_FOCAL_LENGTH;
}

void    CamCalib::setFlag_ZeroDisparity(bool set)
{
  if (set) 
    calibFlag = calibFlag | CV_CALIB_ZERO_DISPARITY;
  else
    calibFlag = calibFlag ^ CV_CALIB_ZERO_DISPARITY;
}

void    CamCalib::setFlag_FixK4(bool set)
{
  if (set) 
    calibFlag = calibFlag | CV_CALIB_FIX_K4;
  else
    calibFlag = calibFlag ^ CV_CALIB_FIX_K4;
}

void    CamCalib::setFlag_FixK5(bool set)
{
  if (set) 
    calibFlag = calibFlag | CV_CALIB_FIX_K5;
  else
    calibFlag = calibFlag ^ CV_CALIB_FIX_K5;
}

void    CamCalib::setFlag_FixK6(bool set)
{
  if (set) 
    calibFlag = calibFlag | CV_CALIB_FIX_K6;
  else
    calibFlag = calibFlag ^ CV_CALIB_FIX_K6;
}

void    CamCalib::setFlag_Rational_Model(bool set)
{
  if (set) 
    calibFlag = calibFlag | CV_CALIB_RATIONAL_MODEL ;
  else
    calibFlag = calibFlag ^ CV_CALIB_RATIONAL_MODEL ;
}

void    CamCalib::setDebugLevel(int level)
{
  debugLevel = level; 
}

void    CamCalib::setDrawChessCorners(bool set, string suffix) 
{
  drawChessCorners = set;  
  drawnCornersSuffix = suffix; 
}


int CamCalib::undistortImg(string srcFile, string dstFile, int camId)
        // This function undistorts an image to an undistorted image.
        // I.e., it transforms an image to compensate for lens distortion.
        // See OpenCV C++ void undistort function for more details. 
{
  // open the image file
//  double t0 = clock();
//  cv::Mat srcImg = cv::imread(srcFile, CV_LOAD_IMAGE_GRAYSCALE);
  cv::Mat srcImg = cv::imread(srcFile);
  //if ( debugLevel >= 4 ) {
  //  cout << "File access time: " 
  //       << (clock()-t0)/CLOCKS_PER_SEC << " sec.\n";
  //}
  // Check image size. If zero, it fails to load image.
  if ( srcImg.cols == 0 || srcImg.rows == 0 ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Warning: Could not open " << srcFile << endl;
    //return;
    SPRINTF(errorMessage, "Error: CamCalib: Can not open %s.", srcFile.c_str() ); 
    return -1; 
  }
  // check if calibrated
  if ( !this->calibrated ) {
    this->calibrate();
  }
  // run undistort
//  t0 = clock();
  cv::Mat dstImg; 
  if ( camId == 0 ) {
    undistort(srcImg, dstImg, this->camMat_0, this->disCoe_0 );
  } 
  if ( camId == 1 ) {
    undistort(srcImg, dstImg, this->camMat_1, this->disCoe_1 );
  } 
  //if ( debugLevel >= 4 ) {
  //  cout << "Undistort time: " 
  //       << (clock()-t0)/CLOCKS_PER_SEC << " sec.\n";
  //}
  // imwrite (output the image to file)
//  t0 = clock();
  vector<int> compression_params;
  compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
  compression_params.push_back(2);
  bool imwriteret = 
    imwrite( dstFile, dstImg, compression_params );
  if ( imwriteret == 0 ) {
      //if ( debugLevel >= 1 ) {
      //    cerr << "imwrite failed.\n";
      //}
    SPRINTF(errorMessage, "Error: CamCalib: imwrite failed.");
    return -1; 
  }

  //if ( debugLevel >= 4 ) {
  //  cout << "imwrite time: " 
  //       << (clock()-t0)/CLOCKS_PER_SEC << " sec.\n";
  //}
  return 0;
}

const char* CamCalib::getErrorMessage() const {
  return this->errorMessage; 
}


cv::Size CamCalib::getResize(int x){
	return reSize;
}

// This function is supposed to be able to replace cv::findChessboardCorners(). 
// It warps cv::findChessboardCorners() and cornerSubPix().
// It saves CPU time for large image by resizing image before calling cv::findChessboardCorners(). 
// 
bool CamCalib::findChessboardCorners_coarsenAndSubpix(cv::InputArray _image, cv::Size patternSize, 
     cv::OutputArray _corners, int flags)
{
  bool found; 
  cv::Mat image = _image.getMat(); 
  cv::Mat corners = _corners.getMat(); 
  int subpixSearchSize; 

  // Check input parameters
  if (image.cols <= 0 || image.rows <= 0) {
    return false; 
  }

  // try equivalent size 400x400, * 2^1, * 2^2 , ... actual size
  for (float imgArea = 400 * 400; true; imgArea *= 2.0f) {
    int w0 = image.cols; 
    int h0 = image.rows; 
    // reduce to new size (w1, h1) that w1 * h1 <= 500 * 500 
    float ratio = sqrt(imgArea / (w0 * h0));  // estimated resize ratio
    subpixSearchSize = (int)(ratio + 0.5 + 3); 
    int w1 = (int) (w0 * ratio + 0.5); 
    int h1 = (int) (h0 * ratio + 0.5); 
    cv::Mat coarsen_image;
    cv::resize(image, coarsen_image, cv::Size(w1, h1)); 
    found = cv::findChessboardCorners( coarsen_image, patternSize, _corners, flags);
    // if found, refine the coordinates of corners
    if (found == true) {
      for (int iw = 0; iw < corners.cols; iw++) {
        for (int ih = 0; ih < corners.rows; ih++) {
          corners.at<cv::Point2f>(ih, iw).x *= (1.0f * w0 / w1); 
          corners.at<cv::Point2f>(ih, iw).y *= (1.0f * h0 / h1); 
        }
      }
      break; // found corners then break
    }  // end if found all corners
    // if image has been larger than original, break (and return fail signal)
    if (ratio > 1.0f) 
      break; 
  } // end of for imgArea increasing from 400*400 --> larger than original size

  if (found == true) {
    cv::cornerSubPix(image, _corners, cv::Size(subpixSearchSize,subpixSearchSize), 
          cv::Size(-1,-1), cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 100, 0.01));
  }
  return found; 
}