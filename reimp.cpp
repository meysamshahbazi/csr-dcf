#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cstring>
#include <fstream>
#include <algorithm>

using namespace cv;
using namespace std;


float current_scale_factor;
Rect2f bounding_box;
Size2i image_size;
Size2f original_target_size;
Size2f template_size;
float rescale_ratio;
Size2i rescaled_template_size;

Point2f object_center



struct Params
{
    bool use_channel_weights = true;
    bool use_segmentation = true;
    bool use_hog = true;
    bool use_color_names = true;
    bool use_gray = true;
    bool use_rgb = false;
    std::string window_function = "hann"; //Window function: "hann", "cheb", "kaiser"
    float kaiser_alpha = 3.75f;
    float cheb_attenuation = 45;
    float padding = 3.0f;
    float template_size = 200;
    float gsl_sigma = 1.0f;
    float hog_orientations = 9;
    float hog_clip = 0.2f;
    int num_hog_channels_used = 18;
    float filter_lr = 0.02f;
    float weights_lr = 0.02f;
    int admm_iterations = 4;
    int number_of_scales = 33;
    float scale_sigma_factor = 0.250f;
    float scale_model_max_area = 512.0f;
    float scale_lr = 0.025f;
    float scale_step = 1.020f;
    int histogram_bins = 16;
    int background_ratio = 2;
    float histogram_lr = 0.04f;
    float psr_threshold = 0.035f;
}; 

Params params;




int cell_size;

float  padding =0; 



void init(InputArray image_, const Rect & boundingBox);
bool update(InputArray image_, Rect& boundingBox);

int main(int argc, char** argv)
{
    std::string video = argv[1];
    VideoCapture cap(video,cv::CAP_FFMPEG);

    Mat frame;
    cap >> frame;
    // target bounding box
    Rect roi;
    roi = selectROI("tracker", frame, true, false);
    if (roi.width == 0 || roi.height == 0)
        return 0;

    

    int64 t1 = cv::getTickCount();
    
    init(frame,roi);
    // csrt.init(frame,roi);
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
        bool isfound = update(frame, roi);
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


void init(InputArray image_, const Rect & boundingBox)
{
    // we assume the input image is RGB

    current_scale_factor = 1.0;
    Mat image;
    image = image_.getMat();
    image_size = image.size();
    bounding_box = boundingBox;

    // cell_size is number betweem 1 and 4
    // mybe it can be changed

    cell_size = cvFloor(std::min(4.0, std::max(1.0, static_cast<double>(
        cvCeil((bounding_box.width * bounding_box.height)/400.0)) )));

    original_target_size = Size(bounding_box.size());
    
    // extend the size of template wrt padding parameter 
    template_size.width = static_cast<float>(cvFloor(original_target_size.width + params.padding *
            sqrt(original_target_size.width * original_target_size.height)));
    template_size.height = static_cast<float>(cvFloor(original_target_size.height + params.padding *
            sqrt(original_target_size.width * original_target_size.height)));
    // make template size like square !
    template_size.width = template_size.height =
        (template_size.width + template_size.height) / 2.0f;
        
    rescale_ratio = sqrt((params.template_size * params.template_size) / (template_size.width * template_size.height));

    // rescale must be less than one 
    cout<<"rescale_ratio: "<<rescale_ratio<<endl; 
    rescale_ratio = std::min<float>(rescale_ratio,1);

    cout<<"rescale_ratio: "<<rescale_ratio<<endl; 

    // rescaled template size with rescale ratio
    rescaled_template_size = Size2i(cvFloor(template_size.width*rescale_ratio),
                                    cvFloor(template_size.height*rescale_ratio) );

    
    // find center of object in order to construct center of ideal guassian response 
    object_center = Point2f(static_cast<float>(boundingBox.x) + original_target_size.width/2.0f,
                            static_cast<float>(boundingBox.y) + original_target_size.height/2.0f );

    

    cout<<"cell_size: "<<(bounding_box.width * bounding_box.height)/400.0<<endl;
    cout << "template_size "<<template_size<<endl;




    return ;
}

bool update(InputArray image_, Rect& boundingBox)
{
    return true;

}
