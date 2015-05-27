#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "ImProHui_pickAPoint.h"
#include "ImProCore_CamCalib.h"

#include "gettime.h"
#include <Windows.h>
#include <fstream>
#include "opencv2/calib3d/calib3d.hpp"
#include "function.h"
using namespace std; 

#ifdef _WIN32
#define SPRINTF sprintf_s 
#define SCANF   scanf_s
#else
#define SPRINTF sprintf
#define SCANF   scanf
#endif

// This function is used to test the classes/functions in ImPro2014 Library. 
// Normally each test program is a main-program-like function with arguments of 
// (int argc, char** argv), and can be called directly like a main program. 

int zz_SteelBarTensileTest(parameter param) {
  int returnVal; 

  // Step 1: Calibration 
  printf("Step 1: Calibration\n");
  double ticWallCalib, tocWallCalib, ticCpusCalib, tocCpusCalib;
  ticWallCalib = getWallTime();
  ticCpusCalib = getCpusTime();

  //         Calibration: set calibration object
  double cellSizeDx, cellSizeDy; 
  int    patternSizeNx, patternSizeNy;
  cv::Size photoSize; 
  cellSizeDx = param.cellSizeDx;
  cellSizeDy = param.cellSizeDy;
  patternSizeNx = param.patternSizeNx; 
  patternSizeNy = param.patternSizeNy; 

  CamCalib thisCamCalib(1 /* number of camera */ ); 
  thisCamCalib.setCellSize(cellSizeDx, cellSizeDy); 
  thisCamCalib.setPatternSize(patternSizeNx, patternSizeNy); 

  //         Calibration: add calibration images
  vector<string> calibImageFilenames;
  std::ifstream icalibFile("calibFileList.txt");


  if (icalibFile.fail())
  {
	  MessageBox(NULL, L"open calib file list failed.", L"error!", NULL);
	  return -1;
  }
  int calibImgCount = 0; // number of images
  for (std::string line; std::getline(icalibFile, line);)
  {
	  calibImageFilenames.push_back(line);
	  std::cout << line << std::endl; // debug
	  calibImgCount++;
  }
  if (calibImgCount < 1)
  {
	  MessageBox(NULL, L"number of calibration image can not less than 1", L"error!", NULL);
	  return -1;
  }
  for (int i = 0; i < calibImgCount; i++)
  {
	  returnVal = thisCamCalib.addCalibImg(calibImageFilenames[i]);
  }
  //returnVal = thisCamCalib.addCalibImg("..\\images\\SteelBarTensileTest\\DSC_0041.JPG");
  //returnVal = thisCamCalib.addCalibImg("..\\images\\SteelBarTensileTest\\DSC_0042.JPG");
  //returnVal = thisCamCalib.addCalibImg("..\\images\\SteelBarTensileTest\\DSC_0043.JPG");
  returnVal = thisCamCalib.getPhotoSize(photoSize); 

  //         Calibration: run calibration
  MessageBox(NULL, L"calibrate start.", L"message", NULL);
  thisCamCalib.calibrate(); 
  MessageBox(NULL, L"calibrate done.", L"message", NULL);

  // out put to file
  FILE* fp;
  char calibDataFilename[260] = "calibrationParameter.txt";

  if ((fopen_s(&fp, calibDataFilename, "w")))
  {
	  fprintf(stderr, "Failed to open calibration data file.");
	  MessageBox(NULL, L"Can not create calibration data file", L"Warning!", NULL);
	  
	  FILETIME ft1, ft2, ft3;
	  ULARGE_INTEGER ui1, ui2, ui3;
	  SYSTEMTIME st;
	  GetSystemTimes(&ft1, &ft2, &ft3);
	  ui1.HighPart = ft1.dwHighDateTime;
	  ui1.LowPart = ft1.dwLowDateTime;
	  ui2.HighPart = ft2.dwHighDateTime;
	  ui2.LowPart = ft2.dwLowDateTime;
	  ui3.HighPart = ft3.dwHighDateTime;
	  ui3.LowPart = ft3.dwLowDateTime;
	  SPRINTF(calibDataFilename, "%d", ui2.QuadPart);
	  if ((fopen_s(&fp, calibDataFilename, "w")))
	  {
		  MessageBox(NULL, L"error...\r\ncan not create file!!", L"E! R! R! O! R!", NULL);
		  return -1;
	  }
  }
  MessageBox(NULL, L"create calibration data file done.", L"message", NULL);

  // print calibration result
  int imageIdOfGlobalCoord;  // the chessboard that represents the global coordinate
  imageIdOfGlobalCoord = 0;  // the 1st calibration represents the global coordinate
  cv::Mat camMatrix = thisCamCalib.getCamMatrix(0 /* camId */); 
  cv::Mat disCoeffs = thisCamCalib.getDisCoeffs(0 /* camId */);
  cv::Mat coordR, coordT; 
  returnVal = thisCamCalib.getChessboardRVecs(imageIdOfGlobalCoord, coordR, 0);
  returnVal = thisCamCalib.getChessboardTVecs(imageIdOfGlobalCoord, coordT, 0);
  fprintf(fp, "        Fx/Fy: %9.3f / %9.3f\n", camMatrix.at<double>(0,0), camMatrix.at<double>(1,1));
  fprintf(fp, "        Cx/Cy: %9.3f / %9.3f\n", camMatrix.at<double>(0,2), camMatrix.at<double>(1,2));
  fprintf(fp, "        Calib. error: %9.3f\n", thisCamCalib.getCamCalibErr(0) );
  cout << "        Coord. R (1st calibration chessboard): \n";
  cout << coordR << endl;
  cout << "        Coord. T (1st calibration chessboard): \n";
  cout << coordT << endl;

  tocWallCalib = getWallTime();
  tocCpusCalib = getCpusTime();
  fprintf(fp, "        Calibration Wall/Cpus time: %9.3f/%9.3f\n", 
    tocWallCalib - ticWallCalib, tocCpusCalib - ticCpusCalib );

  // Step 2: Generate grid object points (3D) of observed region
  fprintf(fp, "Step 2: Generate grid object points\n");
  double ticWallGridObj, tocWallGridObj, ticCpusGridObj, tocCpusGridObj;
  ticWallGridObj = getWallTime();
  ticCpusGridObj = getCpusTime();

  //         generate object points: calculate rpx
  float rpx; // physical length per pixel, for example, if using mm as unit, 
  // rpx = 0.1 means 0.1 mm equals to the width or height of a pixel. 
  // small rpx leads to a large rectfImage
  float physicalDistance, imageDistance; 
  vector<cv::Point2f> chessImagePoints; 
  returnVal = thisCamCalib.getCornersImagePoints(imageIdOfGlobalCoord, chessImagePoints);
  physicalDistance = (float) sqrt(pow(cellSizeDx * (patternSizeNx - 1), 2)  
    + pow(cellSizeDy * (patternSizeNy - 1), 2) ); 
  imageDistance = (float) sqrt(pow(chessImagePoints[0].x 
    - chessImagePoints[patternSizeNx * patternSizeNy - 1].x, 2)
    + pow(chessImagePoints[0].y 
    - chessImagePoints[patternSizeNx * patternSizeNy - 1].y, 2));
  rpx = physicalDistance / imageDistance * 0.5f;  // multiply 0.5 for refiner rectf. image.
  fprintf(fp, "        rpx(%f) = phyDist(%f) / imgDist(%f) * 0.5\n", rpx, physicalDistance, imageDistance);

  //          generate object points: generate object points of dense grid (3D) (gridObjectPoints3D)
  //                      Note: They are large arrays, requiring large memory
  int rectfImageHeight, rectfImageWidth; // size of rectf. image that we will generate
  int ix, iy; 
  float x, y;  
  rectfImageHeight = (int) (cellSizeDy * (patternSizeNy - 1) / rpx + 1.0); 
  rectfImageWidth  = (int) (cellSizeDx * (patternSizeNx - 1) / rpx + 1.0); 
  cv::Mat gridObjectPoints3D(rectfImageHeight, rectfImageWidth, CV_32FC3 /* float, float, float */); 
  fprintf(fp, "        rectf. image with size of %dx%d.\n", rectfImageHeight, rectfImageWidth);
  fprintf(fp, "        Grid (3d) arrays memory cost: %f MB\n", 
    3. * rectfImageHeight * rectfImageWidth * sizeof(float) / (1024 * 1024)); 
  for (iy = 0; iy < rectfImageHeight; iy++) {
    y = iy * rpx; // convert from pixel to physical size (e.g., mm) 
    for (ix = 0; ix < rectfImageWidth; ix++) {
      x = ix * rpx; 
      gridObjectPoints3D.at<cv::Point3f>(rectfImageHeight - iy - 1, ix).x = x; 
      gridObjectPoints3D.at<cv::Point3f>(rectfImageHeight - iy - 1, ix).y = y; 
      gridObjectPoints3D.at<cv::Point3f>(rectfImageHeight - iy - 1, ix).z = 0.f; 
    }
  }

  tocWallGridObj = getWallTime();
  tocCpusGridObj = getCpusTime();
  fprintf(fp, "        Generating grid obj. Wall/Cpus time: %9.3f/%9.3f\n", 
    tocWallGridObj - ticWallGridObj, tocCpusGridObj - ticCpusGridObj);

  // Step 3: Generate grid image points (2D) 
  //         Convert grid object points (gridObjectPoints3D) to image points (gridImagePoints)
  //                      Note: A grid is naturally 2D (represents an image), but projectPoints()
  //                            only accepts 1-D arrays (either 1xN or Nx1 3-channel array, or 
  //                            3xN or Nx3 1-channel array). 
  //                            But then we use remap(), which only accepts 2D arrays. 
  //                            So we use 2D arrays, reshape the 2D arrays to 1D before calling
  //                            projectPoints(), and reshape them back to 2D after projectPoints().
  fprintf(fp, "Step 3: Generate grid image points\n");
  double ticWallGridImg, tocWallGridImg, ticCpusGridImg, tocCpusGridImg;
  ticWallGridImg = getWallTime();
  ticCpusGridImg = getCpusTime();

  cv::Mat gridImagePoints(rectfImageHeight, rectfImageWidth, CV_32FC2 /* float */ ); 
  fprintf(fp, "        Map (2d) arrays memory cost: %f MB\n", 
    2. * rectfImageHeight * rectfImageWidth * sizeof(float) / (1024 * 1024)); 
  cv::projectPoints(gridObjectPoints3D.reshape(0 /* channel not changed */, 1), 
    coordR, coordT, camMatrix, disCoeffs, 
    gridImagePoints.reshape(0 /* channel not changed */, 1)); 

  tocWallGridImg = getWallTime();
  tocCpusGridImg = getCpusTime();
  fprintf(fp, "        Generating grid img points. Wall/Cpus time: %9.3f/%9.3f\n", 
    tocWallGridImg - ticWallGridImg, tocCpusGridImg - ticCpusGridImg);

  // Step 4: Generate rectf. image(s).
  // 
  fprintf(fp, "Step 4: Generate rectf. image.\n");
  double ticWallRectfImg, tocWallRectfImg, ticCpusRectfImg, tocCpusRectfImg;
  ticWallRectfImg = getWallTime();
  ticCpusRectfImg = getCpusTime();

  vector<string> testImageFilenames;
  std::ifstream iFile("testFileList.txt");
  if (iFile.fail())
  {
	  MessageBox(NULL, L"open test file list failed.", L"error!", NULL);
  }
  int testCount = 0; // number of images
  for (std::string line; std::getline(iFile, line);)
  {
	  testImageFilenames.push_back(line);
	  std::cout << line << std::endl; // debug
	  testCount++;
  }
  if (testCount < 1)
  {
	  MessageBox(NULL, L"number of test image can not less than 1", L"error!", NULL);
	  return -1;
  }
  //         generate rectf. image by remapping image
  
  vector<string> testRectfFilenames;
  testRectfFilenames.clear();
  testRectfFilenames.resize(testCount);
  for (int i = 0; i < testCount; i++)
  {
	  testRectfFilenames[i] = "rectf\\rectf_" + testImageFilenames[i];
	  
  }
  //testImageFilenames[0] = "..\\images\\SteelBarTensileTest\\DSC_0040.JPG"; 
  //testImageFilenames[1] = "..\\images\\SteelBarTensileTest\\DSC_0041.JPG"; 
  //testImageFilenames[2] = "..\\images\\SteelBarTensileTest\\DSC_0042.JPG"; 
  //testImageFilenames[3] = "..\\images\\SteelBarTensileTest\\DSC_0043.JPG"; 
  //testRectfFilenames[0] = "..\\images\\SteelBarTensileTest\\DSC_0040_rectf.JPG"; 
  //testRectfFilenames[1] = "..\\images\\SteelBarTensileTest\\DSC_0041_rectf.JPG"; 
  //testRectfFilenames[2] = "..\\images\\SteelBarTensileTest\\DSC_0042_rectf.JPG"; 
  //testRectfFilenames[3] = "..\\images\\SteelBarTensileTest\\DSC_0043_rectf.JPG"; 
  for (int i = 0; i < testCount; i++) {
    fprintf(fp, "        Generating rectf. image: %s.\n", testImageFilenames[i].c_str()); 
    cv::Mat testImage = cv::imread(testImageFilenames[i]);
	
	//cv::Mat TempTestImage;
	//cv::resize(testImage, TempTestImage, thisCamCalib.getResize());

    // check photo size. Sometimes they are rotated.
    if (testImage.rows != photoSize.height || 
      testImage.cols != photoSize.width) {
        fprintf(stderr, "Image size inconsistent. Calib. photos are %dx%d and %s is %dx%d\n", 
          photoSize.height, photoSize.width, testImage.rows, testImage.cols); 
        exit(1);
    }

    cv::Mat rectfImage; 
    cv::Mat emptyMat; 
    cv::remap(testImage, rectfImage, gridImagePoints, emptyMat, cv::INTER_LINEAR); 
    cv::imwrite(testRectfFilenames[i], rectfImage); 
  }

  tocWallRectfImg = getWallTime();
  tocCpusRectfImg = getCpusTime();
  fprintf(fp, "        Generating grid img points. Wall/Cpus time: %9.3f/%9.3f\n", 
    tocWallRectfImg - ticWallRectfImg, tocCpusRectfImg - ticCpusRectfImg);

  // Step 5: Pick control points
  //         There are four types of control points
  //         1. VPoint: vertically moving control points (must be moving along Y)
  //         2. HPoint: horizontally moving control points (must be moving along X)
  //         3. HCPoint: initially constrained HPoint (a fixed distance with another HPoint)
  //         4. GenPoint: general control points (may move in both X and Y directions)
  int num_VPoint, num_HPoint, num_HCPoint, num_GenPoint, num_CtrlPoints;  // number of different
                                                                      // types of control points
  int itr_VPoint, itr_HPoint, itr_HCPoint, itr_GenPoint; // first one of each type of ctrl points
  num_VPoint = param.nVtemplate; 
  num_HPoint = param.nHtemplate; // These points are supposed to be on the left edge of the steel bar. 
  num_HCPoint = num_HPoint; 
  num_GenPoint = 3; 
  itr_VPoint = 0; 
  itr_HPoint = itr_VPoint + num_VPoint; 
  itr_HCPoint = itr_HPoint + num_HPoint;
  itr_GenPoint = itr_HCPoint + num_HCPoint; 
  num_CtrlPoints = num_VPoint + num_HPoint + num_HCPoint + num_GenPoint; 
  vector<cv::Point2f> ctrlPointsPosition(num_CtrlPoints);    // position of the point wrt image coord.
  vector<cv::Rect>    ctrlPointsWinRange(num_CtrlPoints);    // template range of the point wrt image coord.
  vector<cv::Mat>     ctrlPointsImage   (num_CtrlPoints);    // template image of control point


  vector<cv::Mat>     nextCtrlPointsImage   (num_CtrlPoints);    // template image of control point
  //         read initial test image
  cv::Mat testImg = cv::imread(testRectfFilenames[0]); 

  //         pick Vpoint
  for (int i = 0; i < num_VPoint; i++) {
    int pid = i + itr_VPoint;  // ctrl point id
    //       user picking
    char winTitle[1000];
    SPRINTF(winTitle, "VPoint %d/%d.", i + 1, num_VPoint); 
    ImProHui_pickAPoint(winTitle, testImg, ctrlPointsPosition[pid]); 

    //       user input template size
    int templateHeight, templateWidth; 
    fprintf(fp, "VPoint %d is at X:%9.3f Y:%9.3f.\n", i + 1, ctrlPointsPosition[pid].x, 
                                                        ctrlPointsPosition[pid].y);
    //fprintf(fp, "What is the window (or template or pattern) size for the point? (height and width)\n");
    //fprintf(fp, "  (E.g., If you input 8 10, the window/template/pattern height is 8, width is 10): "); 
	templateWidth = param.vTemplateSizeX;
	templateHeight = param.vTemplateSizeY;
    //SCANF("%d", &templateHeight);
    //SCANF("%d", &templateWidth);

    //       check user's input
    if (templateHeight > testImg.rows) {
      printf("Warning: window size cannot be greater than image size. Set height to %d\n", 
        testImg.rows); 
      templateHeight = testImg.rows; 
    }
    if (templateHeight < 3) {
      printf("Warning: window size cannot be smaller than 3. Set height to 3\n");
      templateHeight = 3; 
    }
    if (templateWidth > testImg.cols) {
      printf("Warning: window size cannot be greater than image size. Set width to %d\n", 
        testImg.cols); 
      templateWidth = testImg.cols; 
    }
    if (templateWidth < 3) {
      printf("Warning: window size cannot be smaller than 3. Set width to 3\n");
      templateWidth = 3; 
    }

    //       set window (template/pattern) left-upper position and size.
    ctrlPointsWinRange[pid].height = templateHeight; 
    ctrlPointsWinRange[pid].width  = templateWidth; 
    ctrlPointsWinRange[pid].x = (int) (ctrlPointsPosition[pid].x - templateWidth / 2.0 + 0.5); 
    if (ctrlPointsWinRange[pid].x < 0) ctrlPointsWinRange[pid].x = 0; 
    ctrlPointsWinRange[pid].y = (int) (ctrlPointsPosition[pid].y - templateHeight / 2.0 + 0.5); 
    if (ctrlPointsWinRange[pid].y < 0) ctrlPointsWinRange[pid].y = 0; 

	if (ctrlPointsWinRange[pid].width + ctrlPointsWinRange[pid].x > testImg.cols)
	{
		MessageBox(NULL, L"V template size x is invalid for test image.", L"error!", NULL);
		return -1;
	}

	if (ctrlPointsWinRange[pid].height + ctrlPointsWinRange[pid].y > testImg.rows)
	{
		MessageBox(NULL, L"V template size y is invalid for test image.", L"error!", NULL);
		return -1;
	}


    //       set window (template/pattern) image and save to file
    ctrlPointsImage[pid] = testImg(ctrlPointsWinRange[pid]); 
    char templateFilename[1000];
    SPRINTF(templateFilename, "template\\VPoint_%03d.JPG", i + 1); 
    cv::imwrite(templateFilename, ctrlPointsImage[pid]); 
  } // end of picking VPoints

//=======================================================================================================================//
//=====================================added by Limit5 at 2014/02/15=====================================================//
//=======================================================================================================================//


  // pick HPoint
  // only first and last point.
  int H_templateHeight, H_templateWidth;
  for (int i = 0; i < num_HPoint; i+=(num_HPoint - 1))
  {
	  int pid = i + itr_HPoint;

	  char winTitle[1000];
	  SPRINTF(winTitle, "HPoint %d/%d.", i + 1, num_HPoint);
	  ImProHui_pickAPoint(winTitle, testImg, ctrlPointsPosition[pid]);


	  int templateHeight, templateWidth;
	  fprintf(fp, "HPoint %d is at X:%9.3f Y:%9.3f.\n", i + 1, ctrlPointsPosition[pid].x, ctrlPointsPosition[pid].y);
	  //fprintf(fp, "  (E.g., If you input 8 10, the window/template/pattern height is 8, width is 10): "); 
	  templateWidth = param.hTemplateSizeX;
	  templateHeight = param.hTemplateSizeY;
	  //SCANF("%d", &templateHeight);
	  //SCANF("%d", &templateWidth);

    //       check user's input
    if (templateHeight > testImg.rows) {
      printf("Warning: window size cannot be greater than image size. Set height to %d\n", 
        testImg.rows); 
      templateHeight = testImg.rows; 
    }
    if (templateHeight < 3) {
      printf("Warning: window size cannot be smaller than 3. Set height to 3\n");
      templateHeight = 3; 
    }
    if (templateWidth > testImg.cols) {
      printf("Warning: window size cannot be greater than image size. Set width to %d\n", 
        testImg.cols); 
      templateWidth = testImg.cols; 
    }
    if (templateWidth < 3) {
      printf("Warning: window size cannot be smaller than 3. Set width to 3\n");
      templateWidth = 3; 
    }

	if (ctrlPointsWinRange[pid].width + ctrlPointsWinRange[pid].x > testImg.cols)
	{
		MessageBox(NULL, L"H template size x is invalid for test image.", L"error!", NULL);
		return -1;
	}

	if (ctrlPointsWinRange[pid].height + ctrlPointsWinRange[pid].y > testImg.rows)
	{
		MessageBox(NULL, L"H template size y is invalid for test image.", L"error!", NULL);
		return -1;
	}

    //       set window (template/pattern) left-upper position and size.
    ctrlPointsWinRange[pid].height = templateHeight; 
    ctrlPointsWinRange[pid].width  = templateWidth; 
    ctrlPointsWinRange[pid].x = (int) (ctrlPointsPosition[pid].x - templateWidth / 2.0 + 0.5); 
    if (ctrlPointsWinRange[pid].x < 0) ctrlPointsWinRange[pid].x = 0; 
    ctrlPointsWinRange[pid].y = (int) (ctrlPointsPosition[pid].y - templateHeight / 2.0 + 0.5); 
    if (ctrlPointsWinRange[pid].y < 0) ctrlPointsWinRange[pid].y = 0; 

    //       set window (template/pattern) image and save to file
    ctrlPointsImage[pid] = testImg(ctrlPointsWinRange[pid]); 
    char templateFilename[1000];
    SPRINTF(templateFilename, "template\\HPoint_%03d.JPG", i + 1); 
    cv::imwrite(templateFilename, ctrlPointsImage[pid]); 

	// set template size
	H_templateHeight = templateHeight;
	H_templateWidth = templateWidth;
  }// End of picking HPoints

  float h_dy;
  float H_length_y = 0;
  H_length_y = ctrlPointsPosition[num_HPoint - 1 + itr_HPoint].y - ctrlPointsPosition[itr_HPoint].y;
  h_dy = H_length_y / num_HPoint;
  for (int i = 1; i < num_HPoint - 1; i++)
  {
	  int pid = i + itr_HPoint;
	  int templateHeight, templateWidth;
	  templateHeight = H_templateHeight;
	  templateWidth = H_templateWidth;

	  ctrlPointsPosition[pid].x = ctrlPointsPosition[itr_HPoint].x;
	  ctrlPointsPosition[pid].y = ctrlPointsPosition[itr_HPoint].y + (h_dy * i);
	  

	  //       set window (template/pattern) left-upper position and size.
	  ctrlPointsWinRange[pid].height = templateHeight; 
	  ctrlPointsWinRange[pid].width  = templateWidth; 
	  ctrlPointsWinRange[pid].x = (int) (ctrlPointsPosition[pid].x - templateWidth / 2.0 + 0.5); 
	  if (ctrlPointsWinRange[pid].x < 0) ctrlPointsWinRange[pid].x = 0; 
	  ctrlPointsWinRange[pid].y = (int) (ctrlPointsPosition[pid].y - templateHeight / 2.0 + 0.5); 
	  if (ctrlPointsWinRange[pid].y < 0) ctrlPointsWinRange[pid].y = 0; 

	  //       set window (template/pattern) image and save to file
	  ctrlPointsImage[pid] = testImg(ctrlPointsWinRange[pid]); 
	  char templateFilename[1000];
	  SPRINTF(templateFilename, "template\\HPoint_%03d.JPG", i + 1); 
	  cv::imwrite(templateFilename, ctrlPointsImage[pid]); 	  
  }

  // input diameter. unit: mm
  // convert to pixel
  float diameter  = param.diameter;
  diameter /= rpx;


  // calc. HCPoints
  // HCPoints' x is HPoints' x + diameter
  // HCPoints' y is HPoints' y
  for (int i = 0; i < num_HCPoint; i++)
  {
	  int pidH = i + itr_HPoint;
	  int pidHC = i + itr_HCPoint;

	  ctrlPointsPosition[pidHC].x = ctrlPointsPosition[pidH].x + diameter;
	  ctrlPointsPosition[pidHC].y = ctrlPointsPosition[pidH].y;

	  //       set window (template/pattern) left-upper position and size.
	  ctrlPointsWinRange[pidHC].height = H_templateHeight; 
	  ctrlPointsWinRange[pidHC].width  = H_templateWidth; 
	  ctrlPointsWinRange[pidHC].x = (int) (ctrlPointsPosition[pidHC].x - H_templateWidth / 2.0 + 0.5); 
	  if (ctrlPointsWinRange[pidHC].x < 0) ctrlPointsWinRange[pidHC].x = 0; 
	  ctrlPointsWinRange[pidHC].y = (int) (ctrlPointsPosition[pidHC].y - H_templateHeight / 2.0 + 0.5); 
	  if (ctrlPointsWinRange[pidHC].y < 0) ctrlPointsWinRange[pidHC].y = 0; 

	  //       set window (template/pattern) image and save to file
	  ctrlPointsImage[pidHC] = testImg(ctrlPointsWinRange[pidHC]); 
	  char templateFilename[1000];
	  SPRINTF(templateFilename, "template\\HCPoint_%03d.JPG", i + 1); 
	  cv::imwrite(templateFilename, ctrlPointsImage[pidHC]); 	    
  }

  //==calc. main loop==//
  int nPhoto = 0;

  std::vector<cv::Point2f> points;
  std::vector<cv::Point2f> vpoints;
  
  // point is central point of pattern
  // if pattern is 40Å~40, then point is (20, 20)
  points.push_back(cv::Point2f((float)(ctrlPointsWinRange[itr_HPoint].width / 2), (float)(ctrlPointsWinRange[itr_HPoint].height / 2)));
  vpoints.push_back(cv::Point2f((float)(ctrlPointsWinRange[itr_VPoint].width / 2), (float)(ctrlPointsWinRange[itr_VPoint].height / 2)));

  // get image name from file list
  // and add to fileList vector
  // std::vector<std::string> fileList;
  // std::ifstream iFile("FileList.txt");
  // for (std::string line; std::getline(iFile, line);)
  // {
  //  fileList.push_back(line);
  //  nPhoto++;
  //  std::cout << line << std::endl; // debug
  //}
  //
  // use for calcOpticalFlowPyrLK
  std::vector<cv::Point2f> newPoints;
  std::vector<unsigned char> status;
  std::vector<float> errors;

  // use for output
  std::vector<std::vector<cv::Point2f>> displacement;
  displacement.clear();
  displacement.resize(testCount);

  // debug output
  std::ofstream oErrorFile("optErr.txt");


  for (int i = 0; i < testCount; i++)
  {
	  displacement[i].clear();
	  displacement[i].resize(num_VPoint + num_HPoint + num_HCPoint);

	  // next image
	  cv::Mat newImg = cv::imread(testRectfFilenames[i]);
	  if (newImg.empty())
	  {
		  std::cout << "error!!" << std::endl;
	  }

	  cv::Mat traceImageCopy;
	  newImg.copyTo(traceImageCopy);
	  // trace VPoint
	  oErrorFile << "VPoint " << i << ":|| ";
	  for (int j = 0; j < num_VPoint; j++)
	  {
		  float dist = 0;
		  float dist_x = 0;
		  float dist_y = 0;
		  int pid = j + itr_VPoint;
		  
		  nextCtrlPointsImage[pid] = newImg(ctrlPointsWinRange[pid]);
		  
		  cv::calcOpticalFlowPyrLK(ctrlPointsImage[pid], nextCtrlPointsImage[pid], vpoints, newPoints, status, errors, cv::Size(ctrlPointsImage[pid].cols, ctrlPointsImage[pid].rows), 3, cv::TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 30, 0.001), 0, 0.00001);
		  oErrorFile << errors[0] << " ";
		  //cv::Mat tempNextCtrlPtImg;
		  
		  cv::Point CP;
		  CP.x = ctrlPointsWinRange[pid].x + newPoints[0].x;
		  CP.y = ctrlPointsWinRange[pid].y + newPoints[0].y;
		  cv::circle(traceImageCopy, CP, 1, cv::Scalar(i, j * 5, 100), 3, 8, 0);
		  cv::Rect Retangle;
		  Retangle.x = CP.x - 5;
		  Retangle.y = CP.y - 5;
		  Retangle.width = 10;
		  Retangle.height = 10;
		  //cv::rectangle(traceImageCopy, Retangle, cv::Scalar(i, j, 1), 3);
		  //cv::imwrite("Test.jpg", traceImageCopy);

		  int count = 0;

		  cv::Point2f a, b;
		  a.x = newPoints[count].x - 5;
		  a.y = newPoints[count].y - 5;
		  b.x = newPoints[count].x + 5;
		  b.y = newPoints[count].y + 5;

		  cv::Rect range(a, b);

		  char outName[256];
		  sprintf_s(outName, "ctrlPoints\\vpoint_%d_%d.jpg", i, j);
		  cv::rectangle(newImg, range, cv::Scalar(100, 100, 255));
		  cv::imwrite(outName, newImg);

		  
#ifdef _DEBUG
		  for (std::vector<cv::Point2f>::iterator itk = newPoints.begin(); itk != newPoints.end(); ++itk)
		  {
			  cv::line(nextCtrlPointsImage[pid], vpoints[count], newPoints[count], cv::Scalar(0, 0, 255));
			  std::cout << vpoints[count] << " Å® " << newPoints[count] << std::endl;
			  std::cout << "dist = " << sqrt(powf((newPoints[count].x - vpoints[count].x), 2) + powf((newPoints[count].y - vpoints[count].y), 2)) << std::endl;
			  dist += sqrt(powf((newPoints[count].x - points[count].x), 2) + powf((newPoints[count].y - vpoints[count].y), 2));
			  dist_x += (newPoints[count].x - vpoints[count].x);
			  dist_y += (newPoints[count].y - vpoints[count].y);
			  count++;
		  }
		  dist /= (float)count;
		  dist_x /= (float)count;
		  dist_y /= (float)count;  
		  std::cout << "count = " << count << std::endl;
		  cv::imshow("opt", nextCtrlPointsImage[pid]);
		  cv::waitKey();
		  cv::destroyWindow("opt");
#else
		  dist_x += (newPoints[count].x - vpoints[count].x);
		  dist_y += (newPoints[count].y - vpoints[count].y);

#endif
		  displacement[i][j] = cv::Point2f(dist_x, dist_y);
	  }
	  oErrorFile << std::endl;

	  oErrorFile << "HPoint " << i << ":|| ";
	  // trace HPoint
	  for (int j = 0; j < num_HPoint; j++)
	  {
		  float dist = 0;
		  float dist_x = 0;
		  float dist_y = 0;
		  int pid = j + itr_HPoint;

		  nextCtrlPointsImage[pid] = newImg(ctrlPointsWinRange[pid]);

		  cv::calcOpticalFlowPyrLK(ctrlPointsImage[pid], nextCtrlPointsImage[pid], points, newPoints, status, errors, cv::Size(ctrlPointsImage[pid].cols, ctrlPointsImage[pid].rows), 3, cv::TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 30, 0.001), 0, 0.00001);
		  oErrorFile << errors[0] << " ";

		  cv::Point CP;
		  CP.x = ctrlPointsWinRange[pid].x + newPoints[0].x;
		  CP.y = ctrlPointsWinRange[pid].y + newPoints[0].y;
		  cv::circle(traceImageCopy, CP, 1, cv::Scalar(i, j * 5, 170), 3, 8, 0);
		  cv::Rect Retangle;
		  Retangle.x = CP.x - 5;
		  Retangle.y = CP.y - 5;
		  Retangle.width = 10;
		  Retangle.height = 10;
		  //cv::rectangle(traceImageCopy, Retangle, cv::Scalar(i, j, 1), 3);

		  int count = 0;

		  cv::Point2f a, b;
		  a.x = newPoints[count].x - 5;
		  a.y = newPoints[count].y - 5;
		  b.x = newPoints[count].x + 5;
		  b.y = newPoints[count].y + 5;

		  cv::Rect range(a, b);

		  char outName[256];
		  sprintf_s(outName, "ctrlPoints\\hpoint_%d_%d.jpg", i, j);
		  cv::rectangle(newImg, range, cv::Scalar(100, 100, 255));
		  cv::imwrite(outName, newImg);
#ifdef _DEBUG
		  for (std::vector<cv::Point2f>::iterator itk = newPoints.begin(); itk != newPoints.end(); ++itk)
		  {
			  cv::line(nextCtrlPointsImage[pid], points[count], newPoints[count], cv::Scalar(0, 0, 255));
			  std::cout << points[count] << " Å® " << newPoints[count] << std::endl;
			  std::cout << "dist = " << sqrt(powf((newPoints[count].x - points[count].x), 2) + powf((newPoints[count].y - points[count].y), 2)) << std::endl;
			  dist += sqrt(powf((newPoints[count].x - points[count].x), 2) + powf((newPoints[count].y - points[count].y), 2));
			  dist_x += (newPoints[count].x - points[count].x);
			  dist_y += (newPoints[count].y - points[count].y);
			  count++;
		  }
		  dist /= (float)count;
		  dist_x /= (float)count;
		  dist_y /= (float)count;
		  std::cout << "count = " << count << std::endl;
		  cv::imshow("opt", nextCtrlPointsImage[pid]);
		  cv::waitKey();
		  cv::destroyWindow("opt");
#else
		  dist_x += (newPoints[count].x - points[count].x);
		  dist_y += (newPoints[count].y - points[count].y);

#endif
		  displacement[i][j] = cv::Point2f(dist_x, dist_y);
	  }
	  oErrorFile << std::endl;

	  oErrorFile << "HCPoint " << i << ":|| ";
	  // trace HCPoint
	  for (int j = 0; j < num_HCPoint; j++)
	  {
		  float dist = 0;
		  float dist_x = 0;
		  float dist_y = 0;
		  int pid = j + itr_HCPoint;

		  nextCtrlPointsImage[pid] = newImg(ctrlPointsWinRange[pid]);

		  cv::calcOpticalFlowPyrLK(ctrlPointsImage[pid], nextCtrlPointsImage[pid], points, newPoints, status, errors, cv::Size(ctrlPointsImage[pid].cols, ctrlPointsImage[pid].rows), 3, cv::TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 30, 0.001), 0, 0.00001);
		  oErrorFile << errors[0] << " ";

		  cv::Point CP;
		  CP.x = ctrlPointsWinRange[pid].x + newPoints[0].x;
		  CP.y = ctrlPointsWinRange[pid].y + newPoints[0].y;
		  cv::circle(traceImageCopy, CP, 1, cv::Scalar(i, j * 5, 240), 3, 8, 0);
		  cv::Rect Retangle;
		  Retangle.x = CP.x - 5;
		  Retangle.y = CP.y - 5;
		  Retangle.width = 10;
		  Retangle.height = 10;
		  //cv::rectangle(traceImageCopy, Retangle, cv::Scalar(i, j, 1), 3);

		  int count = 0;
		  cv::Point2f a, b;
		  a.x = newPoints[count].x - 5;
		  a.y = newPoints[count].y - 5;
		  b.x = newPoints[count].x + 5;
		  b.y = newPoints[count].y + 5;

		  cv::Rect range(a, b);

		  char outName[256];
		  sprintf_s(outName, "ctrlPoints\\hcpoint_%d_%d.jpg", i, j);
		  cv::rectangle(newImg, range, cv::Scalar(100, 100, 255));
		  cv::imwrite(outName, newImg);
