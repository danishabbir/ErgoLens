#include <iostream>
#include <iomanip>
#include <memory>
#include <thread>
#include <mutex>
#include <map>
#include <fstream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

#include "Utilities.hpp"

int g_Width = 640;
int g_Height = 480;
int g_RequestedFPS = 120;
int g_MaxCams = 8;
bool g_doCalibrate = false;
bool g_isPause = false;
std::string g_OutDir = ".";

std::vector<std::shared_ptr<cv::VideoCapture>> g_ValidCams;
std::mutex g_Mutex;
bool m_isStopCapture = false;
std::vector<int> g_FPSes;
int g_CapFormat = CV_FOURCC('M', 'J', 'P', 'G');

int g_nCols = 1;
int g_nRows = 1;

cv::Size g_ChessboardPatternSize = cv::Size(8, 6); // Fixed, see base directory for a pattern
float g_ChessSquareSize = 11.8f;				   // Units: mm.

cv::Mat g_CollageFrame;								   // Collage of  raw frames
cv::Mat g_PreviewFrame;								   // Collage of preview frames + additional info
std::vector<cv::Mat> g_CalibFrames;					   // Vector of frames used for calibration
typedef std::vector<cv::Point2f> CalibPattern;		   // calib pattern == vector of points
std::vector<std::vector<CalibPattern>> g_CalibCorners; // Frames x cameras x calib pattern
bool g_isAddToCalibStack = false;
std::stringstream g_CalibFileSS;

#define MIN_CALIB_VIEWS 2

void StartCaptureThread(unsigned int CamIdx)
{
	std::cout << "[ INFO ]: Started thread for camera " << CamIdx << "." << std::endl;

	while (m_isStopCapture == false)
	{
		auto Tic = Common::getCurrentEpochTime();
		{
			cv::Mat CurrentFrame;
			(*g_ValidCams[CamIdx]) >> CurrentFrame;
			{
				int Row = CamIdx / g_nCols; // Integer division
				int Col = CamIdx % g_nCols;
				cv::Mat TmpFrame;
				g_Mutex.lock();
				CurrentFrame.copyTo(TmpFrame);
				g_Mutex.unlock();

				TmpFrame.copyTo(g_CollageFrame(cv::Rect(Col * g_Width, Row * g_Height, CurrentFrame.cols, CurrentFrame.rows)));

				std::vector<cv::Point2f> Corners;
				bool isChessboardFound = cv::findChessboardCorners(TmpFrame, g_ChessboardPatternSize, Corners, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE | CV_CALIB_CB_FAST_CHECK);
				cv::drawChessboardCorners(TmpFrame, g_ChessboardPatternSize, Corners, isChessboardFound);

				TmpFrame.copyTo(g_PreviewFrame(cv::Rect(Col * g_Width, Row * g_Height, CurrentFrame.cols, CurrentFrame.rows)));
			}
		}
		auto Toc = Common::getCurrentEpochTime();

		g_Mutex.lock();
		g_FPSes[CamIdx] = std::floor(1000.0 / ((Toc - Tic) * 1e-3));
		g_Mutex.unlock();
	}
}

void WriteCalibFiles(void)
{
	std::cout << "[ INFO ]: Writing calibration data..." << std::endl;
	int Ctr = 0;
	for (const auto &Frame : g_CalibFrames)
	{
		std::stringstream ss;
		ss << g_OutDir << "/"
		   << "calib_" << std::setfill('0') << std::setw(8) << Ctr << ".png";
		std::cout << "[ INFO ]: Writing " << ss.str() << std::endl;
		cv::imwrite(ss.str(), Frame);
		Ctr++;
	}

	std::string FName = g_OutDir + "/calib.params";
	std::fstream CalibFile(FName, std::fstream::in | std::fstream::out);
	if (CalibFile.is_open())
	{
		std::cout << "[ INFO ]: Writing " << FName << std::endl;
		CalibFile << g_CalibFileSS.str();
		CalibFile.close();
	}
	else
		std::cout << "[ WARN ]: Unable to open file " << FName << std::endl;

	std::cout << "[ INFO ]: All done." << std::endl;
}

