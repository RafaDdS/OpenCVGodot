#pragma once
// Copied from godot-cpp/test/src and modified.

#include "godot_cpp/classes/control.hpp"
#include "godot_cpp/classes/global_constants.hpp"
#include "godot_cpp/classes/tile_map.hpp"
#include "godot_cpp/classes/tile_set.hpp"
#include "godot_cpp/classes/viewport.hpp"
#include "godot_cpp/core/binder_common.hpp"
#include "godot_cpp/variant/variant.hpp"

#include "godot_cpp/classes/image.hpp"
#include <godot_cpp/classes/object.hpp>
#include <opencv2/opencv.hpp>

using namespace godot;

class Mat : public RefCounted
{
    GDCLASS( Mat, RefCounted )

protected:
    static void _bind_methods();

public:
    Mat();
    Mat( String path );
    ~Mat();

    Ref<Image> image;
    Ref<Image> get_image();
    void set_image( Ref<Image> _image );

    cv::Mat get_mat();
    void set_mat( cv::Mat _mat );

    void content();

    cv::Mat mat;
};

class OpenCVGodot : public Node
{
    GDCLASS( OpenCVGodot, Node )

public:
    OpenCVGodot();
    ~OpenCVGodot() override;

    // Added methods.
    static Ref<Mat> takePicture();

    // Core
    static Ref<Mat> add( Ref<Mat> mat1, Ref<Mat> mat2, Ref<Mat> mask, int dtype );
    static Ref<Mat> subtract( Ref<Mat> mat1, Ref<Mat> mat2, Ref<Mat> mask, int dtype );
    static Ref<Mat> max( Ref<Mat> mat1, Ref<Mat> mat2 );
	static Ref<Mat> min( Ref<Mat> mat1, Ref<Mat> mat2 );
	static Ref<Mat> absdiff( Ref<Mat> mat1, Ref<Mat> mat2 );
	static Ref<Mat> vconcat( Ref<Mat> mat1, Ref<Mat> mat2 );
	static Ref<Mat> hconcat( Ref<Mat> mat1, Ref<Mat> mat2 );

protected:
    static void _bind_methods();

    static Ref<Mat> arithmetic_wrapper( void ( *func )( cv::InputArray, cv::InputArray,
                                                        cv::OutputArray, cv::InputArray, int ),
                                        Ref<Mat> mat1, Ref<Mat> mat2, Ref<Mat> mask, int dtype );
	static Ref<Mat> mat_in_mat_in_mat_out_wrapper( void ( *func )( cv::InputArray, cv::InputArray,
                                                        cv::OutputArray ),
                                        Ref<Mat> mat1, Ref<Mat> mat2 );
};
