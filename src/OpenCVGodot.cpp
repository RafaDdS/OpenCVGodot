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
    image = Ref<Image>( memnew( Image ) );
}

Mat::~Mat()
{
    try
    {
        mat.release();
    }
    catch ( cv::Exception &e )
    {
        UtilityFunctions::push_error( e.what() );
    }
}

Ref<Image> Mat::get_image()
{
    if ( image.is_null() || image->is_empty() )
    {
        cv::Mat rgbMat;

        cv::cvtColor( mat, rgbMat, cv::COLOR_BGR2RGB );
        rgbMat.convertTo( rgbMat, CV_8U );

        int sizear = rgbMat.cols * rgbMat.rows * rgbMat.channels();

        PackedByteArray bytes;
        bytes.resize( sizear );
        memcpy( bytes.ptrw(), rgbMat.data, sizear );

        image = Image::create_from_data( rgbMat.cols, rgbMat.rows, false,
                                         Image::Format::FORMAT_RGB8, bytes );
    }

    return image;
}

void Mat::set_image( Ref<Image> _image )
{

    image = _image;
}

int Mat::get_rows()
{
    return mat.rows;
}

int Mat::get_cols()
{
    return mat.cols;
}

void Mat::convert_to( int rtype )
{
    try
    {
        mat.convertTo( mat, rtype );
    }
    catch ( cv::Exception &e )
    {
        UtilityFunctions::push_error( e.what() );
    }
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
    ClassDB::bind_method( D_METHOD( "get_rows" ), &Mat::get_rows );
    ClassDB::bind_method( D_METHOD( "get_cols" ), &Mat::get_cols );
    ClassDB::bind_method( D_METHOD( "convert_to" ), &Mat::convert_to );
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

    try
    {
        func( mat1->get_mat(), mat2->get_mat(), outMat, mask->get_mat(), dtype );
    }
    catch ( cv::Exception &e )
    {
        UtilityFunctions::push_error( e.what() );
    }

    output->set_mat( outMat );

    return output;
}

// Mat in mat in mat out
Ref<Mat> OpenCVGodot::max( Ref<Mat> mat1, Ref<Mat> mat2 )
{
    return mat_in_mat_in_mat_out_wrapper( &cv::max, mat1, mat2 );
}

Ref<Mat> OpenCVGodot::min( Ref<Mat> mat1, Ref<Mat> mat2 )
{
    return mat_in_mat_in_mat_out_wrapper( &cv::min, mat1, mat2 );
}

Ref<Mat> OpenCVGodot::absdiff( Ref<Mat> mat1, Ref<Mat> mat2 )
{
    return mat_in_mat_in_mat_out_wrapper( &cv::absdiff, mat1, mat2 );
}

Ref<Mat> OpenCVGodot::hconcat( Ref<Mat> mat1, Ref<Mat> mat2 )
{
    return mat_in_mat_in_mat_out_wrapper( &cv::hconcat, mat1, mat2 );
}

Ref<Mat> OpenCVGodot::vconcat( Ref<Mat> mat1, Ref<Mat> mat2 )
{
    return mat_in_mat_in_mat_out_wrapper( &cv::vconcat, mat1, mat2 );
}

Ref<Mat> OpenCVGodot::mat_in_mat_in_mat_out_wrapper( void ( *func )( cv::InputArray, cv::InputArray,
                                                                     cv::OutputArray ),
                                                     Ref<Mat> mat1, Ref<Mat> mat2 )
{
    cv::Mat outMat;
    Ref<Mat> output = Ref<Mat>( memnew( Mat ) );

    try
    {
        func( mat1->get_mat(), mat2->get_mat(), outMat );
    }
    catch ( cv::Exception &e )
    {
        UtilityFunctions::push_error( e.what() );
    }

    output->set_mat( outMat );

    return output;
}

// Bitwise operations
Ref<Mat> OpenCVGodot::bitwise_and( Ref<Mat> mat1, Ref<Mat> mat2, Ref<Mat> mask )
{
    return bitwise_wrapper( &cv::bitwise_and, mat1, mat2, mask );
}

Ref<Mat> OpenCVGodot::bitwise_or( Ref<Mat> mat1, Ref<Mat> mat2, Ref<Mat> mask )
{
    return bitwise_wrapper( &cv::bitwise_or, mat1, mat2, mask );
}

Ref<Mat> OpenCVGodot::bitwise_xor( Ref<Mat> mat1, Ref<Mat> mat2, Ref<Mat> mask )
{
    return bitwise_wrapper( &cv::bitwise_xor, mat1, mat2, mask );
}

Ref<Mat> OpenCVGodot::bitwise_not( Ref<Mat> mat, Ref<Mat> mask )
{
    cv::Mat outMat;
    Ref<Mat> output = Ref<Mat>( memnew( Mat ) );

    if ( mask.is_null() )
    {
        mask = Ref<Mat>( memnew( Mat ) );
    }

	try
    {
        cv::bitwise_not( mat->get_mat(), outMat, mask->get_mat() );
    }
    catch ( cv::Exception &e )
    {
        UtilityFunctions::push_error( e.what() );
    }

    output->set_mat( outMat );

    return output;
}

