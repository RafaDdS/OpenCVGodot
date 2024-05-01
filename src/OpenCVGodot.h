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
    ~Mat();

    Ref<Image> image;
    Ref<Image> get_image();
    void set_image( Ref<Image> _image );

    int get_rows();
    int get_cols();

    cv::Mat get_mat();
    void set_mat( cv::Mat _mat );

    void convert_to( int rtype );

    cv::Mat mat;
};

class OpenCVGodot : public Node
{
    GDCLASS( OpenCVGodot, Node )

public:
    OpenCVGodot();
    ~OpenCVGodot() override;

    // Helper methods.
    static Ref<Mat> take_picture();

    // Core
    static Ref<Mat> add( Ref<Mat> mat1, Ref<Mat> mat2, Ref<Mat> mask, int dtype );
    static Ref<Mat> subtract( Ref<Mat> mat1, Ref<Mat> mat2, Ref<Mat> mask, int dtype );
    static Ref<Mat> max( Ref<Mat> mat1, Ref<Mat> mat2 );
    static Ref<Mat> min( Ref<Mat> mat1, Ref<Mat> mat2 );
    static Ref<Mat> absdiff( Ref<Mat> mat1, Ref<Mat> mat2 );
    static Ref<Mat> vconcat( Ref<Mat> mat1, Ref<Mat> mat2 );
    static Ref<Mat> hconcat( Ref<Mat> mat1, Ref<Mat> mat2 );
    static Ref<Mat> bitwise_and( Ref<Mat> mat1, Ref<Mat> mat2, Ref<Mat> mask );
    static Ref<Mat> bitwise_or( Ref<Mat> mat1, Ref<Mat> mat2, Ref<Mat> mask );
    static Ref<Mat> bitwise_xor( Ref<Mat> mat1, Ref<Mat> mat2, Ref<Mat> mask );
    static Ref<Mat> bitwise_not( Ref<Mat> mat, Ref<Mat> mask );
    static Ref<Mat> convertFp16( Ref<Mat> mat );
    static Ref<Mat> exp( Ref<Mat> mat );
    static Ref<Mat> log( Ref<Mat> mat );
    static Ref<Mat> sqrt( Ref<Mat> mat );
    static Ref<Mat> transpose( Ref<Mat> mat );

    // Imgcodecs
    static Ref<Mat> imread( String filename );

protected:
    static void _bind_methods();

    static Ref<Mat> arithmetic_wrapper( void ( *func )( cv::InputArray, cv::InputArray,
                                                        cv::OutputArray, cv::InputArray, int ),
                                        Ref<Mat> mat1, Ref<Mat> mat2, Ref<Mat> mask, int dtype );
    static Ref<Mat> mat_in_mat_in_mat_out_wrapper( void ( *func )( cv::InputArray, cv::InputArray,
                                                                   cv::OutputArray ),
                                                   Ref<Mat> mat1, Ref<Mat> mat2 );
    static Ref<Mat> bitwise_wrapper( void ( *func )( cv::InputArray, cv::InputArray,
                                                     cv::OutputArray, cv::InputArray ),
                                     Ref<Mat> mat1, Ref<Mat> mat2, Ref<Mat> mask );
    static Ref<Mat> mat_in_mat_out_wrapper( void ( *func )( cv::InputArray, cv::OutputArray ),
                                            Ref<Mat> mat );
};
