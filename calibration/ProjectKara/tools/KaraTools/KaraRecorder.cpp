#include <iostream>
#include <iomanip>
#include <memory>
#include <thread>
#include <mutex>
#include <map>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "Utilities.hpp"

int g_Width = 640;
int g_Height = 480;
int g_RequestedFPS = 120;
int g_MaxCams = 8;
uint64_t g_FrameCtr = 0;
bool g_isWriting = false;	
bool g_isPaused = false;
std::string g_OutDir = ".";

std::vector<std::shared_ptr<cv::VideoCapture>> g_ValidCams;
cv::Mat g_PreviewFrame;
cv::Mat g_CollageFrame;
std::mutex g_Mutex;
bool m_isStopCapture = false;
std::vector<int> g_FPSes;
int g_CapFormat = CV_FOURCC('M', 'J', 'P', 'G');
int g_WriteFormat = CV_FOURCC('X', '2', '6', '4');
// int g_WriteFormat = CV_FOURCC('M', 'J', 'P', 'G');
cv::VideoWriter g_VideoWriter;
int g_FrameWriteCtr = 0;

int g_nCols = 1;
int g_nRows = 1;

std::vector<float> g_FileWETime;

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
				g_Mutex.lock();
				CurrentFrame.copyTo(g_CollageFrame(cv::Rect(Col * g_Width, Row * g_Height, CurrentFrame.cols, CurrentFrame.rows)));
				g_Mutex.unlock();
			}
		}
		auto Toc = Common::getCurrentEpochTime();

		g_Mutex.lock();
		g_FPSes[CamIdx] = std::floor(1000.0 / ((Toc - Tic) * 1e-3));
		g_Mutex.unlock();
	}
}

void WriteBufferToFiles(void)
{
	g_VideoWriter.release();
	std::cout << "[ INFO ]: Buffer written. Number of frames: " << g_FrameWriteCtr << std::endl;
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

		std::cout << "[ INFO ]: Camera " << camTryID << " exposure set to: " << CamCap->get(CV_CAP_PROP_EXPOSURE) << std::endl;

		g_ValidCams.push_back(CamCap);
	}
}

void StopRecording()
{
	std::cout << "Pausing file write." << std::endl;
	g_Mutex.lock();
	g_isWriting = false;
	g_Mutex.unlock();

	WriteBufferToFiles();	
}

int main(int argc, char *argv[])
{
	if (argc == 2)
	{
		g_OutDir = std::string(argv[1]);
	}

	init();

	if (g_ValidCams.size() == 0)
	{
		std::cout << "[ ERR ]: Unable to find any cameras. Exiting." << std::endl;
		return -1;
	}

	g_nCols = std::ceil(sqrt(float(g_ValidCams.size())));
	g_nRows = std::ceil(g_ValidCams.size() / float(g_nCols));
	std::cout << "Cols, Rows: " << g_nCols << ", " << g_nRows << std::endl;
	g_CollageFrame = cv::Mat(g_Height * g_nRows, g_Width * g_nCols, CV_8UC3); // Create a single big collage frame
	g_PreviewFrame = cv::Mat(g_Height * g_nRows, g_Width * g_nCols, CV_8UC3); // Create a single big preview frame

	// Start a thread for each camera
	std::vector<std::thread> CamThreads;
	g_FPSes = std::vector<int>(g_ValidCams.size(), 0);

	for (int i = 0; i < g_ValidCams.size(); ++i)
		CamThreads.push_back(std::thread(StartCaptureThread, i));

	// Display stuff
	while (true)
	{
		cv::Mat FrameCopy;
		g_Mutex.lock();
		{
			g_CollageFrame.copyTo(FrameCopy);
		}
		g_Mutex.unlock();
		FrameCopy.copyTo(g_PreviewFrame);

		if (g_isWriting)
		{
			auto Tic = Common::getCurrentEpochTime();
			{
				if(g_VideoWriter.isOpened())
				{
					g_VideoWriter.write(FrameCopy);
					g_FrameWriteCtr++;
				}
			}
			auto Toc = Common::getCurrentEpochTime();
			g_FileWETime.push_back(Toc - Tic);
			g_FrameCtr++;
		}

		std::stringstream ss;
		float ActualFPS = 0.0;
		for (auto F : g_FPSes)
		{
			g_Mutex.lock();
			{
				ActualFPS += F;
			}
			g_Mutex.unlock();
		}
		ss << "FPS: " << std::floor(ActualFPS / g_FPSes.size());

		cv::putText(g_PreviewFrame, ss.str(), cvPoint(30, 30),
					CV_FONT_HERSHEY_PLAIN, 1.2, cvScalar(0, 0, 250), 1, CV_AA);

		if (g_isPaused == false)
			cv::imshow("KaraRecorder - Preview", g_PreviewFrame);

		char k = char(cv::waitKey(1));
		if (k == 27)
		{
			std::cout << "[ INFO ]: Exiting." << std::endl;
			m_isStopCapture = true;
			WriteBufferToFiles();
			break;
		}
		if (k == 'r')
		{
			g_isWriting = !g_isWriting;
			if (g_isWriting)
			{
				std::cout << "Starting file write." << std::endl;
				g_FileWETime.clear();
				g_FrameWriteCtr = 0;

				std::stringstream ss;
				ss << g_OutDir << "/" << "Capture_" << Common::getCurrentEpochTime() << ".avi";
				g_VideoWriter.open(ss.str(), g_WriteFormat, g_RequestedFPS, cv::Size(g_nCols * g_Width, g_nRows * g_Height));
			}
			else
				StopRecording();
		}
		if (k == ' ')
		{
			g_isPaused = !g_isPaused;
			if (g_isPaused)
				std::cout << "Preview paused." << std::endl;
			else
				std::cout << "Preview unpaused." << std::endl;
		}
	}

	for (auto &t : CamThreads)
		t.join();

	return 0;
}