void init()
{
	for (int camTryID = 0; camTryID < g_MaxCams; ++camTryID)
	{
#ifdef _WIN32
		std::shared_ptr<cv::VideoCapture> CamCap = std::make_shared<cv::VideoCapture>(CV_CAP_DSHOW + camTryID);
#else
		std::shared_ptr<cv::VideoCapture> CamCap = std::make_shared<cv::VideoCapture>(camTryID);
#endif
		if (CamCap->isOpened() == false)
		{
			std::cout << "[ WARN ]: Unable to open camera with device ID " << camTryID << ". Trying next." << std::endl;
			continue;
		}
		else
			std::cout << "[ INFO ]: Successfully opened camera with device ID " << camTryID << "." << std::endl;

		// Set properties
		int PropSetSuccess = true;
		PropSetSuccess = CamCap->set(CV_CAP_PROP_FOURCC, g_CapFormat);
		if (PropSetSuccess == false)
		{
			std::cout << "[ WARN ]: Unable to set camera encoding MJPG for device ID " << camTryID << "." << std::endl;
			continue;
		}

#ifdef _WIN32
		PropSetSuccess = PropSetSuccess & CamCap->set(CV_CAP_PROP_FRAME_WIDTH, double(g_Width));
		PropSetSuccess = PropSetSuccess & CamCap->set(CV_CAP_PROP_FRAME_WIDTH, double(g_Height));
		PropSetSuccess = PropSetSuccess & CamCap->set(CV_CAP_PROP_FPS, double(g_RequestedFPS));

		if (PropSetSuccess == false) // Failed to set properties. Not a supported camera.
		{
			std::cout << "[ WARN ]: Unable to set some properties for device ID " << camTryID << "." << std::endl;
			continue;
		}
#endif

		std::cout << "[ INFO ]: Camera " << camTryID << " FOURCC set to: " << CamCap->get(CV_CAP_PROP_FOURCC) << " vs. " << g_CapFormat << std::endl;
		std::cout << "[ INFO ]: Camera " << camTryID << " width set to: " << CamCap->get(CV_CAP_PROP_FRAME_WIDTH) << std::endl;
		std::cout << "[ INFO ]: Camera " << camTryID << " height set to: " << CamCap->get(CV_CAP_PROP_FRAME_HEIGHT) << std::endl;
		std::cout << "[ INFO ]: Camera " << camTryID << " frame rate set to: " << CamCap->get(CV_CAP_PROP_FPS) << std::endl;

		g_ValidCams.push_back(CamCap);
	}
}

bool validateDetectCalibFrame(const cv::Mat &CollageFrame)
{
	std::vector<CalibPattern> CurrentFrameCalibPoints;
	bool isChessboardFoundAllCams = true;
	for (int i = 0; i < g_ValidCams.size(); ++i)
	{
		int Row = i / g_nCols; // Integer division
		int Col = i % g_nCols;

		CalibPattern Corners;
		cv::Mat TmpFrame = CollageFrame(cv::Rect(Col * g_Width, Row * g_Height, g_Width, g_Height));
		isChessboardFoundAllCams &= cv::findChessboardCorners(TmpFrame, g_ChessboardPatternSize, Corners, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE | CV_CALIB_CB_FAST_CHECK);

		if (isChessboardFoundAllCams == false || Corners.size() != g_ChessboardPatternSize.height * g_ChessboardPatternSize.width)
		{
			std::cout << "Current frame is not suitable for calibration. CamIdx: " << i << ", isChessboardFoundAllCams: " << isChessboardFoundAllCams << ", Corners size: " << Corners.size() << std::endl;
			return false;
		}

		CurrentFrameCalibPoints.push_back(Corners);
	}

	// Sanity checks
	if (CurrentFrameCalibPoints.size() != g_ValidCams.size())
	{
		std::cout << "Current frame is not suitable for calibration. CurrentFrameCalibPoints.size(): " << CurrentFrameCalibPoints.size() << std::endl;
		return false;
	}

	g_CalibCorners.push_back(CurrentFrameCalibPoints);
	std::cout << "Added current frame to calibration stack. Size: " << g_CalibCorners.size() << std::endl;

	return true;
}

