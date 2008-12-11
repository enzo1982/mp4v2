#ifndef MP4V2_UTIL_QTFF_H
#define MP4V2_UTIL_QTFF_H

/// @namespace mp4v2::util::qtff (private) QuickTime File Format.
/// <b>WARNING: THIS IS A PRIVATE NAMESPACE. NOT FOR PUBLIC CONSUMPTION.</b>
///
/// This namespace implements some features that are specified by the
/// QuickTime File Format Specification, revision 2007-09-04.
///
/// This namespace provides a rich C++ interface, and is not intended to be
/// exposed as public API in its current state, especially given its curren
/// parent namespace.
///
/// The dependents of this module are libutil/utilities.
///
namespace mp4v2 { namespace util { namespace qtff {
    using namespace std;

///////////////////////////////////////////////////////////////////////////////

/// Singleton class for colr-box (Color Parameter Box) support.
///
/// A colr-box is expected to be contained in a vdeo track which is one of
/// the following coding types:
///     @li avc1
///     @li mp4v
///
/// This implementation assumes a maximum count of <b>1</b> for
/// VideoSampleEntry of the supported codings.
///
/// This implementation supports parameter-type 'nclc' only.
///
class MP4V2_EXPORT ColorParameterBox
{
public:
    /// Object structure for colr-box item.
    /// This object correlates to one colr-box (Color Parameter Box).
    ///
    class MP4V2_EXPORT Item
    {
    public:
        Item();

        /// reset to state of newly constructed object.
        void reset();

        // convert from string CSV format.
        void convertFromCSV( const string& csv );

        // convert to string CSV format.
        string convertToCSV() const;

        // convert to string CSV format with buffer.
        string& convertToCSV( string& buffer ) const;

    public:
        /// a 16-bit unsigned integer index.
        /// Specifies an index into a table specifying the CIE 1931 xy
        /// chromaticity coordinates of the white point and the red, green, and
        /// blue primaries. The table of primaries specifies the white point and
        /// the red, green, and blue primary color points for a video system.
        uint16_t primariesIndex;

        /// a 16-bit unsigned integer index.
        /// Specifies an an index into a table specifying the nonlinear transfer
        /// function coefficients used to translate between RGB color space values
        /// and Y′CbCr values. The table of transfer function coefficients
        /// specifies the nonlinear function coefficients used to translate
        /// between the stored Y′CbCr values and a video capture or display
        /// system.
        uint16_t transferFunctionIndex;

        /// a 16-bit unsigned integer index.
        /// Specifies an index into a table specifying the transformation matrix
        /// coefficients used to translate between RGB color space values and
        /// Y′CbCr values. The table of matrixes specifies the matrix used during
        /// the translation.
        uint16_t matrixIndex;
    };

    class MP4V2_EXPORT IndexedItem {
    public:
        IndexedItem();

        uint16_t trackIndex;
        uint16_t trackId;
        Item     item;
    };

    typedef vector<IndexedItem> ItemList;

    static bool list( MP4FileHandle file, ItemList& itemList );

    /// Add colr-box by track-index.
    ///
    /// This function adds a colr-box to <b>trackId</b> of <b>file</b>.
    /// The track must be a video-track and match one of the supporting
    /// codings.
    ///
    /// @param file on which to operate.
    /// @param trackIndex on which to operate.
    /// @param item colr-box properties to set.
    ///
    /// @return <b>true</b> on failure, <b>false</b> on success.
    ///
    static bool add( MP4FileHandle file, uint16_t trackIndex, const Item& item );

    /// Add colr-box by track-id.
    ///
    /// This function adds a colr-box to <b>trackId</b> of <b>file</b>.
    /// The track must be a video-track and match one of the supporting
    /// codings.
    ///
    /// @param file on which to operate.
    /// @param trackId on which to operate.
    /// @param item colr-box properties to set.
    ///
    /// @return <b>true</b> on failure, <b>false</b> on success.
    ///
    static bool add( MP4FileHandle file, MP4TrackId trackId, const Item& item );

    /// Store colr-box (Color Parameter Box) properties by track-index.
    ///
    /// This function sets the properties of a <b>colr-box</b>
    /// (Color Parameter Box).
    ///
    /// @param file on which to operate.
    /// @param trackIndex on which to operate.
    /// @param item colr-box properties to set.
    ///
    /// @return <b>true</b> on failure, <b>false</b> on success.
    ///
    static bool set( MP4FileHandle file, uint16_t trackIndex, const Item& item );

    /// Store colr-box (Color Parameter Box) properties by track-id.
    ///
    /// This function sets the properties of a <b>colr-box</b>
    /// (Color Parameter Box).
    ///
    /// @param file on which to operate.
    /// @param trackId on which to operate.
    /// @param item colr-box properties to set.
    ///
    /// @return <b>true</b> on failure, <b>false</b> on success.
    ///
    static bool set( MP4FileHandle file, MP4TrackId trackId, const Item& item );

    /// Fetch colr-box (Color Parameter Box) properties by track-index.
    ///
    /// This function gets the properties of a <b>colr-box</b>
    /// (Color Parameter Box).
    ///
    /// @param file on which to operate.
    /// @param trackIndex on which to operate.
    /// @param item colr-box properties to set.
    ///
    /// @return <b>true</b> on failure, <b>false</b> on success.
    ///
    static bool get( MP4FileHandle file, uint16_t trackIndex, Item& item );

    /// Fetch colr-box (Color Parameter Box) properties by track-id.
    ///
    /// This function gets the properties of a <b>colr-box</b>
    /// (Color Parameter Box).
    ///
    /// @param file on which to operate.
    /// @param trackId on which to operate.
    /// @param item colr-box properties to set.
    ///
    /// @return <b>true</b> on failure, <b>false</b> on success.
    ///
    static bool get( MP4FileHandle file, MP4TrackId trackId, Item& item );