#ifdef _DEBUG
		  for (std::vector<cv::Point2f>::iterator itk = newPoints.begin(); itk != newPoints.end(); ++itk)
		  {
			  cv::line(nextCtrlPointsImage[pid], points[count], newPoints[count], cv::Scalar(0, 0, 255));
			  std::cout << points[count] << " Å® " << newPoints[count] << std::endl;
			  std::cout << "dist = " << sqrt(powf((newPoints[count].x - points[count].x), 2) + powf((newPoints[count].y - points[count].y), 2)) << std::endl;
			  dist += sqrt(powf((newPoints[count].x - points[count].x), 2) + powf((newPoints[count].y - points[count].y), 2));
			  dist_x += (newPoints[count].x - points[count].x);
			  dist_y += (newPoints[count].y - points[count].y);
			  count++;
		  }
		  dist /= (float)count;
		  dist_x /= (float)count;
		  dist_y /= (float)count;
		  std::cout << "count = " << count << std::endl;
		  cv::imshow("opt", nextCtrlPointsImage[pid]);
		  cv::waitKey();
		  cv::destroyWindow("opt");
#else
		  dist_x += (newPoints[count].x - points[count].x);
		  dist_y += (newPoints[count].y - points[count].y);

#endif
		  displacement[i][j] = cv::Point2f(dist_x, dist_y);
	  }
	  oErrorFile << std::endl;

	  std::string testTraceFileName = "trace\\trace_rectf_" + testImageFilenames[i];
	  cv::imwrite(testTraceFileName, traceImageCopy);

  }
  oErrorFile.close();




  //-----write file-----//
  std::ofstream oFile("data.csv");


  //** header **//
  oFile << "photo,,";
  for (int i = 0; i < num_VPoint; i++)
  {
	  oFile <<"vPoint" << i << " x," << "vPoint" << i << " y," << "vPoint" << i << " disp x," << "vPoint "<< i << "disp y,,";
  }
  for (int i = 0; i < num_HPoint; i++)
  {
	  oFile <<"hPoint" << i << " x," << "hPoint" << i << " y," << "hPoint" << i << " disp x," << "hPoint "<< i << "disp y,,";
  }
  for (int i = 0; i < num_HCPoint; i++)
  {
	  oFile <<"hcPoint" << i << " x," << "hcPoint" << i << " y," << "hcPoint" << i << " disp x," << "hcPoint "<< i << "disp y,,";
  }
  oFile << "Total = " << num_VPoint + num_HPoint + num_HCPoint << ",,";
  oFile << std::endl;
  

  // data
  for (int i = 0; i < testCount; i++)
  {
	  oFile << i << ",,";

	  // VPoint
	  for (int j = 0; j < num_VPoint; j++)
	  {
		  int pid = j + itr_VPoint;
		  oFile << ctrlPointsPosition[pid].x << "," << ctrlPointsPosition[pid].y << "," << displacement[i][j].x <<  "," << displacement[i][j].y << ",,";
	  }

	  // HPoint
	  for (int j = 0; j < num_HPoint; j++)
	  {
		  int pid = j + itr_HPoint;
		  oFile << ctrlPointsPosition[pid].x << "," << ctrlPointsPosition[pid].y << "," << displacement[i][j].x <<  "," << displacement[i][j].y << ",,";
	  }

	  // HCPoint
	  for (int j = 0; j < num_HCPoint; j++)
	  {
		  int pid = j + itr_HCPoint;
		  oFile << ctrlPointsPosition[pid].x << "," << ctrlPointsPosition[pid].y << "," << displacement[i][j].x <<  "," << displacement[i][j].y << ",,";
	  }
	  oFile << std::endl;
  }
  oFile.close(); // end of write file
  fclose(fp);
  return 0;
}



