// Copied from godot-cpp/test/src and modified.

#include "godot_cpp/classes/global_constants.hpp"
#include "godot_cpp/classes/label.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/variant/utility_functions.hpp"

#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <limits.h>

#include "OpenCVGodot.h"

// Used to mark unused parameters to indicate intent and suppress warnings.
#define UNUSED( expr ) (void)( expr )

namespace
{
    constexpr int MAGIC_NUMBER = 42;

    class MyCallableCustom : public godot::CallableCustom
    {
    public:
        virtual uint32_t hash() const
        {
            return 27;
        }

        virtual godot::String get_as_text() const
        {
            return "<MyCallableCustom>";
        }

        static bool compare_equal_func( const CallableCustom *inA, const CallableCustom *inB )
        {
            return inA == inB;
        }

        virtual CompareEqualFunc get_compare_equal_func() const
        {
            return &MyCallableCustom::compare_equal_func;
        }

        static bool compare_less_func( const CallableCustom *inA, const CallableCustom *inB )
        {
            return reinterpret_cast<const void *>( inA ) < reinterpret_cast<const void *>( inB );
        }

        virtual CompareLessFunc get_compare_less_func() const
        {
            return &MyCallableCustom::compare_less_func;
        }

        bool is_valid() const
        {
            return true;
        }

        virtual godot::ObjectID get_object() const
        {
            return godot::ObjectID();
        }

        virtual void call( const godot::Variant **inArguments, int inArgcount,
                           godot::Variant &outReturnValue,
                           GDExtensionCallError &outCallError ) const
        {
            UNUSED( inArguments );
            UNUSED( inArgcount );

            outReturnValue = "Hi";
            outCallError.error = GDEXTENSION_CALL_OK;
        }
    };
}


OpenCVGodot::OpenCVGodot()
{
    godot::UtilityFunctions::print( "Constructor." );
}

OpenCVGodot::~OpenCVGodot()
{
    godot::UtilityFunctions::print( "Destructor." );
}

// Methods.
void OpenCVGodot::simpleFunc()
{
    godot::UtilityFunctions::print( "  Simple func called." );
}

void OpenCVGodot::emitCustomSignal( const godot::String &inName, int inValue )
{
    emit_signal( "custom_signal", inName, inValue );
}

// Static methods
int OpenCVGodot::testStatic( godot::String path)
{
	cv::Mat image;
 	image = imread( "path", cv::IMREAD_COLOR );

	godot::UtilityFunctions::print( "Dims" );
	godot::UtilityFunctions::print( image.dims );

	char pBuf[256];
	size_t len = sizeof(pBuf); 

	godot::UtilityFunctions::print( "pBuf" );
	godot::UtilityFunctions::print( pBuf );

	int bytes = MIN(readlink("/proc/self/exe", pBuf, len), len - 1);
	if(bytes >= 0)
		pBuf[bytes] = '\0';

	godot::UtilityFunctions::print( "bytes" );
	godot::UtilityFunctions::print( bytes );

    return bytes;
}

void OpenCVGodot::_bind_methods()
{
    // Methods.
    godot::ClassDB::bind_method( godot::D_METHOD( "simple_func" ), &OpenCVGodot::simpleFunc );
    godot::ClassDB::bind_static_method( "OpenCVGodot", godot::D_METHOD( "test_static", "path" ),
                                        &OpenCVGodot::testStatic );




    // Signals.
    ADD_SIGNAL( godot::MethodInfo( "custom_signal",
                                   godot::PropertyInfo( godot::Variant::STRING, "name" ),
                                   godot::PropertyInfo( godot::Variant::INT, "value" ) ) );
    godot::ClassDB::bind_method( godot::D_METHOD( "emit_custom_signal", "name", "value" ),
                                 &OpenCVGodot::emitCustomSignal );
}

void OpenCVGodot::_notification( int inWhat )
{
    //godot::UtilityFunctions::print( "Notification: ", godot::String::num( inWhat ) );
}

bool OpenCVGodot::_set( const godot::StringName &inName, const godot::Variant &inValue )
{
    godot::String name = inName;

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

bool OpenCVGodot::_get( const godot::StringName &inName, godot::Variant &outReturn ) const
{
    godot::String name = inName;

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

void OpenCVGodot::_get_property_list( godot::List<godot::PropertyInfo> *outList ) const
{
    outList->push_back( godot::PropertyInfo( godot::Variant::VECTOR3, "property_from_list" ) );

    for ( int i = 0; i < 3; ++i )
    {
        outList->push_back(
            godot::PropertyInfo( godot::Variant::VECTOR2, "dproperty_" + godot::itos( i ) ) );
    }
}

bool OpenCVGodot::_property_can_revert( const godot::StringName &inName ) const
{
    if ( inName == godot::StringName( "property_from_list" ) &&
         mPropertyFromList != godot::Vector3( MAGIC_NUMBER, MAGIC_NUMBER, MAGIC_NUMBER ) )
    {
        return true;
    }

    return false;
}

bool OpenCVGodot::_property_get_revert( const godot::StringName &inName,
                                    godot::Variant &outProperty ) const
{
    if ( inName == godot::StringName( "property_from_list" ) )
    {
        outProperty = godot::Vector3( MAGIC_NUMBER, MAGIC_NUMBER, MAGIC_NUMBER );

        return true;
    }

    return false;
}

void OpenCVGodot::_validate_property( godot::PropertyInfo &inProperty ) const
{
    godot::String name = inProperty.name;

    // Test hiding the "mouse_filter" property from the editor.
    if ( name == "mouse_filter" )
    {
        inProperty.usage = godot::PROPERTY_USAGE_NO_EDITOR;
    }
}

godot::String OpenCVGodot::_to_string() const
{
    return "[ GDExtension::OpenCVGodot <--> Instance ID:" + godot::uitos( get_instance_id() ) + " ]";
}