    /// Remove colr-box (Color Parameter Box) by track-index.
    ///
    /// @param file on which to operate.
    /// @param trackIndex on which to operate.
    ///
    /// @return <b>true</b> on failure, <b>false</b> on success.
    ///
    static bool remove( MP4FileHandle file, uint16_t trackIndex );

    /// Remove colr-box (Color Parameter Box) by track-id.
    ///
    /// @param file on which to operate.
    /// @param trackId on which to operate.
    ///
    /// @return <b>true</b> on failure, <b>false</b> on success.
    ///
    static bool remove( MP4FileHandle file, MP4TrackId trackId );

public:
    static const string BOX_TYPE; ///< 4-char type code
};

///////////////////////////////////////////////////////////////////////////////

/// Singleton class for pasp-box (Picture Aspect Ratio Box) support.
class MP4V2_EXPORT PictureAspectRatioBox
{
public:
    /// Object structure for pasp-box item.
    /// This object correlates to one pasp-box (Picture Aspect Ratio Box).
    class MP4V2_EXPORT Item
    {
    public:
        Item ();

        /// reset to state of newly constructed object.
        void reset();

        // convert from string CSV format.
        void convertFromCSV( const string& csv );

        // convert to string CSV format.
        string convertToCSV() const;

        // convert to string CSV format with buffer.
        string& convertToCSV( string& buffer ) const;

    public:
        /// an unsigned 32-bit integer specifying the vertical spacing of pixels.
        uint32_t hSpacing;

        /// an unsigned 32-bit integer specifying the horizontal spacing of pixels.
        uint32_t vSpacing;
    };

    class MP4V2_EXPORT IndexedItem {
    public:
        IndexedItem();

        uint16_t trackIndex;
        uint16_t trackId;
        Item     item;
    };

    typedef vector<IndexedItem> ItemList;

    static bool list( MP4FileHandle file, ItemList& itemList );

    /// Add pasp-box by track-index.
    ///
    /// This function adds a pasp-box to <b>trackId</b> of <b>file</b>.
    /// The track must be a video-track and match one of the supporting
    /// codings.
    ///
    /// @param file on which to operate.
    /// @param trackIndex on which to operate.
    /// @param item pasp-box properties to set.
    ///
    /// @return <b>true</b> on failure, <b>false</b> on success.
    ///
    static bool add( MP4FileHandle file, uint16_t trackIndex, const Item& item );

    /// Add pasp-box by track-id.
    ///
    /// This function adds a pasp-box to <b>trackId</b> of <b>file</b>.
    /// The track must be a video-track and match one of the supporting
    /// codings.
    ///
    /// @param file on which to operate.
    /// @param trackId on which to operate.
    /// @param item pasp-box properties to set.
    ///
    /// @return <b>true</b> on failure, <b>false</b> on success.
    ///
    static bool add( MP4FileHandle file, MP4TrackId trackId, const Item& item );

    /// Store pasp-box (Color Parameter Box) properties by track-index.
    ///
    /// This function sets the properties of a <b>pasp-box</b>
    /// (Color Parameter Box).
    ///
    /// @param file on which to operate.
    /// @param trackIndex on which to operate.
    /// @param item pasp-box properties to set.
    ///
    /// @return <b>true</b> on failure, <b>false</b> on success.
    ///
    static bool set( MP4FileHandle file, uint16_t trackIndex, const Item& item );

    /// Store pasp-box (Color Parameter Box) properties by track-id.
    ///
    /// This function sets the properties of a <b>pasp-box</b>
    /// (Color Parameter Box).
    ///
    /// @param file on which to operate.
    /// @param trackId on which to operate.
    /// @param item pasp-box properties to set.
    ///
    /// @return <b>true</b> on failure, <b>false</b> on success.
    ///
    static bool set( MP4FileHandle file, MP4TrackId trackId, const Item& item );

    /// Fetch pasp-box (Color Parameter Box) properties by track-index.
    ///
    /// This function gets the properties of a <b>pasp-box</b>
    /// (Color Parameter Box).
    ///
    /// @param file on which to operate.
    /// @param trackIndex on which to operate.
    /// @param item pasp-box properties to set.
    ///
    /// @return <b>true</b> on failure, <b>false</b> on success.
    ///
    static bool get( MP4FileHandle file, uint16_t trackIndex, Item& item );

    /// Fetch pasp-box (Color Parameter Box) properties by track-id.
    ///
    /// This function gets the properties of a <b>pasp-box</b>
    /// (Color Parameter Box).
    ///
    /// @param file on which to operate.
    /// @param trackId on which to operate.
    /// @param item pasp-box properties to set.
    ///
    /// @return <b>true</b> on failure, <b>false</b> on success.
    ///
    static bool get( MP4FileHandle file, MP4TrackId trackId, Item& item );

    /// Remove pasp-box (Color Parameter Box) by track-index.
    ///
    /// @param file on which to operate.
    /// @param trackIndex on which to operate.
    ///
    /// @return <b>true</b> on failure, <b>false</b> on success.
    ///
    static bool remove( MP4FileHandle file, uint16_t trackIndex );

    /// Remove pasp-box (Color Parameter Box) by track-id.
    ///
    /// @param file on which to operate.
    /// @param trackId on which to operate.
    ///
    /// @return <b>true</b> on failure, <b>false</b> on success.
    ///
    static bool remove( MP4FileHandle file, MP4TrackId trackId );

public:
    static const string BOX_TYPE; ///< 4-char type code
};

///////////////////////////////////////////////////////////////////////////////

}}} // namespace mp4v2::util::qtff

#endif // MP4V2_UTIL_QTTF_H
