cmake_minimum_required(VERSION 3.0)

project(MP4v2 VERSION 2.1.1)

set(CMAKE_CXX_STANDARD 11)

option(BUILD_SHARED "Build libmp4v2 as a shared library" ON)
option(BUILD_UTILS "Build MP4v2 auxiliary tools" ON)

#
# Generate include/mp4v2/project.h and libplatform/config.h
#
include(CheckIncludeFiles)
include(CheckTypeSize)

check_include_files(inttypes.h  HAVE_INTTYPES_H)
check_include_files(stdint.h    HAVE_STDINT_H)
check_include_files(stdio.h     HAVE_STDIO_H)
check_include_files(stdlib.h    HAVE_STDLIB_H)
check_include_files(string.h    HAVE_STRING_H)
check_include_files(sys/stat.h  HAVE_SYS_STAT_H)
check_include_files(sys/types.h HAVE_SYS_TYPES_H)
check_include_files(unistd.h    HAVE_UNISTD_H)

if(NOT WIN32 AND HAVE_SYS_STAT_H)
    set(CMAKE_EXTRA_INCLUDE_FILES sys/stat.h)
    check_type_size("((struct stat*)0)->st_size" ST_SIZE_BYTES)
    if(${ST_SIZE_BYTES} LESS 8)
        set(NEED_LFS_ACTIVATION 1)
    endif()
endif()

file(READ project/project.m4sugar PROJECT_INFO)

macro(projectinfo variable output)
    string(REGEX MATCH "\\[PRJ_${variable}\\],[^\n]*\\[([^\n]*)\\]" _ ${PROJECT_INFO})
    set(${output} ${CMAKE_MATCH_1})
endmacro()

projectinfo(name           PROJECT_name)
projectinfo(name_lower     PROJECT_name_lower)
projectinfo(name_upper     PROJECT_name_upper)
projectinfo(url_website    PROJECT_url_website)
projectinfo(url_downloads  PROJECT_url_downloads)
projectinfo(url_discussion PROJECT_url_discussion)
projectinfo(url_bugreport  PROJECT_url_bugreport)
projectinfo(support        PROJECT_email)
projectinfo(version_major  PROJECT_version_major)
projectinfo(version_minor  PROJECT_version_minor)
projectinfo(version_point  PROJECT_version_point)
projectinfo(repo_url       PROJECT_repo_url)

set(PROJECT_version        "${PROJECT_version_major}.${PROJECT_version_minor}.${PROJECT_version_point}")
set(PROJECT_name_formal    "${PROJECT_name} ${PROJECT_version}")
set(PROJECT_support        "<${PROJECT_email}>")

function(number_to_hex number output)
    set(chars "0123456789abcdef")
    set(hex "")

    foreach(i RANGE 7)
        math(EXPR nibble "${number} & 15")
        string(SUBSTRING "${chars}" "${nibble}" 1 nibble_hex)
        string(CONCAT hex "${nibble_hex}" "${hex}")

        math(EXPR number "${number} >> 4")
        if(${number} EQUAL 0)
            break()
        endif()
    endforeach()

    set("${output}" "0x${hex}" PARENT_SCOPE)
endfunction()

math(EXPR PROJECT_version_dec "${PROJECT_version_major} << 16 | ${PROJECT_version_minor} << 8 | ${PROJECT_version_point}")
number_to_hex("${PROJECT_version_dec}" PROJECT_version_hex)

string(TIMESTAMP PROJECT_build "%Y-%m-%d %H:%M:%S UTC" UTC)

configure_file(libplatform/config.h.cmake libplatform/config.h)
configure_file(include/mp4v2/project.h.in include/mp4v2/project.h)

#
# Configure pkg-config file
#
function(configure_pkg_config_file pkg_config_file_in)
    set(prefix ${CMAKE_INSTALL_PREFIX})
    set(exec_prefix \${prefix})
    set(libdir \${prefix}/${CMAKE_INSTALL_LIBDIR})
    set(includedir \${prefix}/${CMAKE_INSTALL_INCLUDEDIR})
    set(VERSION ${PROJECT_version})
    string(REPLACE ".in" "" pkg_config_file ${pkg_config_file_in})
    configure_file(${pkg_config_file_in} ${pkg_config_file} @ONLY)
endfunction() 

configure_pkg_config_file(mp4v2.pc.in)

