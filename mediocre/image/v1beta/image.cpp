#include <mediocre/image/v1beta/image.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>

namespace mediocre::image::v1beta {

    cv::Mat Decode(std::string image) {
        const auto length = static_cast<int>(image.length());
        const auto encoded = cv::Mat(1, length, CV_8UC1, image.data());
        return cv::imdecode(encoded, cv::IMREAD_UNCHANGED);
    }

    cv::Mat Decode(const Image &image) {
        const auto &image_data_string = image.image_data();
        if (image_data_string.length() > INT_MAX) {
            throw std::invalid_argument("Image was too large");
        }

        return Decode(image_data_string);
    }

    std::string Encode(const cv::Mat &mat) {
        std::vector<uchar> buffer;
        cv::imencode(".png", mat, buffer);
        return { buffer.begin(), buffer.end() };
    }

    void Encode(const cv::Mat &mat, Image *image) {
        auto encoded = Encode(mat);
        image->set_height(mat.rows);
        image->set_width(mat.cols);
        image->set_image_data(encoded);
    }

} // namespace mediocre::image::v1beta