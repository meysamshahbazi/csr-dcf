
#include "csrt.hpp"

CsrtParams params;


CSRT::CSRT(CsrtParams csrtparams=params) 
    :csrtparams(csrtparams)
{
    std::cout<<csrtparams.use_channel_weights<<std::endl;

};





CSRT::~CSRT()
{

}

void CSRT::init(cv::InputArray img_, const cv::Rect & boundingBox)
{
    cv::Mat img = img_.getMat();
    current_scale_factor = 1.0;
    bounding_box = boundingBox;
    image_size = image_size;
    bounding_box = boundingBox;

    cell_size = cvFloor(std::min(4.0, std::max(1.0,
    static_cast<double>cvCeil((bounding_box.width*bounding_box.height/400.0)))));

    


    return;
}