#ifndef MP4V2_UTIL_ITMF_COVERARTBOX_H
#define MP4V2_UTIL_ITMF_COVERARTBOX_H

namespace mp4v2 { namespace util { namespace itmf {

///////////////////////////////////////////////////////////////////////////////

/// Functional class for covr-box (Cover-art Box) support.
///
class MP4V2_EXPORT CoverArtBox
{
public:
    /// Object structure for covr-box item.
    /// This object correlates to one covr->data atom and offers automatic
    /// memory freeing when <b>autofree</b> is true.
    ///
    class MP4V2_EXPORT Item
    {
    public:
        Item();
        ~Item();

        /// Reset to state of newly constructed object.
        /// If <b>buffer</b> is not NULL and <b>autofree</b> is true the
        /// buffer will be free'd.
        void reset();

        BasicType type;     ///< covr-box type.
        uint8_t*  buffer;   ///< buffer point to raw covr-box data.
        uint32_t  size;     ///< size of covr-box buffer size in bytes.
        bool      autofree; ///< when true invoke free(buffer) upon destruction.
    };

    /// Object representing a list of covr-box items.
    typedef vector<Item> ItemList;

    /// Fetch list of covr-box items from file.
    ///
    /// @param file on which to operate.
    /// @param out vector of ArtItem objects.
    ///
    /// @return <b>true</b> on failure, <b>false</b> on success.
    ///
    static bool list( MP4FileHandle file, ItemList& out );

    /// Add covr-box item to file.
    /// Any necessary metadata atoms are first created.
    /// Additionally, if an empty data-atom exists it will be used,
    /// otherwise a new data-atom is added to <b>covr-atom</b>.
    ///
    /// @param file on which to operate.
    /// @param item covr-box object to place in file.
    ///
    /// @return <b>true</b> on failure, <b>false</b> on success.
    ///
    static bool add( MP4FileHandle file, const Item& item );

    /// Replace covr-box item in file.
    ///
    /// @param file on which to operate.
    /// @param item covr-box object to place in file.
    /// @param index 0-based index of covr-box to replace.
    ///
    /// @return <b>true</b> on failure, <b>false</b> on success.
    ///
    static bool set( MP4FileHandle file, const Item& item, uint32_t index );

    /// Fetch covr-box item from file.
    ///
    /// @param file on which to operate.
    /// @param item covr-box object populated with data.
    ///     The resulting object owns the malloc'd buffer and <b>item.autofree</b>
    ///     is set to true for convenient memory management.
    /// @param index 0-based index of covr-box to fetch.
    ///
    /// @return <b>true</b> on failure, <b>false</b> on success.
    ///
    static bool get( MP4FileHandle file, Item& item, uint32_t index );

    /// Remove covr-box item from file.
    ///
    /// @param file on which to operate.
    /// @param index 0-based index of covr-box to remove.
    ///     Default value indicates wildcard behavior to remove all items.
    ///
    /// @return <b>true</b> on failure, <b>false</b> on success.
    ///
    static bool remove( MP4FileHandle file, uint32_t index = numeric_limits<uint32_t>::max() );
};

///////////////////////////////////////////////////////////////////////////////

}}} // namespace mp4v2::util::itmf

#endif // MP4V2_UTIL_ITMF_COVERARTBOX_H
