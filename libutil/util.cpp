#include "impl.h"

namespace mp4v2 { namespace util {

///////////////////////////////////////////////////////////////////////////////

namespace itmf {
    void type_sinit();
    void type_sshutdown();
}

namespace qtff {
    void ColorParameterBox_sinit();
    void ColorParameterBox_sshutdown();

    void PictureAspectRatioBox_sinit();
    void PictureAspectRatioBox_sshutdown();

    void coding_sinit();
    void coding_sshutdown();
}

///////////////////////////////////////////////////////////////////////////////

/// Implementation notes.
/// All C++ classes that are static require ordered initialization.
/// This namespace and all child namespaces must from this function.
///
void
sinit()
{
    itmf::type_sinit();
    qtff::coding_sinit();
    qtff::ColorParameterBox_sinit();
    qtff::PictureAspectRatioBox_sinit();
}

///////////////////////////////////////////////////////////////////////////////

/// Implementation notes.
/// Counterpart to sinit; destroy objects here.
///
void
sshutdown()
{
    qtff::PictureAspectRatioBox_sshutdown();
    qtff::ColorParameterBox_sshutdown();
    qtff::coding_sshutdown();
    itmf::type_sshutdown();
}

///////////////////////////////////////////////////////////////////////////////

}} // namespace mp4v2::util
