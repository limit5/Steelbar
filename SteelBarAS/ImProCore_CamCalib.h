// Copyright (c) 2014, Yuan-Sen Yang (Copyright holder)(mailto: yuansen@hotmail.com)  
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification, are permitted 
// provided that the following conditions are met: 
// 
// 1. Redistributions of source code must retain the above copyright notice, this list of 
//    conditions and the following disclaimer. 
// 2. Redistributions in binary form must reproduce the above copyright notice, this list of 
//    conditions and the following disclaimer in the documentation and/or other materials provided
//    with the distribution. 
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY 
// AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
// POSSIBILITY OF SUCH DAMAGE.
// 
// The views and conclusions contained in the software and documentation are those of the authors
// and should not be interpreted as representing official policies, either expressed or implied, 
// of the FreeBSD Project.
//
// (FreeBSD License)

// 
// class CamCalib
// 
// Description: 
//   CamCalib is designed to simplify chessboard camera calibration (single and/or stereo) process.
//   The process is: 
//   1. User throws chessboard calibration filename or image Mat picture by picture by calling
//      ::addCalibImg. 
//      User can assign camera Id if there are two or more cameras. 
//      This class finds and manages the chessboard corners image coordinate data and generates 
//      object coordinate data. 
//   2. User sets pattern size (how many corners) and cell size, and further sets optional flags. 
//   3. User calls ::calibrate(). This class runs calibration (single or/and stereo). 
//   4. User gets camera parameters by calling ::getCamMatrix(), ::getDisCoeffs(), ... 
// 
// Functions:
//
//
//

// Developer: Yuan-Sen Yang
// 
// Date:  2013-02-07  Initial implementation 
//        2014-02-07  Renamed to ImProCore_CamCalib
//                    Replaced user interface (iostream, stdio) with input/output parameters. 
//        2014-02-12  Added the following functions:
//                    getChessboardRVecs(), getChessboardTVecs(), 
//                    getCornersImagePoints()
//        2014-03-26  Added image coarsening before running findChessboardCorners(). 
//                    Implemented findChessboardCorners_coarsenAndSubpix(). 
//                    Made modifications that when running ::addCalibImg() it only stores file names
//                      without running findChessboardCorners(). Calling ::calibrate() finds the 
//                      corners. 
//        2014-04-16  Bug fixed (findChessboardCorners_coarsenAndSubpix subpixel search window size)
//                    replace ratio + 0.5 + 3 with 1.f / ratio + 0.5 + 3


#ifndef _CamCalib_H_
#define _CamCalib_H_

#define LEFT_CAM  0
#define RIGHT_CAM 1

#include <vector>
#include "opencv2/core/core.hpp"

using namespace std; 

class CamCalib
{
public:
  CamCalib(int nCamera = 1);
  ~CamCalib(void);
  int addCalibImg(string file, int camId = LEFT_CAM); 
          // returns 0 if it succeeds, else if it fails. 
          // cam id: 0 is left cam (default); 1 is right cam. 

  int     setPatternSize(int cols, int rows); 
  int     setCellSize(double cellWidth, double cellHeight); 
  double  getFx (int camId = LEFT_CAM); 
  double  getFy (int camId = LEFT_CAM) ; 
  double  getCx (int camId = LEFT_CAM) ;
  double  getCy (int camId = LEFT_CAM) ;
  double  getK1 (int camId = LEFT_CAM) ;
  double  getK2 (int camId = LEFT_CAM) ;
  double  getP1 (int camId = LEFT_CAM) ;
  double  getP2 (int camId = LEFT_CAM) ;
  double  getK3 (int camId = LEFT_CAM) ;
  double  getK4 (int camId = LEFT_CAM) ;
  double  getK5 (int camId = LEFT_CAM) ;
  double  getK6 (int camId = LEFT_CAM) ;
  cv::Mat getR    () ;  // not valid for nCam = 1
  cv::Mat getT    () ;  // not valid for nCam = 1
  cv::Mat getF    () ;  // not valid for nCam = 1
  cv::Mat getE    () ;  // not valid for nCam = 1
  cv::Mat getRodriguesOfR() ; 
  cv::Mat getCamMatrix (int camId = LEFT_CAM) ;
  cv::Mat getDisCoeffs (int camId = LEFT_CAM) ;
  int     getChessboardRVecs(int imageId, cv::Mat & rvec, int camId = LEFT_CAM); 
  int     getChessboardTVecs(int imageId, cv::Mat & rvec, int camId = LEFT_CAM); 
  int     getCornersImagePoints(int imageId, vector<cv::Point2f> & imagePoints, int camId = LEFT_CAM); 
  int     getPhotoSize(cv::Size & imgsize, int camId = LEFT_CAM); 
  double  getCamCalibErr  (int camId = LEFT_CAM) ;
          // camId == 0: camera 0 (left)  (in pixels)
          // camId == 1: camera 1 (right) (in pixels)
  double  getStereoCalibErr(); 
          
