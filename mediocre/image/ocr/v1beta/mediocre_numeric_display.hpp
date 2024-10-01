#ifndef MEDIOCRE_MEDIOCRE_NUMERIC_DISPLAY_HPP
#define MEDIOCRE_MEDIOCRE_NUMERIC_DISPLAY_HPP

#include <mediocre/image/ocr/v1beta/ocr.pb.h>
#include <opencv2/core/mat.hpp>
#include <string>

namespace mediocre::image::ocr::v1beta {
    class MediocreNumericDisplay {
    public:
        static std::string GetCharacters(const cv::Mat &input, const NumericDisplayParams &params);

    private:
        static char RecogniseCharacter(const cv::Mat &character);
        static std::vector<cv::Mat> DetectCharacters(const cv::Mat &input);
        static cv::Mat ExtractCharacter(const cv::Mat &input, int startColumnInclusive, int endColumnExclusive);
        static std::pair<int, int> FindVerticalBounds(const cv::Mat &input, int startColumnInclusive, int endColumnExclusive);
    };
}// namespace mediocre::image::ocr::v1beta


#endif//MEDIOCRE_MEDIOCRE_NUMERIC_DISPLAY_HPP
