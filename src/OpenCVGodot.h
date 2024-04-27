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

class Mat : public RefCounted {
    GDCLASS(Mat, RefCounted)

    protected:
    	static void _bind_methods();

    public:
		Mat();
		Mat(String path);
		~Mat();

		Ref<Image> image;
		Ref<Image> get_image();
		void set_image(Ref<Image> _image);

		cv::Mat get_mat();
		void set_mat(cv::Mat _mat);

		void content();

		cv::Mat mat;
};


class OpenCVGodot : public Node
{
    GDCLASS( OpenCVGodot, Node )

public:

    OpenCVGodot();
    ~OpenCVGodot() override;

	
	static Ref<Mat> subtract(Ref<Mat> mat1, Ref<Mat> mat2);
    
    void emitCustomSignal( const String &inName, int inValue );

    // Static method.
    static Ref<Mat> takePicture( );

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



