#include <mediocre/image/ocr/v1beta/mediocre_numeric_display.hpp>
#include <mediocre/image/ocr/v1beta/ocr.pb.h>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <stdexcept>
#include <utility>

namespace mediocre::image::ocr::v1beta {

    std::string MediocreNumericDisplay::GetCharacters(const cv::Mat &input, const NumericDisplayParams &params) {
        std::string characters;

        const auto characterMats = DetectCharacters(input);
        for (const auto &characterMat: characterMats) {
            const auto character = RecogniseCharacter(characterMat);
            characters.push_back(character);
        }

        return characters;
    }

    std::pair<int, int> MediocreNumericDisplay::FindVerticalBounds(const cv::Mat &input, int startColumnInclusive, int endColumnExclusive) {
        int topRow = -1;
        int bottomRow = -1;

        for (int rowNum = 0; rowNum < input.rows; rowNum++) {
            const cv::Mat &row = input.row(rowNum).colRange(startColumnInclusive, endColumnExclusive);
            int nonZeroPixelsInRow = cv::countNonZero(row);

            if (nonZeroPixelsInRow > 0) {
                if (topRow == -1) {
                    topRow = rowNum;
                }
                bottomRow = rowNum;
            }
        }

        return {topRow, bottomRow};
    }

    cv::Mat MediocreNumericDisplay::ExtractCharacter(const cv::Mat &input, int startColumnInclusive, int endColumnExclusive) {
        auto [topRow, bottomRow] = FindVerticalBounds(input, startColumnInclusive, endColumnExclusive);

        if (topRow == -1 || bottomRow == -1) {
            throw std::invalid_argument("No character found in the given column range.");
        }

        int width = endColumnExclusive - startColumnInclusive;
        int height = bottomRow - topRow + 1;
        cv::Rect characterBounds(startColumnInclusive, topRow, width, height);

        return input(characterBounds);
    }

    std::vector<cv::Mat> MediocreNumericDisplay::DetectCharacters(const cv::Mat &input) {
        std::vector<cv::Mat> characters;
        bool isInsideCharacter = false;
        int characterStartColumn = 0;

        for (int column = 0; column < input.cols; column++) {
            bool columnHasNonZero = cv::countNonZero(input.col(column)) > 0;

            if (columnHasNonZero && !isInsideCharacter) {
                // Start of a new character
                characterStartColumn = column;
                isInsideCharacter = true;
            } else if (!columnHasNonZero && isInsideCharacter) {
                // End of a character
                const cv::Mat &character = ExtractCharacter(input, characterStartColumn, column);
                characters.push_back(character);
                isInsideCharacter = false;
            }
        }

        if (isInsideCharacter) {
            const cv::Mat &character = ExtractCharacter(input, characterStartColumn, input.cols);
            characters.push_back(character);
        }

        return characters;
    }

    const auto &segment0 = std::vector<bool>{true, true, true, false, true, true, true};
    const auto &segment2 = std::vector<bool>{true, false, true, true, true, false, true};
    const auto &segment3 = std::vector<bool>{true, false, true, true, false, true, true};
    const auto &segment4 = std::vector<bool>{false, true, true, true, false, true, false};
    const auto &segment5 = std::vector<bool>{true, true, false, true, false, true, true};
    const auto &segment6 = std::vector<bool>{true, true, false, true, true, true, true};
    const auto &segmentStraight7 = std::vector<bool>{true, false, true, false, false, true, false};
    const auto &segmentSlanted7 = std::vector<bool>{true, false, false, false, false, false, false};
    const auto &segment8 = std::vector<bool>{true, true, true, true, true, true, true};
    const auto &segment9 = std::vector<bool>{true, true, true, true, false, true, true};

    char MediocreNumericDisplay::RecogniseCharacter(const cv::Mat &character) {
        const auto width = character.cols;
        const auto height = character.rows;

        const auto ratio = static_cast<double>(width) / height;
        if (ratio < 0.5) {
            const auto row = character.row(height / 2);
            const auto nonZeroPixelsInRow = cv::countNonZero(row);

            if (nonZeroPixelsInRow > 0) {
                return '1';
            } else {
                return ':';
            }
        }

        // this will fall over with very small characters
        const auto lookupWidth = 3;
        const auto lookupHeight = 3;

        const auto lookupArea = lookupWidth * lookupHeight;
        const auto lookupThreshold = lookupArea / 2;

        const cv::Rect topSegmentRect((width / 2) - 1, 0, lookupWidth, lookupHeight);
        const cv::Rect topLeftSegmentRect(0, (height / 4), lookupWidth, lookupHeight);
        const cv::Rect topRightSegmentRect(width - lookupWidth, (height / 4), lookupWidth, lookupHeight);
        const cv::Rect middleSegmentRect((width / 2) - 1, (height / 2) - 1, lookupWidth, lookupHeight);
        const cv::Rect bottomLeftSegmentRect(0, 3 * (height / 4), lookupWidth, lookupHeight);
        const cv::Rect bottomRightSegmentRect(width - lookupWidth, 3 * (height / 4), lookupWidth, lookupHeight);
        const cv::Rect bottomSegmentRect((width / 2) - 1, height - lookupHeight, lookupWidth, lookupHeight);

        const cv::Mat topSegmentMat = character(topSegmentRect);
        const cv::Mat topLeftSegmentMat = character(topLeftSegmentRect);
        const cv::Mat topRightSegmentMat = character(topRightSegmentRect);
        const cv::Mat middleSegmentMat = character(middleSegmentRect);
        const cv::Mat bottomLeftSegmentMat = character(bottomLeftSegmentRect);
        const cv::Mat bottomRightSegmentMat = character(bottomRightSegmentRect);
        const cv::Mat bottomSegmentMat = character(bottomSegmentRect);

        const auto topSegment = cv::countNonZero(topSegmentMat) > lookupThreshold;
        const auto topLeftSegment = cv::countNonZero(topLeftSegmentMat) > lookupThreshold;
        const auto topRightSegment = cv::countNonZero(topRightSegmentMat) > lookupThreshold;
        const auto middleSegment = cv::countNonZero(middleSegmentMat) > lookupThreshold;
        const auto bottomLeftSegment = cv::countNonZero(bottomLeftSegmentMat) > lookupThreshold;
        const auto bottomRightSegment = cv::countNonZero(bottomRightSegmentMat) > lookupThreshold;
        const auto bottomSegment = cv::countNonZero(bottomSegmentMat) > lookupThreshold;
        const auto segments = std::vector<bool>{
                topSegment,
                topLeftSegment,
                topRightSegment,
                middleSegment,
                bottomLeftSegment,
                bottomRightSegment,
                bottomSegment};

        if (segments == segment0) {
            return '0';
        }

        // 1 is handled earlier

        if (segments == segment2) {
            return '2';
        }

        if (segments == segment3) {
            return '3';
        }

        if (segments == segment4) {
            return '4';
        }

        if (segments == segment5) {
            return '5';
        }

        if (segments == segment6) {
            return '6';
        }

        if (segments == segmentStraight7 || segments == segmentSlanted7) {
            return '7';
        }

        if (segments == segment8) {
            return '8';
        }

        if (segments == segment9) {
            return '9';
        }

        throw std::invalid_argument("Character could not be recognised");
    }
}// namespace mediocre::image::ocr::v1beta
