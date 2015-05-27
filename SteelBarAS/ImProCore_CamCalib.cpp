#include "stdafx.h"
#include <stdlib.h>
#include <cmath>

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


// This function is supposed to be able to replace cv::findChessboardCorners(). 
// It warps cv::findChessboardCorners() and cornerSubPix().
// It saves CPU time for large image by resizing image before calling cv::findChessboardCorners(). 
// 
bool findChessboardCorners_coarsenAndSubpix(cv::InputArray image, cv::Size patternSize, 
     cv::OutputArray corners, int flags=cv::CALIB_CB_ADAPTIVE_THRESH+cv::CALIB_CB_NORMALIZE_IMAGE);

CamCalib::CamCalib(int nCamera)
{
  // check nCam
  if ( nCamera < 1 || nCamera > 2 ) {
    //if ( debugLevel >= 1 ) {
    //  cerr << "Error-CamCalib: Invalid # of cameras: " << nCam << endl;
    //}
    SPRINTF(errorMessage, "Error: CamCalib: Invalid # of cameras: %d. Set to 1.", nCam); 
    nCamera = 1; 
  }
  this->nCam            = nCamera; 
  calibrated            = false; 
  patternSize.width     = 0;
  patternSize.height    = 0;
  cellWidth             = 0.0;
  cellHeight            = 0.0; 
  calibImgSize.resize(nCamera);
  // initialize calibImgSize[iCam]
  for (int iCam = 0; iCam < nCam; iCam++) {
    calibImgSize[iCam].width  = 0; 
    calibImgSize[iCam].height = 0; 
  }
  // resize vectors
  calibImgFiles.resize(this->nCam); 
  calibImgIndex.resize(this->nCam); 
  cornersVec.resize(this->nCam); 
  camMat.resize(this->nCam); 
  disCoe.resize(this->nCam); 
  rvecs.resize(this->nCam);
  tvecs.resize(this->nCam); 
  camCalibErr.resize(this->nCam); 
  calibFlag             = 0;
  debugLevel            = 1;
  drawChessCorners      = false; 
//  drawChessCorners      = true; 
//  drawnCornersSuffix    = "";
  drawnCornersSuffix    = ".corners.JPG";
}

CamCalib::~CamCalib(void)
{
}

int CamCalib::addCalibImg(string file, int iCam)
{
  // check if pattern size is assigned.
  if ( patternSize.width < 1 || patternSize.height < 1 ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Warning: Assign CamCalib pattern size before adding file.\n";
    SPRINTF(errorMessage, "Warning: CamCalib: Please assign pattern size before adding file."); 
    return -1; 
  }
  // check if camId is valid
  if ( iCam < 0 || iCam >= this->nCam ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Warning: Invalid camId . Taking it as 0 (left cam).\n";
    // camId = 0;
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camera ID: %d.", iCam); 
    return -1; 
  }

  // add the image file name into a file-name vector
  calibImgFiles[iCam].push_back(file); 

  return 0; 
}