#
# Define MP4v2 source files
#
set(MP4V2_PUBLIC_HEADERS
        include/mp4v2/chapter.h
        include/mp4v2/file.h
        include/mp4v2/file_prop.h
        include/mp4v2/general.h
        include/mp4v2/isma.h
        include/mp4v2/itmf_generic.h
        include/mp4v2/itmf_tags.h
        include/mp4v2/mp4v2.h
        include/mp4v2/platform.h
        include/mp4v2/project.h
        include/mp4v2/sample.h
        include/mp4v2/streaming.h
        include/mp4v2/track.h
        include/mp4v2/track_prop.h)

set(MP4V2_PRIVATE_HEADERS
        libplatform/io/File.h
        libplatform/io/FileSystem.h
        libplatform/number/random.h
        libplatform/prog/option.h
        libplatform/sys/error.h
        libplatform/time/time.h
        libplatform/config.h
        libplatform/endian.h
        libplatform/impl.h
        libplatform/platform.h
        libplatform/platform_base.h
        libplatform/platform_posix.h
        libplatform/platform_win32.h
        libplatform/warning.h
        libutil/impl.h
        libutil/other.h
        libutil/TrackModifier.h
        libutil/util.h
        libutil/Utility.h
        src/bmff/bmff.h
        src/bmff/impl.h
        src/bmff/typebmff.h
        src/itmf/CoverArtBox.h
        src/itmf/generic.h
        src/itmf/impl.h
        src/itmf/itmf.h
        src/itmf/Tags.h
        src/itmf/type.h
        src/qtff/coding.h
        src/qtff/ColorParameterBox.h
        src/qtff/impl.h
        src/qtff/PictureAspectRatioBox.h
        src/qtff/qtff.h
        src/atoms.h
        src/descriptors.h
        src/enum.h
        src/exception.h
        src/impl.h
        src/log.h
        src/mp4array.h
        src/mp4atom.h
        src/mp4container.h
        src/mp4descriptor.h
        src/mp4file.h
        src/mp4property.h
        src/mp4track.h
        src/mp4util.h
        src/ocidescriptors.h
        src/odcommands.h
        src/qosqualifiers.h
        src/rtphint.h
        src/src.h
        src/text.h
        src/util.h)

if(WIN32)
   set(MP4V2_OSSPEC_SRC
        libplatform/io/File_win32.cpp
        libplatform/io/FileSystem_win32.cpp
        libplatform/number/random_win32.cpp
        libplatform/time/time_win32.cpp
        libplatform/platform_win32.cpp
   )
else()
   set(MP4V2_OSSPEC_SRC
        libplatform/io/File_posix.cpp
        libplatform/io/FileSystem_posix.cpp
        libplatform/number/random_posix.cpp
        libplatform/time/time_posix.cpp
       )
endif()

set(MP4V2_SOURCE_FILES
        ${MP4V2_OSSPEC_SRC}
        libplatform/io/File.cpp
        libplatform/io/FileSystem.cpp
        libplatform/prog/option.cpp
        libplatform/sys/error.cpp
        libplatform/time/time.cpp
        libutil/other.cpp
        libutil/TrackModifier.cpp
        libutil/Utility.cpp
        src/bmff/typebmff.cpp
        src/itmf/CoverArtBox.cpp
        src/itmf/generic.cpp
        src/itmf/Tags.cpp
        src/itmf/type.cpp
        src/qtff/coding.cpp
        src/qtff/ColorParameterBox.cpp
        src/qtff/PictureAspectRatioBox.cpp
        src/3gp.cpp
        src/atom_ac3.cpp
        src/atom_amr.cpp
        src/atom_avc1.cpp
        src/atom_avcC.cpp
        src/atom_chpl.cpp
        src/atom_colr.cpp
        src/atom_d263.cpp
        src/atom_dac3.cpp
        src/atom_damr.cpp
        src/atom_dref.cpp
        src/atom_elst.cpp
        src/atom_enca.cpp
        src/atom_encv.cpp
        src/atom_free.cpp
        src/atom_ftab.cpp
        src/atom_ftyp.cpp
        src/atom_gmin.cpp
        src/atom_hdlr.cpp
        src/atom_hinf.cpp
        src/atom_hnti.cpp
        src/atom_href.cpp
        src/atom_mdat.cpp
        src/atom_mdhd.cpp
        src/atom_meta.cpp
        src/atom_mp4s.cpp
        src/atom_mp4v.cpp
        src/atom_mvhd.cpp
        src/atom_nmhd.cpp
        src/atom_ohdr.cpp
        src/atom_pasp.cpp
        src/atom_root.cpp
        src/atom_rtp.cpp
        src/atom_s263.cpp
        src/atom_sdp.cpp
        src/atom_sdtp.cpp
        src/atom_smi.cpp
        src/atom_sound.cpp
        src/atom_standard.cpp
        src/atom_stbl.cpp
        src/atom_stdp.cpp
        src/atom_stsc.cpp
        src/atom_stsd.cpp
        src/atom_stsz.cpp
        src/atom_stz2.cpp
        src/atom_text.cpp
        src/atom_tfhd.cpp
        src/atom_tkhd.cpp
        src/atom_treftype.cpp
        src/atom_trun.cpp
        src/atom_tx3g.cpp
        src/atom_udta.cpp
        src/atom_url.cpp
        src/atom_urn.cpp
        src/atom_uuid.cpp
        src/atom_video.cpp
        src/atom_vmhd.cpp
        src/cmeta.cpp
        src/descriptors.cpp
        src/enum.tcc
        src/exception.cpp
        src/isma.cpp
        src/log.cpp
        src/mp4.cpp
        src/mp4atom.cpp
        src/mp4container.cpp
        src/mp4descriptor.cpp
        src/mp4file.cpp
        src/mp4file_io.cpp
        src/mp4info.cpp
        src/mp4property.cpp
        src/mp4track.cpp
        src/mp4util.cpp
        src/ocidescriptors.cpp
        src/odcommands.cpp
        src/qosqualifiers.cpp
        src/rtphint.cpp
        src/text.cpp)

