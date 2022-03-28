#ifndef CSRT_HPP
#define CSRT_HPP
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <iostream>


class CSRT {
    public:
        CSRT();
        CSRT(int x);
        ~CSRT();
        void init(cv::InputArray img, const cv::Rect & boundingBox);

    private:
        bool use_channel_weights = true;
        bool use_segmentation = true;
        bool use_hog = true;
        bool use_color_names = true;
        bool use_gray = true;
        bool use_rgb = false;
        std::string  window_function = "hann";
        float kaiser_alpha = 3.75f;
        int cheb_attenuation = 45;
        float padding = 3.0f;
        int template_size = 200;
        float gsl_sigma = 1.0f;
        int hog_orientations = 9;
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
        float histogram_bins = 16;
        float background_ratio = 2;
        float histogram_lr = 0.04f;
        float psr_threshold = 0.035f;
        //-----------------------------------------------
        float current_scale_factor = 1.0f;
        cv::Rect bounding_box;

};

#endif 


