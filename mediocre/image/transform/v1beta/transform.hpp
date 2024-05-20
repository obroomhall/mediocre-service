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
    };

}// namespace mediocre::image::transform::v1beta

#endif// mediocre_image_transform_v1beta_h