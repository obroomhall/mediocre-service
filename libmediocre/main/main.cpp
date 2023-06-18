#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <iomanip>
#include <libmediocre/main/main.hpp>
#include <opencv4/opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

namespace mediocre::main {
    // Logic and data behind the server's behavior.
    class GreeterServiceImpl final : public Greeter::Service {
        grpc::Status SayHello(grpc::ServerContext *context, const HelloRequest *request,
                              HelloReply *reply) override {
            std::string prefix("Hello ");
            std::string suffix(" from C++");
            reply->set_message(prefix + request->name() + suffix);
            return grpc::Status::OK;
        }
    };

    void RunServer(uint16_t port) {
        std::ostringstream server_address_stream;
        server_address_stream << "0.0.0.0:" << port;
        std::string server_address = server_address_stream.str();
        GreeterServiceImpl service;

        grpc::EnableDefaultHealthCheckService(true);
        grpc::reflection::InitProtoReflectionServerBuilderPlugin();
        grpc::ServerBuilder builder;
        // Listen on the given address without any authentication mechanism.
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        // Register "service" as the instance through which we'll communicate with
        // clients. In this case it corresponds to an *synchronous* service.
        builder.RegisterService(&service);
        // Finally assemble the server.
        std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
        std::cout << "Server listening on " << server_address << std::endl;

        // Wait for the server to shutdown. Note that some other thread must be
        // responsible for shutting down the server for this call to ever return.
        server->Wait();
    }

    int opencv(int argc, char** argv )
    {
        if ( argc != 2 )
        {
            printf("usage: DisplayImage.out <Image_Path>\n");
            return -1;
        }
        cv::Mat image;
        image = cv::imread( argv[1], cv::IMREAD_COLOR );
        if ( !image.data )
        {
            printf("No image data \n");
            return -1;
        }
        cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
        cv::imshow("Display Image", image);
        cv::waitKey(0);
        return 0;
    }

    int tesseract()
    {
        char *outText;

        tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
        // Initialize tesseract-ocr with English, without specifying tessdata path
        if (api->Init(NULL, "eng")) {
            fprintf(stderr, "Could not initialize tesseract.\n");
            exit(1);
        }

        // Open input image with leptonica library
        Pix *image = pixRead("/usr/src/tesseract/testing/phototest.tif");
        api->SetImage(image);
        // Get OCR result
        outText = api->GetUTF8Text();
        printf("OCR output:\n%s", outText);

        // Destroy used object and release memory
        api->End();
        delete api;
        delete [] outText;
        pixDestroy(&image);

        return 0;
    }
}

int main(int argc, char *argv[]) {
    mediocre::main::RunServer(50051);
    return EXIT_SUCCESS;
}
