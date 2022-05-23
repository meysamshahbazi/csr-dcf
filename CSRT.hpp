
#ifndef CSRT_HPP
#define CSRT_HPP


#include "precomp.hpp"
#include <iostream>
#include "CSRTSegmentation.hpp"
#include "CSRTUtils.hpp"
#include "CSRTScaleEstimation.hpp"

using namespace std;

namespace cv {
inline namespace tracking {
namespace impl {
class TrackerCSRTModel CV_FINAL : public TrackerModel
{
public:
    TrackerCSRTModel(){}
    ~TrackerCSRTModel(){}
protected:
    void modelEstimationImpl(const std::vector<Mat>& /*responses*/) CV_OVERRIDE {}
    void modelUpdateImpl() CV_OVERRIDE {}
};

class TrackerCSRTImpl CV_FINAL : public TrackerCSRT
{
public:
    TrackerCSRTImpl(const Params &parameters = Params());

    Params params;

    Ptr<TrackerCSRTModel> model;

    // Tracker API
    virtual void init(InputArray image, const Rect& boundingBox) CV_OVERRIDE;
    virtual bool update(InputArray image, Rect& boundingBox) CV_OVERRIDE;
    virtual void setInitialMask(InputArray mask) CV_OVERRIDE;

protected:
    void update_csr_filter(const Mat &image, const Mat &my_mask);
    void update_histograms(const Mat &image, const Rect &region);
    void extract_histograms(const Mat &image, cv::Rect region, Histogram &hf, Histogram &hb);
    std::vector<Mat> create_csr_filter(const std::vector<cv::Mat>
            img_features, const cv::Mat Y, const cv::Mat P);
    Mat calculate_response(const Mat &image, const std::vector<Mat> filter);
    Mat get_location_prior(const Rect roi, const Size2f target_size, const Size img_sz);
    Mat segment_region(const Mat &image, const Point2f &object_center,
            const Size2f &template_size, const Size &target_size, float scale_factor);
    Point2f estimate_new_position(const Mat &image);
    std::vector<Mat> get_features(const Mat &patch, const Size2i &feature_size);

    bool check_mask_area(const Mat &mat, const double obj_area);
    float current_scale_factor;
    Mat window;
    Mat yf;
    Rect2f bounding_box;
    std::vector<Mat> csr_filter;
    std::vector<float> filter_weights;
    Size2f original_target_size;
    Size2i image_size;
    Size2f template_size;
    Size2i rescaled_template_size;
    float rescale_ratio;
    Point2f object_center;
    DSST dsst;
    Histogram hist_foreground;
    Histogram hist_background;
    double p_b;
    Mat erode_element;
    Mat filter_mask;
    Mat preset_mask;
    Mat default_mask;
    float default_mask_area;
    int cell_size;
};


}}}

#endif
