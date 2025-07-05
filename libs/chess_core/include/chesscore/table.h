/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESSCORE_TABLE_H
#define CHESSCORE_TABLE_H

#include <array>

namespace chesscore {

/**
 * \brief A type that can be used to index a table.
 *
 * The type has to provide an index() method that returns a size_t.
 */
template<typename T>
concept IndexType = requires(const T &i) {
    { i.index() } -> std::convertible_to<std::size_t>;
};

/**
 * \brief A table.
 *
 * A table is a collection of elements indexed by some type. The elements can be
 * tables themselves, thereby forming "multi-dimensional" tables. The index type
 * can either be an integral type or it should conform to the IndexType concept.
 * \tparam ElementT Type of the elements in the table.
 * \tparam Size Number of entries in the table.
 * \tparam IndexT Type used for indexing into the table.
 */
template<typename ElementT, size_t Size, typename IndexT>
class Table {
public:
    /**
     * \brief Create an empty table.
     */
    constexpr Table() = default;

    /**
     * \brief Create a table with the given entries.
     *
     * \tparam Args The entries.
     */
    template<typename... Args>
    constexpr Table(Args... data) : m_data{data...} {
        static_assert(sizeof...(data) == Size, "Wrong number of elements");
    }

    /**
     * \brief The number of elements in the table.
     *
     * \return The number of elements in the table.
     */
    constexpr auto size() const -> size_t { return m_data.size(); }

    /**
     * \brief Index table entries.
     *
     * Allows accessing table entries via index.
     * \param index Index of the element.
     * \return The element.
     */
    constexpr auto operator[](const IndexT &index) const -> const ElementT &
    requires std::is_integral<IndexT>::value
    {
        return m_data[index];
    }

    /**
     * \brief Index table entries.
     *
     * Allows accessing table entries via index.
     * \param index Index of the element.
     * \return The element.
     */
    constexpr auto operator[](const IndexT &index) const -> const ElementT &
    requires IndexType<IndexT>
    {
        return m_data[index.index()];
    }
private:
    std::array<ElementT, Size> m_data;
};

} // namespace chesscore

#endif
