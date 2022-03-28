
#include "csrt.hpp"

CSRT::CSRT() {};

CSRT::CSRT(int x )
{
    
    std::cout<<"CSRT"<<histogram_bins<<std::endl;
    
}

CSRT::~CSRT()
{
    std::cout<<"CSRT"<<std::endl;
    
}

void CSRT::init(cv::InputArray img_, const cv::Rect & boundingBox)
{
    cv::Mat img = img_.getMat();
    current_scale_factor = 1.0;
    bounding_box = boundingBox;

    


    return;
}