Ref<Mat> OpenCVGodot::bitwise_wrapper( void ( *func )( cv::InputArray, cv::InputArray,
                                                       cv::OutputArray, cv::InputArray ),
                                       Ref<Mat> mat1, Ref<Mat> mat2, Ref<Mat> mask )
{
    cv::Mat outMat;
    Ref<Mat> output = Ref<Mat>( memnew( Mat ) );

    if ( mask.is_null() )
    {
        mask = Ref<Mat>( memnew( Mat ) );
    }

    try
    {
        func( mat1->get_mat(), mat2->get_mat(), outMat, mask->get_mat() );
    }
    catch ( cv::Exception &e )
    {
        UtilityFunctions::push_error( e.what() );
    }

    output->set_mat( outMat );

    return output;
}

// Mat in mat out
Ref<Mat> OpenCVGodot::convertFp16( Ref<Mat> mat )
{
    return mat_in_mat_out_wrapper( &cv::convertFp16, mat );
}

Ref<Mat> OpenCVGodot::exp( Ref<Mat> mat )
{
    return mat_in_mat_out_wrapper( &cv::exp, mat );
}

Ref<Mat> OpenCVGodot::log( Ref<Mat> mat )
{
    return mat_in_mat_out_wrapper( &cv::log, mat );
}

Ref<Mat> OpenCVGodot::sqrt( Ref<Mat> mat )
{
    return mat_in_mat_out_wrapper( &cv::sqrt, mat );
}

Ref<Mat> OpenCVGodot::transpose( Ref<Mat> mat )
{
    return mat_in_mat_out_wrapper( &cv::transpose, mat );
}

Ref<Mat> OpenCVGodot::mat_in_mat_out_wrapper( void ( *func )( cv::InputArray, cv::OutputArray ),
                                              Ref<Mat> mat )
{
    cv::Mat outMat;
    Ref<Mat> output = Ref<Mat>( memnew( Mat ) );

    try
    {
        func( mat->get_mat(), outMat );
    }
    catch ( cv::Exception &e )
    {
        UtilityFunctions::push_error( e.what() );
    }

    output->set_mat( outMat );

    return output;
}

// Imgcodecs
Ref<Mat> OpenCVGodot::imread( String filename )
{
    cv::Mat outMat;
    Ref<Mat> output = Ref<Mat>( memnew( Mat ) );

    try
    {
        const char *path = filename.utf8().get_data();
        outMat = cv::imread( path );
    }
    catch ( cv::Exception &e )
    {
        UtilityFunctions::push_error( e.what() );
    }

    output->set_mat( outMat );

    return output;
}

// Helper methods
Ref<Mat> OpenCVGodot::take_picture()
{
    Ref<Mat> empty = Ref<Mat>( memnew( Mat ) );

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
    // Helper Methods
    ClassDB::bind_static_method( "OpenCVGodot", D_METHOD( "take_picture" ),
                                 &OpenCVGodot::take_picture );
    // Core
    ClassDB::bind_static_method( "OpenCVGodot", D_METHOD( "add" ), &OpenCVGodot::add, "mat1",
                                 "mat2", "mask", "dType" );
    ClassDB::bind_static_method( "OpenCVGodot", D_METHOD( "subtract" ), &OpenCVGodot::subtract,
                                 "mat1", "mat2", "mask", "dType" );
    ClassDB::bind_static_method( "OpenCVGodot", D_METHOD( "max" ), &OpenCVGodot::max, "mat1",
                                 "mat2" );
    ClassDB::bind_static_method( "OpenCVGodot", D_METHOD( "min" ), &OpenCVGodot::min, "mat1",
                                 "mat2" );
    ClassDB::bind_static_method( "OpenCVGodot", D_METHOD( "absdiff" ), &OpenCVGodot::absdiff,
                                 "mat1", "mat2" );
    ClassDB::bind_static_method( "OpenCVGodot", D_METHOD( "vconcat" ), &OpenCVGodot::vconcat,
                                 "mat1", "mat2" );
    ClassDB::bind_static_method( "OpenCVGodot", D_METHOD( "hconcat" ), &OpenCVGodot::hconcat,
                                 "mat1", "mat2" );
    ClassDB::bind_static_method( "OpenCVGodot", D_METHOD( "bitwise_and" ),
                                 &OpenCVGodot::bitwise_and, "mat1", "mat2", "mask" );
    ClassDB::bind_static_method( "OpenCVGodot", D_METHOD( "bitwise_or" ), &OpenCVGodot::bitwise_or,
                                 "mat1", "mat2", "mask" );
    ClassDB::bind_static_method( "OpenCVGodot", D_METHOD( "bitwise_xor" ),
                                 &OpenCVGodot::bitwise_xor, "mat1", "mat2", "mask" );
    ClassDB::bind_static_method( "OpenCVGodot", D_METHOD( "bitwise_not" ),
                                 &OpenCVGodot::bitwise_not, "mat", "mask" );
    ClassDB::bind_static_method( "OpenCVGodot", D_METHOD( "convertFp16" ),
                                 &OpenCVGodot::convertFp16, "mat" );
    ClassDB::bind_static_method( "OpenCVGodot", D_METHOD( "exp" ), &OpenCVGodot::exp, "mat" );
    ClassDB::bind_static_method( "OpenCVGodot", D_METHOD( "log" ), &OpenCVGodot::log, "mat" );
    ClassDB::bind_static_method( "OpenCVGodot", D_METHOD( "sqrt" ), &OpenCVGodot::sqrt, "mat" );
    ClassDB::bind_static_method( "OpenCVGodot", D_METHOD( "transpose" ), &OpenCVGodot::transpose,
                                 "mat" );

    // Imgcodecs
    ClassDB::bind_static_method( "OpenCVGodot", D_METHOD( "imread" ), &OpenCVGodot::imread,
                                 "filename" );
}
