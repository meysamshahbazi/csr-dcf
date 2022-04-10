#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cstring>
#include <fstream>
#include "src/csrt.hpp"

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    // show help
    if (argc<2) {
        cout <<
            " Usage: example_tracking_csrt <video_name>\n"
            " examples:\n"
            " example_tracking_csrt Bolt/img/%04.jpg\n"
            " example_tracking_csrt Bolt/img/%04.jpg Bolt/grouondtruth.txt\n"
            " example_tracking_csrt faceocc2.webm\n"
            << endl;
        return 0;
    }



    // create the tracker
    CsrtParams params;

    CSRT csrt {params};
    cv::TrackerCSRT::Params parameters = cv::TrackerCSRT::Params();

    // parameters.
    
    Ptr<TrackerCSRT> tracker = TrackerCSRT::create(parameters); //#TODO:




    // set input video
    std::string video = argv[1];
    VideoCapture cap(video,cv::CAP_FFMPEG); //   cv::CAP_GSTREAMER cv::CAP_FFMPEG
    // and read first frame
    Mat frame;
    cap >> frame;

    // target bounding box
    Rect roi;
   
    roi = selectROI("tracker", frame, true, false);
    

    //quit if ROI was not selected
    if (roi.width == 0 || roi.height == 0)
        return 0;

    // initialize the tracker
    int64 t1 = cv::getTickCount();
    tracker->init(frame, roi);
    csrt.init(frame,roi);
    int64 t2 = cv::getTickCount();
    int64 tick_counter = t2 - t1;

    // do the tracking
    // tick_counter = 0; 
    printf("Start tick_counter the tracking process, press ESC to quit.\n");
    int frame_idx = 1; // 1
    for (;;) {
        // get frame from the video
        cap >> frame;

        // stop the program if no more images
        if (frame.rows == 0 || frame.cols == 0)
            break;

        // update the tracking result
        t1 = cv::getTickCount();
        bool isfound = tracker->update(frame, roi);
        t2 = cv::getTickCount();
        tick_counter += t2 - t1;
        frame_idx++;

        if (!isfound) {
            cout << "The target has been lost...\n";
            cout << "Elapsed sec: " << static_cast<double>(tick_counter) / cv::getTickFrequency() << endl;
            cout << "FPS: " << ((double)(frame_idx)) / (static_cast<double>(tick_counter) / cv::getTickFrequency()) << endl;
            // waitKey(0);
            return 0;
        }

        // draw the tracked object and show the image
        rectangle(frame, roi, Scalar(255, 0, 0), 2, 1);
        imshow("tracker", frame);

        //quit on ESC button
        if (waitKey(1) == 27)break;
    }

    cout << "Elapsed sec: " << static_cast<double>(tick_counter) / cv::getTickFrequency() << endl;
    cout << "FPS: " << ((double)(frame_idx)) / (static_cast<double>(tick_counter) / cv::getTickFrequency()) << endl;
    
   return 0;
}


