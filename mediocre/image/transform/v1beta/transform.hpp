#ifndef mediocre_image_transform_v1beta_h
#define mediocre_image_transform_v1beta_h

#include <mediocre/image/transform/v1beta/transform.grpc.pb.h>
#include <opencv2/core/mat.hpp>

namespace mediocre::image::transform::v1beta {

    using grpc::ServerContext;
    using grpc::ServerWriter;
    using grpc::Status;
    using mediocre::image::v1beta::Image;

    class TransformServiceImpl final : public TransformService::Service {
    public:
        Status Transform(
                ServerContext *context,
                const TransformRequest *request,
                ServerWriter<TransformResponse> *writer) override;

    private:
        static std::vector<std::function<cv::Mat(cv::Mat)>>
        getImageTransformations(const google::protobuf::RepeatedPtrField<TransformToImage> &trs);

        static std::function<cv::Mat(cv::Mat)>
        getImageTransformation(const TransformToImage &transform);

        static std::function<std::string(cv::Mat)>
        getCharactersTransformation(const TransformToOther &transform);

        static std::function<cv::Mat(cv::Mat)>
        compose(const std::vector<std::function<cv::Mat(cv::Mat)>> &functions, const std::function<void(cv::Mat, double)> &onTransform);

        template<typename Type>
        static void
        compose(std::function<Type(Type)> &composedFunction, const std::vector<std::function<Type(Type)>> &functions, const std::function<void(Type, double)> &onTransform);

        template<typename IN, typename OUT>
        static std::function<OUT(IN)>
        compose(const std::function<IN(IN)> &composedFunction, const std::function<OUT(IN)> &function, const std::function<void(OUT, double)> &onTransform);
    };

}// namespace mediocre::image::transform::v1beta

#endif// mediocre_image_transform_v1beta_h