void calcBoardCornerPositions(cv::Size boardSize, float squareSize, std::vector<cv::Point3f> &corners)
{
	corners.clear();

	for (int i = 0; i < boardSize.height; ++i)
	{
		for (int j = 0; j < boardSize.width; ++j)
			corners.push_back(cv::Point3f(float(j * squareSize), float(i * squareSize), 0));
	}
}

void calibrateAndWrite(void)
{
	if (g_CalibCorners.size() < MIN_CALIB_VIEWS)
	{
		std::cout << "[ WARN ]: Not enough frames in the calibration stack. Add more at different angles. MIN_CALIB_VIEWS = " << MIN_CALIB_VIEWS << std::endl;
		return;
	}

	std::cout << "Starting calibration based on calibration stack." << std::endl;

	// First, let's calibrate only for intrinsics for each camera separately by looking only at the corresponding camera view
	for (int CamIdx = 0; CamIdx < g_ValidCams.size(); ++CamIdx) // Pick a camera
	{
		std::vector<CalibPattern> SameCamAllFrameCorners;
		for (int i = 0; i < g_CalibCorners.size(); ++i) // Concatenate over frames for that camera
			SameCamAllFrameCorners.push_back(g_CalibCorners[i][CamIdx]);

		std::vector<std::vector<cv::Point3f>> ObjectPoints(1);
		calcBoardCornerPositions(g_ChessboardPatternSize, g_ChessSquareSize, ObjectPoints[0]);
		ObjectPoints.resize(SameCamAllFrameCorners.size(), ObjectPoints[0]); // Copy the same object coords for all points

		// Initialize matrices
		cv::Mat IntrinsicsMat = cv::Mat::eye(3, 3, CV_64F);
		cv::Mat DistortionCoeffs = cv::Mat::zeros(8, 1, CV_64F);
		std::vector<cv::Mat> rvecs, tvecs;

		double rms = cv::calibrateCamera(ObjectPoints, SameCamAllFrameCorners, cv::Size(g_Width, g_Height), IntrinsicsMat, DistortionCoeffs, rvecs, tvecs, CV_CALIB_FIX_K4 | CV_CALIB_FIX_K5);

		g_CalibFileSS << "Calibrated camera " << CamIdx << std::endl;
		g_CalibFileSS << "RMS: " << rms << std::endl;
		g_CalibFileSS << "Intrinsics: " << IntrinsicsMat << std::endl;
		g_CalibFileSS << "Distortion coeff: " << DistortionCoeffs << std::endl
					  << std::endl;
	}

	std::cout << g_CalibFileSS.str() << std::endl;

	// Second, get extrinsics. There are multiple ways depending on accuracy needed. Here we go for (presumably) the least accurate
	for (int i = 0; i < g_CalibFrames.size(); ++i) // For each frame, estimate camera extrinsics
	{
		std::vector<CalibPattern> AllCamsForSameFrame = g_CalibCorners[i]; // Get the corners for all cams in current frame

		std::vector<std::vector<cv::Point3f>> ObjectPoints(1);
		calcBoardCornerPositions(g_ChessboardPatternSize, g_ChessSquareSize, ObjectPoints[0]);
		ObjectPoints.resize(AllCamsForSameFrame.size(), ObjectPoints[0]); // Copy the same object coords for all points

		// Initialize matrices
		cv::Mat IntrinsicsMat = cv::Mat::eye(3, 3, CV_64F);		 // Same for all cameras, obviously not correct
		cv::Mat DistortionCoeffs = cv::Mat::zeros(8, 1, CV_64F); // Same for all cameras, obviously not correct
		std::vector<cv::Mat> RotVecs, TransVec;

		double rms = cv::calibrateCamera(ObjectPoints, AllCamsForSameFrame, cv::Size(g_Width, g_Height), IntrinsicsMat, DistortionCoeffs, RotVecs, TransVec, CV_CALIB_FIX_K4 | CV_CALIB_FIX_K5);

		std::cout << "Calibration stack frame: " << i << std::endl;
		std::cout << "RMS: " << rms << std::endl;
		std::cout << "Rotations: " << rms << std::endl;

		g_CalibFileSS << "Calibration stack frame: " << i << std::endl;
		g_CalibFileSS << "RMS: " << rms << std::endl;g_CalibFileSS << "Calibration stack frame: " << i << std::endl;
		g_CalibFileSS << "RMS: " << rms << std::endl;
		g_CalibFileSS << "Rotations: " << rms << std::endl;

		g_CalibFileSS << "Rotations: " << rms << std::endl;


		for (const auto &rvec : RotVecs)
		{
			cv::Mat RMat;
			cv::Rodrigues(rvec, RMat);
			std::cout << RMat << std::endl;
			g_CalibFileSS << RMat << std::endl;
		}
		std::cout << "Translations: " << rms << std::endl;
		g_CalibFileSS << "Translations: " << rms << std::endl; 
		for (const auto &tvec : TransVec) {
			std::cout << tvec << std::endl;
			g_CalibFileSS << tvec << std::endl;
		}

		std::cout << std::endl
				  << std::endl;
	}

	WriteCalibFiles();

	// Reset all
	g_CalibFrames.clear();
	g_CalibCorners.clear(); // Reset calibration stack state
	g_CalibFileSS.str("");
}

