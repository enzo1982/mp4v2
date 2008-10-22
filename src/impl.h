#ifndef MP4V2_IMPL_IMPL_H
#define MP4V2_IMPL_IMPL_H

///////////////////////////////////////////////////////////////////////////////

#include "platform/platform.h"
#include <mp4v2/mp4v2.h>

// we are forced to bring platform because of inline funcs in headers
// TODO: uninline those silly functions as it dirties up everything
#ifndef HAVE_GETTIMEOFDAY
namespace mp4v2 {
namespace impl {
using namespace mp4v2::platform;
}
} // namespace mp4v2::impl
#endif

#include "type.h"
#include "util.h"
#include "mp4util.h"
#include "mp4array.h"
#include "mp4track.h"
#include "mp4file.h"
#include "mp4property.h"
#include "mp4container.h"

#include "mp4atom.h"
#include "atoms.h"

#include "mp4descriptor.h"
#include "descriptors.h"
#include "ocidescriptors.h"

#include "qosqualifiers.h"
#include "odcommands.h"
#include "rtphint.h"
#include "virtual_io.h"

///////////////////////////////////////////////////////////////////////////////

#endif // MP4V2_IMPL_IMPL_H
