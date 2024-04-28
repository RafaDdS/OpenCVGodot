// Copied from godot-cpp/test/src and modified.

#include "godot_cpp/classes/global_constants.hpp"
#include "godot_cpp/classes/image.hpp"
#include "godot_cpp/classes/label.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/variant/utility_functions.hpp"

#include <limits.h>
#include <opencv2/opencv.hpp>
#include <unistd.h>

#include "OpenCVGodot.h"

// Used to mark unused parameters to indicate intent and suppress warnings.
#define UNUSED( expr ) (void)( expr )

using namespace godot;

Mat::Mat()
{
    if ( image == nullptr )
    {
        image = Ref<Image>( memnew( Image ) );
    }
}

Mat::Mat( String path )
{
    if ( image == nullptr )
    {
        image = Ref<Image>( memnew( Image ) );
    }
}

Mat::~Mat()
{
}

void Mat::content()
{
}

Ref<Image> Mat::get_image()
{
    if ( image.is_null() || image->is_empty() )
    {
        Array *array = memnew( Array );

        uint8_t *pixelPtr = (uint8_t *)mat.data;
        int cn = mat.channels();
        cv::Scalar_<uint8_t> bgrPixel;

        for ( int i = 0; i < mat.rows; i++ )
        {
            for ( int j = 0; j < mat.cols; j++ )
            {
                auto B = pixelPtr[i * mat.cols * cn + j * cn + 0]; // B
                auto G = pixelPtr[i * mat.cols * cn + j * cn + 1]; // G
                auto R = pixelPtr[i * mat.cols * cn + j * cn + 2]; // R

                // do something with BGR values...
                array->append( R );
                array->append( G );
                array->append( B );
            }
        }

        image = Image::create_from_data( mat.cols, mat.rows, false, Image::Format::FORMAT_RGB8,
                                         PackedByteArray( *array ) );
    }

    return image;
}

void Mat::set_image( Ref<Image> _image )
{

    image = _image;
}

void Mat::set_mat( cv::Mat _mat )
{

    mat = _mat;
}

cv::Mat Mat::get_mat()
{

    return mat;
}

void Mat::_bind_methods()
{
    ClassDB::bind_method( D_METHOD( "get_image" ), &Mat::get_image );
    ClassDB::bind_method( D_METHOD( "set_image", "Image" ), &Mat::set_image );
}

OpenCVGodot::OpenCVGodot()
{
    UtilityFunctions::print( "Constructor." );
}

OpenCVGodot::~OpenCVGodot()
{
    UtilityFunctions::print( "Destructor." );
}

// Arithmetic Operations
Ref<Mat> OpenCVGodot::add( Ref<Mat> mat1, Ref<Mat> mat2, Ref<Mat> mask, int dtype )
{
    return arithmetic_wrapper( &cv::add, mat1, mat2, mask, dtype );
}

Ref<Mat> OpenCVGodot::subtract( Ref<Mat> mat1, Ref<Mat> mat2, Ref<Mat> mask, int dtype )
{
    return arithmetic_wrapper( &cv::subtract, mat1, mat2, mask, dtype );
}

Ref<Mat> OpenCVGodot::arithmetic_wrapper( void ( *func )( cv::InputArray, cv::InputArray,
                                                          cv::OutputArray, cv::InputArray, int ),
                                          Ref<Mat> mat1, Ref<Mat> mat2, Ref<Mat> mask, int dtype )
{
    cv::Mat outMat;
    Ref<Mat> output = Ref<Mat>( memnew( Mat ) );

    if ( mask.is_null() )
    {
        mask = Ref<Mat>( memnew( Mat ) );
    }

    func( mat1->get_mat(), mat2->get_mat(), outMat, mask->get_mat(), dtype );

    output->set_mat( outMat );

    return output;
}
// MaxMin
Ref<Mat> OpenCVGodot::max( Ref<Mat> mat1, Ref<Mat> mat2 )
{
    return mat_in_mat_in_mat_out_wrapper(&cv::max, mat1, mat2);
}

Ref<Mat> OpenCVGodot::min( Ref<Mat> mat1, Ref<Mat> mat2 )
{
    return mat_in_mat_in_mat_out_wrapper(&cv::min, mat1, mat2);
}

Ref<Mat> OpenCVGodot::absdiff( Ref<Mat> mat1, Ref<Mat> mat2 )
{
    return mat_in_mat_in_mat_out_wrapper(&cv::absdiff, mat1, mat2);
}

Ref<Mat> OpenCVGodot::hconcat( Ref<Mat> mat1, Ref<Mat> mat2 )
{
    return mat_in_mat_in_mat_out_wrapper(&cv::hconcat, mat1, mat2);
}

Ref<Mat> OpenCVGodot::vconcat( Ref<Mat> mat1, Ref<Mat> mat2 )
{
    return mat_in_mat_in_mat_out_wrapper(&cv::vconcat, mat1, mat2);
}

Ref<Mat> OpenCVGodot::mat_in_mat_in_mat_out_wrapper( void ( *func )( cv::InputArray, cv::InputArray,
                                                                     cv::OutputArray ),
                                                     Ref<Mat> mat1, Ref<Mat> mat2 )
{
	cv::Mat outMat;
    Ref<Mat> output = Ref<Mat>( memnew( Mat ) );

    func( mat1->get_mat(), mat2->get_mat(), outMat );

    output->set_mat( outMat );

    return output;
}

// Added methods
Ref<Mat> OpenCVGodot::takePicture()
{
    auto empty = Ref<Mat>( memnew( Mat ) );

    cv::Mat frame;

    cv::VideoCapture cap;

    int deviceID = 0;        // 0 = open default camera
    int apiID = cv::CAP_ANY; // 0 = autodetect default API

    cap.open( deviceID, apiID );

    if ( !cap.isOpened() )
    {
        UtilityFunctions::print( "ERROR! Unable to open camera\n" );
        return empty;
    }

    cap.read( frame );

    if ( frame.empty() )
    {
        UtilityFunctions::print( "ERROR! blank frame grabbed\n" );
        return empty;
    }

    empty->set_mat( frame );

    return empty;
}

void OpenCVGodot::_bind_methods()
{
    // Added Methods
    ClassDB::bind_static_method( "OpenCVGodot", D_METHOD( "take_picture" ),
                                 &OpenCVGodot::takePicture );
    // Core
    ClassDB::bind_static_method( "OpenCVGodot", D_METHOD( "add" ), &OpenCVGodot::add, "mat1",
                                 "mat2", "mask", "dType" );
    ClassDB::bind_static_method( "OpenCVGodot", D_METHOD( "subtract" ), &OpenCVGodot::subtract,
                                 "mat1", "mat2", "mask", "dType" );
    ClassDB::bind_static_method( "OpenCVGodot", D_METHOD( "max" ), &OpenCVGodot::max, "mat1",
                                 "mat2" );
	ClassDB::bind_static_method( "OpenCVGodot", D_METHOD( "min" ), &OpenCVGodot::min, "mat1",
                                 "mat2" );
	ClassDB::bind_static_method( "OpenCVGodot", D_METHOD( "absdiff" ), &OpenCVGodot::absdiff, "mat1",
                                 "mat2" );
	ClassDB::bind_static_method( "OpenCVGodot", D_METHOD( "vconcat" ), &OpenCVGodot::vconcat, "mat1",
                                 "mat2" );
	ClassDB::bind_static_method( "OpenCVGodot", D_METHOD( "hconcat" ), &OpenCVGodot::hconcat, "mat1",
                                 "mat2" );
}
