#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cstring>
#include <fstream>
#include "CSRT.hpp"

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

    cv::TrackerCSRT::Params parameters = cv::TrackerCSRT::Params();

    // parameters.use_channel_weights = false;
    parameters.use_segmentation = false;
    parameters.use_hog = true;
    parameters.use_color_names = false;
    parameters.use_gray = true;
    parameters.use_rgb = false;
    // parameters.window_function = "hann";
    // parameters.kaiser_alpha = 3.75f;
    // parameters.cheb_attenuation = 45;
    // parameters.padding = 3.0f;
    parameters.template_size = 100; // 200 
    parameters.gsl_sigma = 1.0f; // for createing gussian response 
    // parameters.hog_orientations = 9;
    // parameters.hog_clip = 0.2f;
    parameters.num_hog_channels_used = 12; // need to som e change in csrtUTILS file in 2 place 
    parameters.filter_lr = 0.02f; // 0.02f this is \etha in ALGO2 step 10
    parameters.weights_lr = 0.02f; // 0.02f this is \etha in ALGO2 step 11, it and perv line need to ba same  it effect on loss target
    parameters.admm_iterations = 8; //4 number of step for algoritm 1
    //DSST MODULE---------------------------------------------------------------
    parameters.number_of_scales = 50; // 33 this effect on changeing scale of window 
    // parameters.scale_sigma_factor = 0.250f;
    parameters.scale_model_max_area =1024.0f;
    parameters.scale_lr = 0.025f; //0.025f 
    parameters.scale_step = 1.020f;
    // SEGMENTION MODULE parameters
    // parameters.histogram_bins = 16;
    // parameters.background_ratio = 2;
    // parameters.histogram_lr = 0.04f;


    parameters.psr_threshold = 0.015f;
    
    Ptr<TrackerCSRT> tracker = TrackerCSRT::create(parameters); //#TODO:




    // set input video
    std::string video = argv[1];
    VideoCapture cap(video,cv::CAP_FFMPEG); //   cv::CAP_GSTREAMER cv::CAP_FFMPEG
    // and read first frame
    Mat frame;
    cap >> frame;
    
    Rect roi;

    std::string groundtruthPath = argv[2];
    std::ifstream gtIfstream(groundtruthPath.c_str());
    std::string gtLine;
    getline(gtIfstream, gtLine);
    // gtIfstream.close();

    // parse the line by elements
    std::stringstream gtStream(gtLine);
    std::string element;
    std::vector<int> elements;

    while (std::getline(gtStream, element, ','))
    {
        elements.push_back(cvRound(std::atof(element.c_str())));
    }

    // std::cout<<elements.at(0)<<std::endl;
    std::cout<<elements.size()<<std::endl;
    Rect roi_gt;
    if (elements.size() == 4) {
        // ground-truth is rectangle
        roi_gt = cv::Rect(elements[0], elements[1],
                    elements[2]-elements[0], elements[3]-elements[1]);
        roi_gt = cv::Rect(elements[0], elements[1],
                    elements[2], elements[3]);

    }    

    // roi = selectROI("tracker", frame, true, false);
    //quit if ROI was not selected
    if (roi_gt.width == 0 || roi_gt.height == 0)
        return 0;

    // initialize the tracker
    int64 t1 = cv::getTickCount();
    tracker->init(frame, roi_gt);
    int64 t2 = cv::getTickCount();
    int64 tick_counter = t2 - t1;

    rectangle(frame, roi_gt, Scalar(0, 255, 0), 1, 1);
    imshow("tracker", frame);
    waitKey(0);


    // do the tracking
    // tick_counter = 0; 
    printf("Start tick_counter the tracking process, press ESC to quit.\n");
    int frame_idx = 1; // 1
    for (;;) {
        // get frame from the video
        if (frame_idx > 1)
            cap >> frame;

        // stop the program if no more images
        if (frame.rows == 0 || frame.cols == 0)
            break;
        getline(gtIfstream, gtLine);
         // parse the line by elements
        std::stringstream gtStream(gtLine);
        std::string element;
        std::vector<int> elements;

        while (std::getline(gtStream, element, ','))
        {
            elements.push_back(cvRound(std::atof(element.c_str())));
        }

        // std::cout<<elements.at(0)<<std::endl;
        // Rect roi_gt;
        if (elements.size() == 4) {
            // ground-truth is rectangle
            roi_gt = cv::Rect(elements[0], elements[1],
                        elements[2]-elements[0], elements[3]-elements[1]);
            roi_gt = cv::Rect(elements[0], elements[1],
                    elements[2], elements[3]);

        }    
        


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
        rectangle(frame, roi_gt, Scalar(0, 255, 0), 1, 1);
        rectangle(frame, roi, Scalar(255, 0, 0), 2, 1);
        imshow("tracker", frame);

        //quit on ESC button
        if (waitKey(1) == 27)break;
    }
    gtIfstream.close();
    cout << "Elapsed sec: " << static_cast<double>(tick_counter) / cv::getTickFrequency() << endl;
    cout << "FPS: " << ((double)(frame_idx)) / (static_cast<double>(tick_counter) / cv::getTickFrequency()) << endl;
    
   return 0;
}




