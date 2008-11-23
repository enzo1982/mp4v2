/** @mainpage MP4v2 API Documentation

The MP4v2 library provides an API to create and modify mp4 files as
defined by ISO-IEC:14496-1:2001 MPEG-4 Systems. This file format is
derived from Apple's QuickTime file format that has been used as a
multimedia file format in a variety of platforms and applications.
It is a very powerful and extensible format that can accomodate practic
ally any type of media.

The basic structure of an mp4 file is that the file is a container for one
or more tracks. These tracks contain one type of media, such as audio or
video. Each track has its own timeline, samples, and properties. An example
of a sample is a frame of video. The file describes how to synchronize the
timelines of the tracks and the aggregate properties of the tracks.

The MP4v2 library is focussed on providing an easy to use API for the mp4
file format. It has been used with an encoder, a server, a player, and a
number of mp4 utilities. However, it may not be adequate for multimedia
editors that wish to work directly with mp4 files. It can be used by these
type of tools to export an mp4 file.

In providing a easy to use API not all the information in the mp4 file is
directly exposed via the API. To accomodate applications that need access
to information not othe rwise available via the API there are file and
track level generic get and set property routines that use arbitary string
property names. To use these routines you will need to be familar with the
mp4 file specification or be willing to wade through the output of
MP4Dump() to determine what you want. See MP4GetIntegerProperty() for more
etails.

@section invocation Invocation

The public libary API is defined in @c <mp4v2/mp4v2.h> which includes all the
necessary dependent header files. You should never use any other header
files from MP4v2 sources.

The MP4v2 library can be used by either C or C++ programs. The calling
convention is C but for convenience if C++ is used default function arguments
have been added where appropriate:

@code
MP4Create( "foo.mp4", 0, 0, 0 ); /* OK in C++ and C */
MP4Create( "foo.mp4" ); /* OK in C++, ERROR in C */
@endcode

@section examples Examples

See @c util/ and @c unit/ for simple programs that use this library.
*/