  int     nImagePairs () const ; 
  void    setFlag_UseIntrinsicGuess(bool set = true); // 2^0
  void    setFlag_FixAspectRatio   (bool set = true); // 2^1
  void    setFlag_FixPrincipalPoint(bool set = true); // 2^2
  void    setFlag_ZeroTangentDist  (bool set = true); // 2^3
  void    setFlag_FixFocalLength   (bool set = true); // 2^4
  void    setFlag_FixK1            (bool set = true); // 2^5 
  void    setFlag_FixK2            (bool set = true); // 2^6
  void    setFlag_FixK3            (bool set = true); // 2^7
  void    setFlag_FixIntrinsic     (bool set = true); // 2^8
  void    setFlag_SameFocalLength  (bool set = true); // 2^9
  void    setFlag_ZeroDisparity    (bool set = true); // 2^10
  void    setFlag_FixK4            (bool set = true); // 2^11
  void    setFlag_FixK5            (bool set = true); // 2^12
  void    setFlag_FixK6            (bool set = true); // 2^13
  void    setFlag_Rational_Model   (bool set = true); // 2^14

  void    setDebugLevel(int level);                   
          // 0: No debugging info will be printed to cerr.
          // 1: Only Vital error messages will be printed to cerr.
          // 2: Level 1 + warning messages will be printed to cerr.
          // 3: Level 2 + analysis info. will be printed to cerr.
          // 4: Level 3 + timing statistics will be printed to cerr. 

  void    setDrawChessCorners(bool   set = true, 
                              string suffix = ".ChessCorners.jpg");
          // If it is set, new images with corners drawn will be generated.
          // This function should be called before addCalibImg().

  double  calibrate   ();

  // undistort points / images
  int     undistortImg(string srcFile, string dstFile, int camId = LEFT_CAM);
          // This function undistorts an image to an undistorted image.
          // I.e., it transforms an image to compensate for lens distortion.
          // See OpenCV C++ void undistort function for more details. 

  const char* getErrorMessage() const;  

  void reset();

protected:
  bool findAllCalibImgsCorners(); 
       // This function is internal, not for public use. 
       // This function finds chessboard corners of each added image. If successfully, adds found 
       // corners to cornersVec_0 or cornersVec_1, also adds object points (3D) to objectPoints. 
       // If nCam >= 2, call cameras must have the same number of images (i.e., 
       // calibImgFiles_0.size() == calibImgFiles_1.size(). If not, it returns -1 immediately.
       // If nCam >= 2 (say 2), it adds corners/object points only if corners of both images
       // are successfully found. 

protected:
  int                         nCam; 
  cv::Size                    patternSize;             // # of cells of chessboard. (E.g., 10*6 is a wide chessboard)
  double                      cellWidth, cellHeight;   // phyical size of a chessboard cell
  vector<cv::Size>            calibImgSize;            // [iCam]: image size (camera resolution) of camera [0] and [1]
  vector<vector<string>>      calibImgFiles;           // [iCam][iPair]: image file name of iCam of iPair
  vector<vector<int>>         calibImgIndex;           // [iCam][iPair]: calibImgIndex[iCam][iPair] is the index of the corners of this image in cornersVec                                             
                                                       //    Note: not all calibImgFiles's corners are found and added into cornersVec.
                                                       //          Only those corners that are all found are added. 
                                                       //          So we need an index mapping to find out where are corners of each image
                                                       //          Negative index denotes corners are not found and added
  vector<vector<vector<cv::Point2f>>> cornersVec;      // [iCam][iPair][iPoint]: image coord. (2D) of corners in each calib image 
  vector<vector<cv::Point3f>> objectPoints;            // 3D coord. of corners in each calib. image
  vector<cv::Mat>             camMat;                  // [iCam]: camera matrix of iCam
  vector<cv::Mat>             disCoe;                  // [iCam]: distortion coefficients of iCam
  cv::Mat                     R, T, F, E;              // extrinsic parameters (matrices) of the stereo camera
  vector<vector<cv::Mat>>     rvecs, tvecs;            // [iCam][iPair]: coord. system (ext. parm) of each calib. image. Each Mat size is 3x1.
  vector<double>              camCalibErr;             // [iCam] calib. error of iCam
  double                      stereoCalibErr;          // calib. error of stereo camera
  bool                        calibrated;              // the camera(s) are calibrated (intrinsic/
                                                       //   extrinsic parameters are available)
  int                         calibFlag;               // flags (options) for calibration
  int                         debugLevel;              
  bool                        drawChessCorners;
  string                      drawnCornersSuffix;
  char                        errorMessage[1000];            
};


// Key OpenCV function(s)
// C++: double stereoCalibrate(
//             InputArrayOfArrays objectPoints, 
//             InputArrayOfArrays imagePoints0, 
//             InputArrayOfArrays imagePoints1, 
//             InputOutputArray   cameraMatrix0, 
//             InputOutputArray   distCoeffs0, 
//             InputOutputArray   cameraMatrix1, 
//             InputOutputArray   distCoeffs1, 
//             Size               imageSize, 
//             OutputArray        R,    
//             OutputArray        T, 
//             OutputArray        E, 
//             OutputArray        F, 
//             TermCriteria criteria=TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 30, 1e-6), 
//             int flags=CALIB_FIX_INTRINSIC )

#endif 
