#pragma once
// Copied from godot-cpp/test/src and modified.

#include "godot_cpp/classes/control.hpp"
#include "godot_cpp/classes/global_constants.hpp"
#include "godot_cpp/classes/tile_map.hpp"
#include "godot_cpp/classes/tile_set.hpp"
#include "godot_cpp/classes/viewport.hpp"
#include "godot_cpp/core/binder_common.hpp"
#include "godot_cpp/variant/variant.hpp"

#include <godot_cpp/classes/object.hpp>
#include "godot_cpp/classes/image.hpp"
#include <opencv2/opencv.hpp>

using namespace godot;

class Mat : public Object {
    GDCLASS(Mat, Object)

    protected:
    	static void _bind_methods();

    public:
		Mat();
		~Mat();
		Ref<Image> image;
		Ref<Image> getImage();
		void setImage(Ref<Image> _image);

		void content();

		cv::Mat data;
};


class OpenCVGodot : public Node
{
    GDCLASS( OpenCVGodot, Node )

public:

    OpenCVGodot();
    ~OpenCVGodot() override;

    // Functions.
    void simpleFunc();
    
    void emitCustomSignal( const String &inName, int inValue );

    // Static method.
    static Ref<Image> takePicture( );

protected:
    static void _bind_methods();

    void _notification( int inWhat );
    bool _set( const StringName &inName, const Variant &inValue );
    bool _get( const StringName &inName, Variant &outReturn ) const;
    void _get_property_list( List<PropertyInfo> *outList ) const;
    bool _property_can_revert( const StringName &inName ) const;
    bool _property_get_revert( const StringName &inName, Variant &outProperty ) const;
    void _validate_property( PropertyInfo &inProperty ) const;

    String _to_string() const;

private:
    Vector2 mCustomPosition;
    Vector3 mPropertyFromList;
    Vector2 mDProp[3];
};



