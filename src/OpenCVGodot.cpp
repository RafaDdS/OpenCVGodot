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
Ref<Mat> OpenCVGodot::subtract( Ref<Mat> mat1, Ref<Mat> mat2 )
{
    cv::Mat outMat;
    Ref<Mat> output = Ref<Mat>( memnew( Mat ) );
    auto mask = Ref<Mat>( memnew( Mat ) );
    auto dtype = -1;

    cv::subtract( mat1->get_mat(), mat2->get_mat(), outMat, mask->get_mat(), dtype );

    output->set_mat( outMat );

    return output;
}

void OpenCVGodot::emitCustomSignal( const String &inName, int inValue )
{
    emit_signal( "custom_signal", inName, inValue );
}

// Static methods
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
    // Methods.
    ClassDB::bind_static_method( "OpenCVGodot", D_METHOD( "take_picture" ),
                                 &OpenCVGodot::takePicture );
    ClassDB::bind_static_method( "OpenCVGodot", D_METHOD( "subtract" ), &OpenCVGodot::subtract );

    // Signals.
    ADD_SIGNAL( MethodInfo( "custom_signal", PropertyInfo( Variant::STRING, "name" ),
                            PropertyInfo( Variant::INT, "value" ) ) );
    ClassDB::bind_method( D_METHOD( "emit_custom_signal", "name", "value" ),
                          &OpenCVGodot::emitCustomSignal );
}

void OpenCVGodot::_notification( int inWhat )
{
    // UtilityFunctions::print( "Notification: ", String::num( inWhat ) );
}

bool OpenCVGodot::_set( const StringName &inName, const Variant &inValue )
{
    String name = inName;

    if ( name.begins_with( "dproperty" ) )
    {
        int64_t index = name.get_slicec( '_', 1 ).to_int();
        mDProp[index] = inValue;

        return true;
    }

    if ( name == "property_from_list" )
    {
        mPropertyFromList = inValue;

        return true;
    }

    return false;
}

bool OpenCVGodot::_get( const StringName &inName, Variant &outReturn ) const
{
    String name = inName;

    if ( name.begins_with( "dproperty" ) )
    {
        int64_t index = name.get_slicec( '_', 1 ).to_int();
        outReturn = mDProp[index];

        return true;
    }

    if ( name == "property_from_list" )
    {
        outReturn = mPropertyFromList;

        return true;
    }

    return false;
}

void OpenCVGodot::_get_property_list( List<PropertyInfo> *outList ) const
{
    outList->push_back( PropertyInfo( Variant::VECTOR3, "property_from_list" ) );

    for ( int i = 0; i < 3; ++i )
    {
        outList->push_back( PropertyInfo( Variant::VECTOR2, "dproperty_" + itos( i ) ) );
    }
}

bool OpenCVGodot::_property_can_revert( const StringName &inName ) const
{
    if ( inName == StringName( "property_from_list" ) && mPropertyFromList != Vector3( 1, 1, 1 ) )
    {
        return true;
    }

    return false;
}

bool OpenCVGodot::_property_get_revert( const StringName &inName, Variant &outProperty ) const
{
    if ( inName == StringName( "property_from_list" ) )
    {
        outProperty = Vector3( 1, 1, 1 );

        return true;
    }

    return false;
}

void OpenCVGodot::_validate_property( PropertyInfo &inProperty ) const
{
    String name = inProperty.name;

    // Test hiding the "mouse_filter" property from the editor.
    if ( name == "mouse_filter" )
    {
        inProperty.usage = PROPERTY_USAGE_NO_EDITOR;
    }
}

String OpenCVGodot::_to_string() const
{
    return "[ GDExtension::OpenCVGodot <--> Instance ID:" + uitos( get_instance_id() ) + " ]";
}
