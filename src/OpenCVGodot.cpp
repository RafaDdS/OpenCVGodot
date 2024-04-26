// Copied from godot-cpp/test/src and modified.

#include "godot_cpp/classes/global_constants.hpp"
#include "godot_cpp/classes/label.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/variant/utility_functions.hpp"
#include "godot_cpp/classes/image.hpp"

#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <limits.h>

#include "OpenCVGodot.h"

// Used to mark unused parameters to indicate intent and suppress warnings.
#define UNUSED( expr ) (void)( expr )


using namespace godot;


Mat::Mat(){
	if (image == nullptr) {
        image = Ref<Image>(memnew(Image));
    }
}

Mat::~Mat(){}

void Mat::content(){}

Ref<Image> Mat::getImage(){
	return image;
}

void Mat::setImage(Ref<Image> _image){

    image = _image;
}

void Mat::_bind_methods(){
	ClassDB::bind_method( D_METHOD( "get_Image" ),
                                 &Mat::getImage );
    ClassDB::bind_method( D_METHOD( "set_Image", "Image" ),
                                 &Mat::setImage );
}


OpenCVGodot::OpenCVGodot()
{
    UtilityFunctions::print( "Constructor." );
}

OpenCVGodot::~OpenCVGodot()
{
    UtilityFunctions::print( "Destructor." );
}

// Methods.
void OpenCVGodot::simpleFunc()
{
    UtilityFunctions::print( "  Simple func called." );
}

void OpenCVGodot::emitCustomSignal( const String &inName, int inValue )
{
    emit_signal( "custom_signal", inName, inValue );
}

// Static methods
Ref<Image> OpenCVGodot::takePicture( )
{

	auto empty = Ref<Image>(memnew(Image));

	cv::Mat frame;

	cv::VideoCapture cap;

	int deviceID = 0; // 0 = open default camera
	int apiID = cv::CAP_ANY; // 0 = autodetect default API
	UtilityFunctions::print( deviceID );
	UtilityFunctions::print( apiID );

	

	cap.open(deviceID, apiID);

	if (!cap.isOpened()) {
		UtilityFunctions::print( "ERROR! Unable to open camera\n" );
		return empty;
	}

	cap.read(frame);

	
	if (frame.empty()) {
		UtilityFunctions::print( "ERROR! blank frame grabbed\n" );
		return empty;
	}

	

	Array* array = memnew(Array);

	uint8_t* pixelPtr = (uint8_t*)frame.data;
	int cn = frame.channels();
	cv::Scalar_<uint8_t> bgrPixel;

	for(int i = 0; i < frame.rows; i++)
	{
		for(int j = 0; j < frame.cols; j++)
		{
			auto B = pixelPtr[i*frame.cols*cn + j*cn + 0]; // B
			auto G = pixelPtr[i*frame.cols*cn + j*cn + 1]; // G
			auto R = pixelPtr[i*frame.cols*cn + j*cn + 2]; // R
			
			// do something with BGR values...
			array->append(R);
			array->append(G);
			array->append(B);
		}
	}
	
	

	UtilityFunctions::print( array );

	auto output = Image::create_from_data (frame.cols, frame.rows, false, Image::Format::FORMAT_RGB8, PackedByteArray(*array));

	return output;
}

void OpenCVGodot::_bind_methods()
{
    // Methods.
    ClassDB::bind_method( D_METHOD( "simple_func" ), &OpenCVGodot::simpleFunc );
    ClassDB::bind_static_method( "OpenCVGodot", D_METHOD( "take_picture" ),
                                        &OpenCVGodot::takePicture );




    // Signals.
    ADD_SIGNAL( MethodInfo( "custom_signal",
                                   PropertyInfo( Variant::STRING, "name" ),
                                   PropertyInfo( Variant::INT, "value" ) ) );
    ClassDB::bind_method( D_METHOD( "emit_custom_signal", "name", "value" ),
                                 &OpenCVGodot::emitCustomSignal );
}

void OpenCVGodot::_notification( int inWhat )
{
    //UtilityFunctions::print( "Notification: ", String::num( inWhat ) );
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
        outList->push_back(
            PropertyInfo( Variant::VECTOR2, "dproperty_" + itos( i ) ) );
    }
}

bool OpenCVGodot::_property_can_revert( const StringName &inName ) const
{
    if ( inName == StringName( "property_from_list" ) &&
         mPropertyFromList != Vector3( 1, 1, 1 ) )
    {
        return true;
    }

    return false;
}

bool OpenCVGodot::_property_get_revert( const StringName &inName,
                                    Variant &outProperty ) const
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