#
# Define libutil source files
#
set(LIBUTIL_HEADER_FILES
        libutil/crc.h
        libutil/Timecode.h)

set(LIBUTIL_SOURCE_FILES
        libutil/crc.cpp
        libutil/Timecode.cpp)

#
# Define library targets
#
if(BUILD_SHARED)
    add_library(mp4v2 SHARED ${MP4V2_SOURCE_FILES})
    target_compile_definitions(mp4v2 PRIVATE MP4V2_EXPORTS)
    set_target_properties(mp4v2 PROPERTIES VERSION ${PROJECT_version} SOVERSION ${PROJECT_version_major})
else()
    add_library(mp4v2 STATIC ${MP4V2_SOURCE_FILES})
    target_compile_definitions(mp4v2 PUBLIC MP4V2_USE_STATIC_LIB)
endif()

#
# Set include folders
#
target_include_directories(mp4v2 PUBLIC
   ${CMAKE_CURRENT_BINARY_DIR}/include
   ${CMAKE_CURRENT_SOURCE_DIR}/include
   ${CMAKE_CURRENT_BINARY_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR})

#
# Define utilities targets
#
if(BUILD_UTILS)
    set(UTILITY_HEADER_FILES
       util/impl.h)

    add_library(util STATIC ${LIBUTIL_HEADER_FILES} ${LIBUTIL_SOURCE_FILES})
    target_link_libraries(util mp4v2)

    add_executable(mp4art ${UTILITY_HEADER_FILES} util/mp4art.cpp)
    target_link_libraries(mp4art mp4v2 util)

    add_executable(mp4chaps ${UTILITY_HEADER_FILES} util/mp4chaps.cpp)
    target_link_libraries(mp4chaps mp4v2 util)

    add_executable(mp4extract ${UTILITY_HEADER_FILES} util/mp4extract.cpp)
    target_link_libraries(mp4extract mp4v2)

    add_executable(mp4file ${UTILITY_HEADER_FILES} util/mp4file.cpp)
    target_link_libraries(mp4file mp4v2)

    add_executable(mp4info ${UTILITY_HEADER_FILES} util/mp4info.cpp)
    target_link_libraries(mp4info mp4v2)

    add_executable(mp4subtitle ${UTILITY_HEADER_FILES} util/mp4subtitle.cpp)
    target_link_libraries(mp4subtitle mp4v2)

    add_executable(mp4tags ${UTILITY_HEADER_FILES} util/mp4tags.cpp)
    target_link_libraries(mp4tags mp4v2)

    add_executable(mp4track ${UTILITY_HEADER_FILES} util/mp4track.cpp)
    target_link_libraries(mp4track mp4v2)

    add_executable(mp4trackdump ${UTILITY_HEADER_FILES} util/mp4trackdump.cpp)
    target_link_libraries(mp4trackdump mp4v2)
endif()

#
# Define install targets
#
include(GNUInstallDirs)

install(FILES ${MP4V2_PUBLIC_HEADERS} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/mp4v2")

if(BUILD_SHARED)
    install(TARGETS mp4v2 LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")
else()
    install(TARGETS mp4v2 ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}")
endif()

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/mp4v2.pc" DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

if(BUILD_UTILS)
    install(TARGETS mp4art
                    mp4chaps
                    mp4extract
                    mp4file
                    mp4info
                    mp4subtitle
                    mp4tags
                    mp4track
                    mp4trackdump RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
endif()

