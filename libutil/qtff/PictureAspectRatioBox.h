#ifndef MP4V2_UTIL_QTFF_PICTUREAPSECTRATIOBOX_H
#define MP4V2_UTIL_QTFF_PICTUREAPSECTRATIOBOX_H

namespace mp4v2 { namespace util { namespace qtff {
    using namespace std;

///////////////////////////////////////////////////////////////////////////////

/// Functional class for pasp-box (Picture Aspect Ratio Box) support.
///
/// A pasp-box is expected to be contained in a video track which is one of
/// the following coding types:
///     @li avc1
///     @li mp4v
///
/// This implementation assumes a maximum count of <b>1</b> for
/// VideoSampleEntry of the supported codings.
///
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
};

///////////////////////////////////////////////////////////////////////////////

}}} // namespace mp4v2::util::qtff

#endif // MP4V2_UTIL_QTTF_PICTUREAPSECTRATIOBOX_H
