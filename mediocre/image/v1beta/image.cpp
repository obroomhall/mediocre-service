#include <mediocre/image/v1beta/image.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>

namespace mediocre::image::v1beta {

    cv::Mat Decode(std::string image, int length) {
        const auto encoded = cv::Mat(1, length, CV_8UC1, image.data());
        return cv::imdecode(encoded, cv::IMREAD_COLOR);
    }

    cv::Mat Decode(const Image &image) {
        const auto &blob = image.blob().data();
        if (blob.length() > INT_MAX) {
            throw std::invalid_argument("Image was too large");
        }

        const auto length = static_cast<int>(blob.length());
        return Decode(blob, length);
    }

    std::string Encode(const cv::Mat &mat) {
        std::vector<uchar> buffer;
        cv::imencode(".png", mat, buffer);
        return {buffer.begin(), buffer.end()};
    }

    void Encode(const cv::Mat &mat, Image *image) {
        auto encoded = Encode(mat);
        image->mutable_blob()->set_data(encoded);
    }

}// namespace mediocre::image::v1beta