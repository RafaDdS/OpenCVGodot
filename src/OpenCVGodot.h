#pragma once
// Copied from godot-cpp/test/src and modified.

#include "godot_cpp/classes/control.hpp"
#include "godot_cpp/classes/global_constants.hpp"
#include "godot_cpp/classes/tile_map.hpp"
#include "godot_cpp/classes/tile_set.hpp"
#include "godot_cpp/classes/viewport.hpp"
#include "godot_cpp/core/binder_common.hpp"
#include "godot_cpp/variant/variant.hpp"

class OpenCVGodot : public godot::Node
{
    GDCLASS( OpenCVGodot, godot::Node )

public:

    OpenCVGodot();
    ~OpenCVGodot() override;

    // Functions.
    void simpleFunc();
    
    void emitCustomSignal( const godot::String &inName, int inValue );

    // Static method.
    static int testStatic( godot::String path );

protected:
    static void _bind_methods();

    void _notification( int inWhat );
    bool _set( const godot::StringName &inName, const godot::Variant &inValue );
    bool _get( const godot::StringName &inName, godot::Variant &outReturn ) const;
    void _get_property_list( godot::List<godot::PropertyInfo> *outList ) const;
    bool _property_can_revert( const godot::StringName &inName ) const;
    bool _property_get_revert( const godot::StringName &inName, godot::Variant &outProperty ) const;
    void _validate_property( godot::PropertyInfo &inProperty ) const;

    godot::String _to_string() const;

private:
    godot::Vector2 mCustomPosition;
    godot::Vector3 mPropertyFromList;
    godot::Vector2 mDProp[3];
};