int main(int argc, char *argv[])
{
	if (argc == 2)
		g_OutDir = std::string(argv[1]);

	init();

	if (g_ValidCams.size() == 0)
	{
		std::cout << "[ ERR ]: Unable to find any cameras. Exiting." << std::endl;
		return -1;
	}

	g_nCols = std::ceil(sqrt(float(g_ValidCams.size())));
	g_nRows = std::ceil(g_ValidCams.size() / float(g_nCols));
	//std::cout << "Cols, Rows: " << g_nCols << ", " << g_nRows << std::endl;
	g_CollageFrame = cv::Mat(g_Height * g_nRows, g_Width * g_nCols, CV_8UC3); // Create a single big frame
	g_PreviewFrame = cv::Mat(g_Height * g_nRows, g_Width * g_nCols, CV_8UC3); // Create a single big preview frame

	// Start a thread for each camera
	std::vector<std::thread> CamThreads;
	g_FPSes = std::vector<int>(g_ValidCams.size(), 0);

	for (int i = 0; i < g_ValidCams.size(); ++i)
		CamThreads.push_back(std::thread(StartCaptureThread, i));

	// Display stuff
	while (true)
	{
		if (g_doCalibrate)
		{
			auto Tic = Common::getCurrentEpochTime();
			{
				calibrateAndWrite();
			}
			auto Toc = Common::getCurrentEpochTime();
		}

		std::stringstream ss;
		float ActualFPS = 0.0;
		for (auto F : g_FPSes)
		{
			g_Mutex.lock();
			ActualFPS += F;
			g_Mutex.unlock();
		}
		ss << "FPS: " << std::floor(ActualFPS / g_FPSes.size());

		cv::putText(g_PreviewFrame, ss.str(), cvPoint(30, 30),
					CV_FONT_HERSHEY_PLAIN, 1.2, cvScalar(0, 0, 250), 1, CV_AA);

		if (g_isPause == false)
			cv::imshow("KaraCalibrator - Preview", g_PreviewFrame);

		char k = char(cv::waitKey(1));
		if (k == 27)
		{
			std::cout << "[ INFO ]: Exiting." << std::endl;
			m_isStopCapture = true;
			WriteCalibFiles();
			break;
		}
		if (k == 'c')
		{
			calibrateAndWrite();
		}
		if (k == ' ')
		{
			g_Mutex.lock();
			bool isValid = validateDetectCalibFrame(g_CollageFrame);
			if (isValid)
			{
				cv::Mat CollageCopy;
				g_CollageFrame.copyTo(CollageCopy);
				g_CalibFrames.push_back(CollageCopy);
			}
			g_Mutex.unlock();
		}
		if (k == 'p')
		{
			g_isPause = !g_isPause;
			if (g_isPause)
				std::cout << "Pausing preview." << std::endl;
			else
				std::cout << "Unpausing preview." << std::endl;
		}
	}

	for (auto &t : CamThreads)
		t.join();

	return 0;
}
