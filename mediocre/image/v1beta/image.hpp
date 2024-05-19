#ifndef mediocre_image_v1beta_h
#define mediocre_image_v1beta_h

#include <mediocre/image/v1beta/image.pb.h>
#include <opencv2/core/mat.hpp>

namespace mediocre::image::v1beta {

    cv::Mat Decode(const Image& image);
    void Encode(const cv::Mat& mat, Image* image);

} // namespace mediocre::image::v1beta


#endif //mediocre_image_v1beta_h