// If 
bool CamCalib::findAllCalibImgsCorners() {
  bool addedAnyPair = false; 
  // Check data
  // Check all cameras have the same number of calib. images
  for (int iCam = 1; iCam < this->nCam; iCam++) {
    if (calibImgFiles[iCam].size() != calibImgFiles[0].size()) {
      SPRINTF(errorMessage, "Error: CamCalib: cameras have different number of images.");
      return false;
    }
  }
  // Clear/initialize cornersVec[][][]
  cornersVec.clear(); 
  cornersVec.resize(this->nCam);
  for (int iCam = 0; iCam < this->nCam; iCam++) {
    cornersVec[iCam].resize( calibImgFiles[iCam].size() ); 
  }

  // Initialize/resize calibImgIndex[nCam][nPair] to the same size of calibImgFiles[nCam][nPair]
  for (int iCam = 0; iCam < this->nCam; iCam++) 
    this->calibImgIndex[iCam].resize(calibImgFiles[iCam].size()); 
  // For each pair and cam, read image, get corners, and store corners
  // Note: In a pair, if calib image of any of the cameras fails, all images of this pair should drop. 
  // for each pair
  int iPair, iCam, nCornersVecAdded = 0; 
  for (iPair = 0; iPair < calibImgFiles[0].size(); iPair++) {
    vector<cv::Mat> corners_of_a_pair;  // (only this pair but for all cameras)
    corners_of_a_pair.resize(this->nCam); 
    // for each camera
    for (iCam = 0; iCam < this->nCam; iCam++) {
      // allocate corners of this pair 
      corners_of_a_pair[iCam] = cv::Mat(patternSize.width, patternSize.height, CV_32FC2);
      // open the image file
      string file; 
      file = calibImgFiles[iCam][iPair]; 
      double t0 = getWallTime();
      cv::Mat myCalibImg = cv::imread(file, CV_LOAD_IMAGE_GRAYSCALE );
      // Check image size. If zero, it fails to load image of this pair. Skip this pair.
      if ( myCalibImg.cols == 0 || myCalibImg.rows == 0 ) {
        //if ( debugLevel >= 1 ) 
        //  cerr << "Warning: Could not open " << file << endl;
        //SPRINTF(errorMessage, "Warning: CamCalib: Could not open %s.", file.c_str()); 
        //return -1;
        for (int iCam2 = 0; iCam2 < this->nCam; iCam2++)  // one cam fails and all cams fail.
          this->calibImgIndex[iCam2][iPair] = -1;  // negative index denotes corners are not added into cornersVec
        break;
      }
      // Check calib. image size.
       // if it is the first image of this cam, remember its image size.
      // else, check the image sizes are consistent 
      if ( calibImgSize[iCam].width == 0 && calibImgSize[iCam].height == 0 ) {
        // for the first image of this cam.
        calibImgSize[iCam].width  = myCalibImg.cols;
        calibImgSize[iCam].height = myCalibImg.rows;
      } else {
        // check if image size is consistent to the first one. If not, skip this pair
        if ( calibImgSize[iCam].width  != myCalibImg.cols || 
             calibImgSize[iCam].height != myCalibImg.rows    ) {
            //SPRINTF(errorMessage, 
            //  "Error: CamCalib: Cam %d: Image sizes are not consistent (Previous:%dx%d. This:%dx%d).",
            //  iCam, calibImgSize_0.height, calibImgSize_0.width, myCalibImg.rows, myCalibImg.cols); 
            for (int iCam2 = 0; iCam2 < this->nCam; iCam2++)  // one cam fails and all cams fail.
              this->calibImgIndex[iCam2][iPair] = -1; 
            break;
        }
      }
      
      // find chessboard corners
      bool found; 
      int  subpixSearchSize; 
      //cv::Mat corners( patternSize.width, patternSize.height, CV_32FC2);
      t0 = getWallTime();
      int runCoarsenOrNot = 1; 
      // try to coarsen calibration image to save CPU cost on findChessboardCorners
      if (runCoarsenOrNot == 0) { 
        found = cv::findChessboardCorners( myCalibImg, patternSize, corners_of_a_pair[iCam], 
          CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE  );
        subpixSearchSize = 3; 
      } else {
        found = findChessboardCorners_coarsenAndSubpix(myCalibImg, patternSize, corners_of_a_pair[iCam], 
          CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE  );
        // if found, no need to run cornerSubpix()
        if (found == true) 
          subpixSearchSize = 0; 
      } // end of if runCoarsenOrNot

      if ( found == true ) {
        //if ( debugLevel >= 2 ) {
        //  cout << "Found corners of image " << file << endl;
        //}
        if (subpixSearchSize > 0) 
          cv::cornerSubPix( myCalibImg, corners_of_a_pair[iCam], 
             cv::Size(subpixSearchSize,subpixSearchSize), 
             cv::Size(-1,-1), cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 100, 0.01));
        //if ( debugLevel >= 3 ) {
        //  cout << "Finding corners time:" 
        //       << (clock()-t0)/CLOCKS_PER_SEC << " sec.\n";
        //}
        // output drawn corners
        if ( this->drawChessCorners == true ) {
          //      t0 = clock();
          cv::drawChessboardCorners( myCalibImg, patternSize, corners_of_a_pair[iCam], found );
//          cv::imwrite( file + this->drawnCornersSuffix, myCalibImg );
          cv::imshow("Show", myCalibImg); 
          cv::waitKey(); 
          //if ( debugLevel >= 3 ) {
          //    cout << "Drawing & writing corners time: " 
          //         << (clock()-t0)/CLOCKS_PER_SEC << " sec.\n";
          //}
        }
      } else { // if not found 
        //if ( debugLevel >= 1 ) 
        //  cerr << "Can not find corners of image " << file << endl;
        //SPRINTF(errorMessage, "Error: CamCalib: Cannot find corners in this image.");
        //return -1; 
        for (int iCam2 = 0; iCam2 < this->nCam; iCam2++)  // one cam fails and all cams fail.
          this->calibImgIndex[iCam2][iPair] = -1; 
        break;      }  
    } // end of for iCam (from 0 to nCam)
    // if all corners of all cameras of this pair are found, add all corners to cornersVec[iCam][iPair]
    if (iCam >= this->nCam) {  // all found
      for (iCam = 0; iCam < this->nCam; iCam++) {
        cornersVec[iCam][nCornersVecAdded] = corners_of_a_pair[iCam];
        // if all corners are found, set calibImgIndex[iCam][iPair] = index (iPair)
        this->calibImgIndex[iCam][iPair] = nCornersVecAdded; 
      }
      nCornersVecAdded++; 
      addedAnyPair = true; 
    }
  } // end of for iPair
  // resize cornersVec
  for (int iCam = 0; iCam < this->nCam; iCam++) {
    this->cornersVec[iCam].resize(nCornersVecAdded); 
  }
  return addedAnyPair;
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
int chessboardObjectPoints(const  cv::Size patternSize,  // # of inner corners per a chessboard row and column
                            int    nViews,                // # of views (images) of chessboard
                            double squareSizeX,           // width of square (x distance between corners)
                            double squareSizeY,           // height of square (y distance between corners)
                            vector<vector<cv::Point3f>> & object_points) // object points of chessboard corners
{
    int iView, iY, iX, nY, nX, ic;
    nY = patternSize.height; 
    nX = patternSize.width;
    // resize the object_points object
    object_points.clear();
    object_points.resize(nViews);
    for (iView = 0; iView < nViews; iView++) {
        object_points[iView].resize(nY*nX);
    }
    // fill the object points
    for (iView = 0; iView < nViews; iView++) {
        ic = 0;
        for (iY = nY - 1; iY >= 0; iY--) {
            for (iX = 0; iX < nX; iX++) {
                object_points[iView][ic].x = (float)(iX * squareSizeX);
                object_points[iView][ic].y = (float)(iY * squareSizeY);
                object_points[iView][ic].z = (float) 0.0;
                ic++;
            }
        }
    }
    return 0; 
}

