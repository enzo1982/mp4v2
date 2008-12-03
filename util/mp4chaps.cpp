/* mp4chaps -- tool to set iTunes-compatible chapter markers from Nero chapter markers
 *
 * The contents of this file are subject to the Mozilla Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 */

#include "libutil/util.h"

namespace mp4v2 { namespace util {

/* One-letter options -- if you want to rearrange these, change them
   here, immediately below in OPT_STRING, and in the help text. */
#define OPT_HELP     'h'
#define OPT_VERSION  'V'
#define OPT_VERBOSE  'v'
#define OPT_REMOVE   'r'
#define OPT_CONVERT  'c'
#define OPT_NERO     'N'
#define OPT_OPTIMIZE 'o'
#define OPT_QT       'Q'
#define OPT_EVERY    'e'
#define OPT_LIST     'l'
#define OPT_STRING   "hVv::rcNoQe:l"

static const char* help_text =
    "OPTION... FILE...\n"
    "\nConvert/Manage Nero chapter markers or QT/iTunes/iPod\n"
    "  chapter markers in MP4 Audio files\n"
    "\n"
    "  -h, -help             Display this help text and exit\n"
    "  -V, -version          Display version information and exit\n"
    "  -v, -verbose <level>  enable verbose processing output\n"
    "  -r, -remove           Remove chapter markers\n"
    "  -c, -convert          Convert chapter markers\n"
    "  -o, -optimize         re-write file in optimized form\n"
    "  -N, -Nero             write/remove/convert to Nero chapter markers\n"
    "  -Q, -QuickTime        write/remove/convert to QuickTime chapter markers\n"
    "  -e, -every n          creates chapter markers every n seconds\n"
    "  -l, -list             list available chapter markers\n"
    "\n"
    "Either -Q or -N can be given. To write or delete both do not give any of them.\n";

char* ProgName;

// function prototypes
void ListChapters(MP4FileHandle hFile, MP4ChapterType fromChapterType);
int CreateChaptersEvery(MP4FileHandle hFile, int seconds, const char * filename, MP4ChapterType toChapterType);


extern "C" int
    main(int argc, char** argv)
{
    static const prog::Option long_options[] =
    {
        { "help",      prog::Option::NO_ARG,       0, OPT_HELP     },
        { "verbose",   prog::Option::OPTIONAL_ARG, 0, OPT_VERBOSE  },
        { "version",   prog::Option::NO_ARG,       0, OPT_VERSION  },
        { "remove",    prog::Option::NO_ARG,       0, OPT_REMOVE   },
        { "convert",   prog::Option::NO_ARG,       0, OPT_CONVERT  },
        { "optimize",  prog::Option::NO_ARG,       0, OPT_OPTIMIZE },
        { "Nero",      prog::Option::NO_ARG,       0, OPT_NERO     },
        { "QuickTime", prog::Option::NO_ARG,       0, OPT_QT       },
        { "every",     prog::Option::REQUIRED_ARG, 0, OPT_EVERY    },
        { "list",      prog::Option::NO_ARG,       0, OPT_LIST     },
        { NULL,        prog::Option::NO_ARG,       0, 0            }
    };


    bool optEvery = false;
    bool optDelete = false;
    bool optConvert = false;
    bool optOptimize = false;
    bool optList = false;
    bool needsModification = false;
    MP4ChapterType chapterType = MP4ChapterTypeAny;
    uint32_t seconds = 0;
    uint32_t verbosity = MP4_DETAILS_ERROR | MP4_DETAILS_WARNING;

    ProgName = argv[0];

    /* Option-processing loop. */
    int c = prog::getOptionSingle(argc, argv, OPT_STRING, long_options, NULL);
    while (c != -1)
    {
        switch (c)
        {
            /* getopt() returns '?' if there was an error.  It already
            printed the error message, so just return. */
        case '?':
            return 1;

            /* Help and version requests handled here. */
        case OPT_HELP:
            fprintf(stderr, "usage %s %s", argv[0], help_text);
            return 0;
        case OPT_VERSION:
            fprintf(stderr, "%s - %s\n", ProgName, MP4V2_PROJECT_name_formal);
            return 0;

        case OPT_VERBOSE:
            verbosity |= MP4_DETAILS_READ | MP4_DETAILS_WRITE;
            if ( prog::optarg ) {
                uint32_t level;
                if (sscanf( prog::optarg, "%u", &level) == 1)
                {
                    if (0 == level)
                    {
                        verbosity = level;
                    }
                    else
                    {
                        if (level >= 2)
                        {
                            verbosity |= MP4_DETAILS_TABLE;
                        }
                        if (level >= 3)
                        {
                            verbosity |= MP4_DETAILS_SAMPLE;
                        }
                        if (level >= 4)
                        {
                            verbosity = MP4_DETAILS_ALL;
                        }
                    }
                }
            }
            break;
        case OPT_NERO:
            if (MP4ChapterTypeAny != chapterType)
            {
                fprintf(stderr, "usage %s %s", argv[0], help_text);
                return 0;
            }
            chapterType = MP4ChapterTypeNero;
            break;

        case OPT_QT:
            if (MP4ChapterTypeAny != chapterType)
            {
                fprintf(stderr, "usage %s %s", argv[0], help_text);
                return 0;
            }
            chapterType = MP4ChapterTypeQt;
            break;

        case OPT_REMOVE:
            optDelete = true;
            needsModification = true;
            break;

        case OPT_CONVERT:
            optConvert = true;
            needsModification = true;
            break;

        case OPT_OPTIMIZE:
            optOptimize = true;
            needsModification = true;
            break;

        case OPT_LIST:
            optList = true;
            break;

            /* Numeric arguments: convert them using sscanf(). */
        case OPT_EVERY:
        {
            int r = sscanf(prog::optarg, "%ul", &seconds);
            if (r < 1)
            {
                fprintf(stderr, "%s: option requires numeric argument -- %c\n",
                        argv[0], c);
                return 2;
            }
            optEvery = true;
            needsModification = true;
        }
        break;

        default:
            break;
        } /* end switch */

        c = prog::getOptionSingle(argc, argv, OPT_STRING, long_options, NULL);
    } /* end while */

    /* Check that we have at least one non-option argument */
    if ((argc - prog::optind) < 1)
    {
        fprintf(stderr,
                "%s: You must specify at least one MP4 file.\n",
                argv[0]);
        fprintf(stderr, "usage %s %s", argv[0], help_text);
        return 3;
    }

    if (verbosity > MP4_DETAILS_ERROR)
    {
        fprintf( stderr, "%s version %s\n", ProgName, MP4V2_PROJECT_version );
    }

    /* Loop through the non-option arguments, and do the requested chapter work */
    int rc = 0;
    bool needsOptimize = false;
    while (prog::optind < argc)
    {
        char *filename = argv[prog::optind++];

        MP4FileHandle h = 0;
        if (needsModification)
        {
            h = MP4Modify(filename, verbosity);
        }
        else
        {
            h = MP4Read(filename, verbosity);
        }

        if (h == MP4_INVALID_FILE_HANDLE)
        {
            fprintf(stderr, "Could not open '%s'... aborting\n", filename);
            return 5;
        }

        if (optDelete)
        {
            MP4DeleteChapters(h, chapterType);
        }

        if (optEvery)
        {
            rc = CreateChaptersEvery(h, seconds, filename, chapterType);
            if (0 != rc)
            {
                MP4Close(h);
                continue;
            }
            needsOptimize = true;
        }

        if (optConvert)
        {
            if (MP4ChapterTypeAny != chapterType)
            {
                MP4ConvertChapters(h, chapterType);
                needsOptimize = true;
            }
        }

        if (optList)
        {
            ListChapters(h, chapterType);
        }

        if (0 == rc)
        {
            if (optOptimize || needsOptimize)
            {
                // get around a QuickTime/iPod issue with storing the number of samples in a signed 32Bit value
                uint32_t timescale = MP4GetTimeScale(h);
                MP4Duration duration = MP4GetDuration(h);
                uint64_t sampleCount = duration * timescale;
                if (INT_MAX < sampleCount)
                {
                    // timescale too high, lower it
                    MP4ChangeMovieTimeScale(h, 1000);
                }
            }

            MP4Close(h);

            if (optOptimize || needsOptimize)
            {
                MP4Optimize(filename);
            }
        }
    } /* end while prog::optind < argc */

    return 0;
}

/*
 * format a duration to a readable format ("stolen" from gpac MP4Box)
 */
static char *format_duration(uint64_t dur, char *szDur)
{
    uint32_t h, m, s, ms;

    h = (uint32_t) (dur / 3600000);
    m = (uint32_t) (dur / 60000) - h * 60;
    s = (uint32_t) (dur / 1000) - h * 3600 - m * 60;
    ms = (uint32_t) (dur) - h * 3600000 - m * 60000 - s * 1000;
    if (h<=24)
    {
        sprintf(szDur, "%02d:%02d:%02d.%03d", h, m, s, ms);
    }
    else
    {
        uint32_t d = (uint32_t) (dur / 3600000 / 24);
        h = (uint32_t) (dur/3600000)-24*d;
        if (d<=365)
        {
            sprintf(szDur, "%d Days, %02d:%02d:%02d.%03d", d, h, m, s, ms);
        }
        else
        {
            uint32_t y=0;
            while (d>365)
            {
                y++;
                d-=365;
                if (y%4) d--;
            }
            sprintf(szDur, "%d Years %d Days, %02d:%02d:%02d.%03d", y, d, h, m, s, ms);
        }
    }
    return szDur;
}

/*
 * List chapter start time and title
 */
void ListChapters(MP4FileHandle hFile, MP4ChapterType fromChapterType)
{
    MP4Chapter_t * chapters = 0;
    uint32_t chapterCount = 0;
    char szDur[20] = {0};
    MP4Duration durationSum = 0;

    // get the list of chapters
    MP4ChapterType type = MP4GetChapters(hFile, &chapters, &chapterCount, fromChapterType);
    if (0 == chapterCount)
    {
        return;
    }

    // start output (in mp4box format)
    switch(type)
    {
    case MP4ChapterTypeQt:
        fprintf(stdout, "\nQuickTime");
        break;
    case MP4ChapterTypeNero:
        fprintf(stdout, "\nNero");
        break;
    default:
        fprintf(stdout, "\nUnknown");
    }
    fprintf(stdout, " Chapters:\n");

    for (uint32_t i = 0; i < chapterCount; ++i)
    {
        // get the tile
        const char * title = chapters[i].title;
        // format the start time
        const char * formattedDuration = format_duration(durationSum, szDur);

        // print the infos
        fprintf(stdout, "\tChapter #%u - %s - \"%s\"\n", i+1, formattedDuration, title);

        // add the duration of this chapter to the sum (is the start time of the next chapter)
        durationSum += chapters[i].duration;
    }

    // free up the memory
    MP4Free(chapters);
}

/*
 * Create Qt/Nero chapter markers with a duration of 'seconds' seconds
 */
int CreateChaptersEvery(MP4FileHandle hFile, int seconds, const char * filename, MP4ChapterType toChapterType)
{
    // get the audio track that will reference the chapter track
    MP4TrackId refTrackId = MP4FindTrackId(hFile, MP4_INVALID_TRACK_ID, MP4_AUDIO_TRACK_TYPE);
    if (! MP4_IS_VALID_TRACK_ID(refTrackId))
    {
        MP4Close(hFile);
        fprintf(stderr, "Could not find audio track in file '%s'... aborting\n", filename);
        return 6;
    }

    // get informations about the audio track
    MP4Duration refTrackDuration = MP4GetTrackDuration(hFile, refTrackId);
    uint32_t scale = MP4GetTrackTimeScale(hFile, refTrackId);

    // calculate the count of chapters (yes we do accept rounding errors)
    uint32_t playlength = refTrackDuration / scale;
    uint32_t chapterCount = playlength / seconds;

    // create an array for the chapters and fill it
    MP4Chapter_t * chapters = (MP4Chapter_t*)malloc(sizeof(MP4Chapter_t) * chapterCount);
    for (uint32_t i = 0; i < chapterCount; ++i)
    {
        chapters[i].duration = seconds * 1000;
        sprintf(chapters[i].title, "Chapter %i", i+1);
    }

    // now set the chapters
    MP4SetChapters(hFile, chapters, chapterCount, toChapterType);

    // free up the memory
    free(chapters);

    return 0;
}

///////////////////////////////////////////////////////////////////////////////

}} // namespace mp4v2::util
