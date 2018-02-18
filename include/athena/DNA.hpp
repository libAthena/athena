#ifndef DNA_HPP
#define DNA_HPP

/* BIG FAT WARNING!!!
 *
 * The type-structure of this file is expected to remain consistent for 'atdna'
 * Any changes to the types or namespacing must be reflected in 'atdna/main.cpp'
 */

#include "Global.hpp"
#include "IStreamReader.hpp"
#include "IStreamWriter.hpp"
#include "DNAOp.hpp"
#include <vector>
#include <memory>

namespace athena::io
{

/**
 * @brief Base DNA class used against 'atdna'
 * @tparam DNAE Default-endianness for contained DNA values
 *
 * Athena bundles a build-tool called 'atdna'. This tool functions
 * just like the 'clang' compiler, except it emits a full .cpp implementation
 * with all read/write calls necessary to marshal the DNA structure to/from
 * a streamed medium
 */
template <Endian DNAE>
struct DNA
{
    /**
     * @brief Designated byte-order used for serializing fields
     */
    static constexpr Endian DNAEndian = DNAE;

    /**
     * @brief Template type signaling atdna to capture the value where it's used
     * @tparam T The type of the value. Can be any numeric type or atVec* type
     * @tparam VE Endianness of the value
     */
    template <typename T, Endian VE = DNAE>
    using Value = T;

    /**
     * @brief Template type wrapping std::vector and signaling atdna to manipulate it where it's used
     * @tparam T The type of contained elements. Can be any numeric type, atVec* type, or another DNA subclass
     * @tparam cntVar C++ expression wrapped in DNA_COUNT macro to determine number of elements for vector
     * @tparam VE Endianness of the contained values
     */
    template <typename T, size_t cntVar, Endian VE = DNAE>
    using Vector = std::vector<T>;

    /**
     * @brief Template type wrapping std::unique_ptr<atUint8[]> and signaling atdna to read a
     *        raw byte-buffer where it's used
     * @tparam sizeVar C++ expression wrapped in DNA_COUNT macro to determine number of bytes for buffer
     */
    template <size_t sizeVar>
    using Buffer = std::unique_ptr<atUint8[]>;

    /**
     * @brief Template type wrapping std::string and signaling atdna to read string data where it's used
     * @tparam sizeVar C++ expression wrapped in DNA_COUNT macro to determine number of characters for string
     *                 -1 literal indicates null-terminated string
     */
    template <atInt32 sizeVar = -1>
    using String = std::string;

    /**
     * @brief Template type wrapping std::wstring and signaling atdna to read wstring data where it's used
     * @tparam sizeVar C++ expression wrapped in DNA_COUNT macro to determine number of characters for wstring
     *                 -1 literal indicates null-terminated wstring
     */
    template <atInt32 sizeVar = -1, Endian VE = DNAE>
    using WString = std::wstring;

    /**
     * @brief Meta Template signaling atdna to insert a stream seek where it's used
     * @tparam offset C++ expression wrapped in DNA_COUNT macro to determine number of bytes to seek
     * @tparam direction SeekOrigin to seek relative to
     */
    template <off_t offset, SeekOrigin direction>
    struct Seek {};

    /**
     * @brief Meta Template signaling atdna to insert an aligning stream seek where it's used
     * @tparam align Number of bytes to align to
     */
    template <size_t align>
    struct Align {};

    /**
     * @brief Meta Template preventing atdna from emitting read/write implementations
     */
    struct Delete {};
};

/** Macro to supply count variable to atdna and mute it for other compilers */
#ifdef __clang__
#define DNA_COUNT(cnt) sizeof(cnt)
#else
#define DNA_COUNT(cnt) 0
#endif

}

#endif // DNA_HPP