double CamCalib::calibrate() 
{
  // Check if there are sufficient images
  for (int iCam = 0; iCam < this->nCam; iCam++) {
    if ( calibImgFiles[iCam].size() < 1 ) {
      //if ( debugLevel >= 1 ) 
      //  cerr << "Error: Insufficient calibration image.\n";
      SPRINTF(errorMessage, "Error: CamCalib: no calibration image."); 
      return -1.0;
    }
  }

  // Check cell size
  if ( cellWidth <= 0.0 || cellHeight <= 0.0 ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Error: chessboard cell size is not assigned yet.\n";
    SPRINTF(errorMessage, "Error: CamCalib: cell sizes are not set yet."); 
    return -1.0;
  }

  // Check if all cams have same # of cal. images.
  for (int iCam = 0; iCam < this->nCam; iCam++)  {
    if (cornersVec[iCam].size() != cornersVec[0].size()) {
      //if ( debugLevel >= 1 ) 
      //  cerr << "Error: Cam 0 and Cam 1 have different numbers of images.\n";
      SPRINTF(errorMessage, 
        "Error: CamCalib: Left/right cams image numbers are not matched (Cam %d:%d imgs. Cam %d:%dimgs)",
        0, cornersVec[0].size(), iCam, cornersVec[iCam].size() ); 
      return -1.0;
    }
  }

  // find all corners of all image files
  bool res = this->findAllCalibImgsCorners(); 
  if (res == false) 
    return -1.0; 

  // generate object points of chessboard
  vector<vector<cv::Point3f>> object_points;
  chessboardObjectPoints( patternSize, (int) cornersVec[0].size(), 
                          cellWidth, cellHeight, 
                          object_points );

  // calibrate camera 0 (left or single)
//  vector<vector<double>> rvecs_0, tvecs_0;
  for (int iCam = 0; iCam < this->nCam; iCam++) {
    this->camCalibErr[iCam] = cv::calibrateCamera( object_points, 
                          cornersVec[iCam], 
                          calibImgSize[iCam], 
                          camMat[iCam], 
                          disCoe[iCam], 
                          rvecs[iCam], tvecs[iCam], 
                          calibFlag, 
                          cv::TermCriteria( cv::TermCriteria::COUNT+cv::TermCriteria::EPS, 30, DBL_EPSILON) ); 
  }
  //if ( debugLevel >= 3 ) {
  //  cout << "Cam 0 calibration error: " << this->camCalibErr_0 << endl;
  //}

  if ( nCam <= 1 ) {
    return this->camCalibErr[0];
  }

  // stereo calibration for iCam 0 and 1
  this->stereoCalibErr = cv::stereoCalibrate(  object_points, 
                                cornersVec[0], 
                                cornersVec[1], 
                                camMat[0], 
                                disCoe[0], 
                                camMat[1], 
                                disCoe[1],
                                calibImgSize[0], // assuming all calib. images are the same size. 
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


cv::Mat CamCalib::getCamMatrix (int iCam) 
{
  // check if camId is valid
  if ( iCam < 0 || iCam >= this->nCam ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Warning: Invalid camId . Taking it as 0 (left cam).\n";
    // camId = 0;
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", iCam); 
    return cv::Mat(); 
  }
  // run calibration if it is not done yet.
  if ( calibrated == false ) {   
    this->calibrate();
  }
  return camMat[iCam]; 
}

cv::Mat CamCalib::getDisCoeffs (int iCam) 
{
  // check if camId is valid
  if ( iCam < 0 || iCam >= this->nCam ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Warning: Invalid camId . Taking it as 0 (left cam).\n";
    // camId = 0;
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", iCam); 
    return cv::Mat(); 
  }
  // run calibration if it is not done yet.
  if ( calibrated == false ) {   
    this->calibrate();
  }
  return disCoe[iCam]; 
}

int     CamCalib::getChessboardRVecs(int imageId, cv::Mat & rvec, int iCam) {
  // check data 
  if (iCam >= this->nCam) {
    // camId is too large, not a valid camera Id. 
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", iCam); 
    return -1;
  }
  if (imageId >= (int) calibImgFiles[iCam].size() ) {
    // imageId is too large, not a valid image Id.
    SPRINTF(errorMessage, "Error: CamCalib: Invalid image %d", imageId); 
    return -1;
  }
  if (this->calibImgIndex[iCam][imageId] < 0) {
    // this image is not valid, maybe because corners are not all found.
    SPRINTF(errorMessage, "Error: CamCalib: Image %d not valid, maybe becuase some corners are not found.", imageId);
    return -1;
  }
  // Get the R (rotation) vector of a chessboard (imageId) wrt a camera (camId)
  if (iCam >= 0 && iCam < this->nCam) {
    int index = this->calibImgIndex[iCam][imageId]; 
    rvec = this->rvecs[iCam][index]; 
    return 0;
  }  else { 
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", iCam); 
    return -1; 
  }
  return 0; 
}

int     CamCalib::getChessboardTVecs(int imageId, cv::Mat & tvec, int iCam) {
  // check data 
  if (iCam >= this->nCam) {
    // camId is too large, not a valid camera Id. 
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", iCam); 
    return -1;
  }
  if (imageId >= (int) calibImgFiles[iCam].size() ) {
    // imageId is too large, not a valid image Id.
    SPRINTF(errorMessage, "Error: CamCalib: Invalid image %d", imageId); 
    return -1;
  }
  if (this->calibImgIndex[iCam][imageId] < 0) {
    // this image is not valid, maybe because corners are not all found.
    SPRINTF(errorMessage, "Error: CamCalib: Image %d not valid, maybe becuase some corners are not found.", imageId);
    return -1;
  }
  // Get the T (translation) vector of a chessboard (imageId) wrt a camera (camId)
  if (iCam >= 0 && iCam < this->nCam) {
    int index = this->calibImgIndex[iCam][imageId]; 
    tvec = this->tvecs[iCam][index]; 
    return 0;
  }  else { 
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", iCam); 
    return -1; 
  }
  return 0; 
}

int     CamCalib::getCornersImagePoints(int imageId, vector<cv::Point2f> & imagePoints, int iCam)
{
  // check data 
  if (iCam >= this->nCam) {
    // camId is too large, not a valid camera Id. 
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", iCam); 
    return -1;
  }
  if (imageId >= calibImgFiles[iCam].size() ) {
    // imageId is too large, not a valid image Id.
    SPRINTF(errorMessage, "Error: CamCalib: Invalid image %d", imageId); 
    return -1;
  }
  if (this->calibImgIndex[iCam][imageId] < 0) {
    // this image is not valid, maybe because corners are not all found.
    SPRINTF(errorMessage, "Error: CamCalib: Image %d not valid, maybe becuase some corners are not found.", imageId);
    return -1;
  }
  // Get the image points of corners of a chessboard (imageId) wrt a camera (camId)
  if (iCam >= 0 && iCam < this->nCam) {
    int index = this->calibImgIndex[iCam][imageId]; 
    imagePoints = this->cornersVec[iCam][index]; 
    return 0;
  } else { 
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", iCam); 
    return -1; 
  }
  return 0; 
}

int     CamCalib::getPhotoSize(cv::Size & imgsize, int iCam)
{
  // check data 
  if (iCam < 0 || iCam >= this->nCam) {
    // camId is too large, not a valid camera Id. 
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", iCam); 
    return -1;
  }
  imgsize = this->calibImgSize[iCam];
  return 0; 
}


double  CamCalib::getCamCalibErr(int iCam) 
{
  // check if camId is valid
  if ( iCam < 0 || iCam >= this->nCam ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Warning: Invalid camId . Taking it as 0 (left cam).\n";
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", iCam); 
    return -1;
  }
  // run calibration if it is not done yet.
  if ( calibrated == false ) {   
    this->calibrate();
  }
  return camCalibErr[iCam]; 
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
  return (int) calibImgFiles[0].size();
}

double  CamCalib::getFx(int iCam) 
{
  // check if camId is valid
  if ( iCam < 0 || iCam >= this->nCam ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Warning: Invalid camId . Taking it as 0 (left cam).\n";
    // camId = 0;
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", iCam); 
    return -1; 
  }
  // run calibration if it is not done yet.
  if ( calibrated == false ) {   
    this->calibrate();
  }
  return camMat[iCam].at<double>(0,0) ; 
}

double  CamCalib::getFy(int iCam) 
{
  // check if camId is valid
  if ( iCam < 0 || iCam >= this->nCam ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Warning: Invalid camId . Taking it as 0 (left cam).\n";
    // camId = 0;
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", iCam); 
    return -1; 
  }
  // run calibration if it is not done yet.
  if ( calibrated == false ) {   
    this->calibrate();
  }
  return camMat[iCam].at<double>(1,1) ; 
}

double  CamCalib::getCx(int iCam) 
{
  // check if camId is valid
  if ( iCam < 0 || iCam >= this->nCam ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Warning: Invalid camId . Taking it as 0 (left cam).\n";
    // camId = 0;
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", iCam); 
  }
  // run calibration if it is not done yet.
  if ( calibrated == false ) {   
    this->calibrate();
  }
  return camMat[iCam].at<double>(0,2) ; 
}

double  CamCalib::getCy(int iCam) 
{
  // check if camId is valid
  if ( iCam < 0 || iCam >= this->nCam ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Warning: Invalid camId . Taking it as 0 (left cam).\n";
    //camId = 0;
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", iCam); 
    return -1; 
  }
  // run calibration if it is not done yet.
  if ( calibrated == false ) {   
    this->calibrate();
  }
  return camMat[iCam].at<double>(1,2) ; 
}

double  CamCalib::getK1(int iCam)
{
  // check if camId is valid
  if ( iCam < 0 || iCam >= this->nCam ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Warning: Invalid camId . Taking it as 0 (left cam).\n";
    //camId = 0;
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", iCam); 
    return -1; 
  }
  // run calibration if it is not done yet.
  if ( calibrated == false ) {   
    this->calibrate();
  }
  return disCoe[iCam].at<double>(0);
}

double  CamCalib::getK2(int iCam)
{
  // check if camId is valid
  if ( iCam < 0 || iCam >= this->nCam ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Warning: Invalid camId . Taking it as 0 (left cam).\n";
    //camId = 0;
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", iCam); 
    return -1; 
  }
  // run calibration if it is not done yet.
  if ( calibrated == false ) {   
    this->calibrate();
  }
  return disCoe[iCam].at<double>(1);
}

double  CamCalib::getP1(int iCam)
{
  // check if camId is valid
  if ( iCam < 0 || iCam >= this->nCam ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Warning: Invalid camId . Taking it as 0 (left cam).\n";
    //camId = 0;
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", iCam); 
    return -1; 
  }
  // run calibration if it is not done yet.
  if ( calibrated == false ) {   
    this->calibrate();
  }
  return disCoe[iCam].at<double>(2);
}

double  CamCalib::getP2(int iCam)
{
  // check if camId is valid
  if ( iCam < 0 || iCam >= this->nCam ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Warning: Invalid camId . Taking it as 0 (left cam).\n";
    //camId = 0;
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", iCam); 
    return -1; 
  }
  // run calibration if it is not done yet.
  if ( calibrated == false ) {   
    this->calibrate();
  }
  return disCoe[iCam].at<double>(3);
}

double  CamCalib::getK3(int iCam)
{
  // check if camId is valid
  if ( iCam < 0 || iCam >= this->nCam ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Warning: Invalid camId . Taking it as 0 (left cam).\n";
    //camId = 0;
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", iCam); 
    return -1; 
  }
  // run calibration if it is not done yet.
  if ( calibrated == false ) {   
    this->calibrate();
  }
  return disCoe[iCam].at<double>(4);
}

double  CamCalib::getK4(int iCam)
{
  // check if camId is valid
  if ( iCam < 0 || iCam >= this->nCam ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Warning: Invalid camId . Taking it as 0 (left cam).\n";
    //camId = 0;
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", iCam); 
    return -1; 
  }
  // run calibration if it is not done yet.
  if ( calibrated == false ) {   
    this->calibrate();
  }
  if ( disCoe[iCam].cols <= 5 ) 
      return 0.0;
  else 
      return disCoe[iCam].at<double>(5);
}

double  CamCalib::getK5(int iCam)
{
  // check if camId is valid
  if ( iCam < 0 || iCam >= this->nCam ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Warning: Invalid camId . Taking it as 0 (left cam).\n";
    //camId = 0;
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", iCam); 
    return -1; 
  }
  // run calibration if it is not done yet.
  if ( calibrated == false ) {   
    this->calibrate();
  }
  if ( disCoe[iCam].cols <= 6 ) 
      return 0.0;
    else 
      return disCoe[iCam].at<double>(6);
}

double  CamCalib::getK6(int iCam)
{
  // check if camId is valid
  if ( iCam < 0 || iCam >= this->nCam ) {
    //if ( debugLevel >= 1 ) 
    //  cerr << "Warning: Invalid camId . Taking it as 0 (left cam).\n";
    //camId = 0;
    SPRINTF(errorMessage, "Error: CamCalib: Invalid camId %d", iCam); 
    return -1; 
  }
  // run calibration if it is not done yet.
  if ( calibrated == false ) {   
    this->calibrate();
  }
  if ( disCoe[iCam].cols <= 7 ) 
    return 0.0;
  else 
    return disCoe[iCam].at<double>(7);
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


int CamCalib::undistortImg(string srcFile, string dstFile, int iCam)
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
  undistort(srcImg, dstImg, this->camMat[iCam], this->disCoe[iCam]);
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

void CamCalib::reset()
{
	this->calibImgFiles.clear();
	this->calibImgIndex.clear();
	this->calibImgSize.clear();
	this->camCalibErr.clear();
	this->camMat.clear();
	this->cornersVec.clear();
	this->disCoe.clear();
	this->objectPoints.clear();
	this->rvecs.clear();
	this->tvecs.clear();

	return;
}


// This function is supposed to be able to replace cv::findChessboardCorners(). 
// It warps cv::findChessboardCorners() and cornerSubPix().
// It saves CPU time for large image by resizing image before calling cv::findChessboardCorners(). 
// 
bool findChessboardCorners_coarsenAndSubpix(cv::InputArray _image, cv::Size patternSize, 
     cv::OutputArray _corners, int flags)
{
  bool found; 
  cv::Mat image = _image.getMat(); 
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
    subpixSearchSize = (int)(1.f / ratio + 0.5f + 3); 
    int w1 = (int) (w0 * ratio + 0.5); 
    int h1 = (int) (h0 * ratio + 0.5); 
    cv::Mat coarsen_image;
    cv::resize(image, coarsen_image, cv::Size(w1, h1)); 
    found = cv::findChessboardCorners( coarsen_image, patternSize, _corners, flags);
    // if found, refine the coordinates of corners
    if (found == true) {
      cv::Mat corners = _corners.getMat(); 
      for (int iw = 0; iw < corners.cols; iw++) {
        for (int ih = 0; ih < corners.rows; ih++) {
          //corners.at<cv::Point2f>(ih, iw).x *= (1.0f * w0 / w1); 
          //corners.at<cv::Point2f>(ih, iw).y *= (1.0f * h0 / h1); 
          float x = corners.at<cv::Point2f>(ih, iw).x; 
          float y = corners.at<cv::Point2f>(ih, iw).y; 
          x *= (1.0f * w0 / w1); 
          y *= (1.0f * h0 / h1); 
          corners.at<cv::Point2f>(ih, iw).x = x; 
          corners.at<cv::Point2f>(ih, iw).y = y; 
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

