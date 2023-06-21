#include <iomanip>
#include <leptonica/allheaders.h>
#include <libmediocre/dummy/dummy.hpp>
#include <opencv4/opencv2/opencv.hpp>
#include <tesseract/baseapi.h>

namespace mediocre::dummy {

    grpc::Status DummyServiceImpl::DoThing(
            grpc::ServerContext *context,
            const DummyRequest *request,
            DummyResponse *response) {
        response->set_response("yes");
        return grpc::Status::OK;
    }

    //
    //    int opencv(int argc, char** argv )
    //    {
    //        if ( argc != 2 )
    //        {
    //            printf("usage: DisplayImage.out <Image_Path>\n");
    //            return -1;
    //        }
    //        cv::Mat image;
    //        image = cv::imread( argv[1], cv::IMREAD_COLOR );
    //        if ( !image.data )
    //        {
    //            printf("No image data \n");
    //            return -1;
    //        }
    //        cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
    //        cv::imshow("Display Image", image);
    //        cv::waitKey(0);
    //        return 0;
    //    }
    //
    //    int tesseract()
    //    {
    //        char *outText;
    //
    //        tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    //        // Initialize tesseract-ocr with English, without specifying tessdata path
    //        if (api->Init(NULL, "eng")) {
    //            fprintf(stderr, "Could not initialize tesseract.\n");
    //            exit(1);
    //        }
    //
    //        // Open input image with leptonica library
    //        Pix *image = pixRead("/usr/src/tesseract/testing/phototest.tif");
    //        api->SetImage(image);
    //        // Get OCR result
    //        outText = api->GetUTF8Text();
    //        printf("OCR output:\n%s", outText);
    //
    //        // Destroy used object and release memory
    //        api->End();
    //        delete api;
    //        delete [] outText;
    //        pixDestroy(&image);
    //
    //        return 0;
    //    }


}// namespace mediocre::dummy
