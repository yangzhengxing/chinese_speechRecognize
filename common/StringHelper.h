#ifndef STRING_HELPER_H_2006_03_28
#define STRING_HELPER_H_2006_03_28

#include <cassert>
#include <typeinfo>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <cstdint>

#include "utf8.h"

namespace ekho {
namespace StringHelper {

/////////////////////////////////////////////////////////////////////////////////

/** @namespace StringHelper
 *  @brief Collection of functions for processing strings.
 *
 *  The namespace consists of functions for processing strings. <br>
 *  Most of these functions is applicable to std::string encoded in UTF-8. <br>
 *  The range definition is consistent with STL, i.e. [Begin, End) .
 *
 *  @author Yun Huang, huangyunict@gmail.com
 *  @date 2006-11-07
 *
 */

/////////////////////////////////////////////////////////////////////////////////
//  detail namespace implementation
namespace detail {

template <typename TOther>
inline TOther& String2OtherImpl(const std::string& str, TOther& other) {
    //  TODO: delete the dynamic check of typeid, use static check
    assert(typeid(TOther) != typeid(std::string));
    assert(typeid(TOther) != typeid(std::wstring));
    std::istringstream iss(str);
    iss >> other;
    if (iss.fail()) {
        throw std::runtime_error("String2Other: convert failed for string: " + str);
    }
    return other;
}

template <>
inline std::string& String2OtherImpl<std::string>(const std::string& str, std::string& other) {
    if (&str != &other) {
        other = str;
    }
    return other;
}

template <typename TOther>
inline std::string Other2StringImpl(const TOther& other, std::streamsize prec) {
    std::ostringstream oss;
    oss.precision(prec);
    oss << other;
    return oss.str();
}

template <>
inline std::string Other2StringImpl(const std::string& other, std::streamsize prec) {
    return other;
}

template <typename TInputIter, typename FIterSelect>
inline std::string ConcatImpl(TInputIter begin, TInputIter end, const std::string& sep, FIterSelect fIterSelect) {
    std::ostringstream oss;
    for (TInputIter it=begin; it!=end; ++it) {
        if (it != begin) {
            oss << sep;
        }
        oss << fIterSelect(it);
    }
    return oss.str();
}

template <typename TInputIter>
inline std::string ConcatImpl(TInputIter begin, TInputIter end, const std::string& sep) {
    std::ostringstream oss;
    for (TInputIter it=begin; it!=end; ++it) {
        if (it != begin) {
            oss << sep;
        }
        oss << *it;
    }
    return oss.str();
}

inline std::string ReplaceImpl(const std::string& str, const std::string& old_str, const std::string& new_str, std::string::size_type* pReplaced)
{
    if (old_str.empty())
    {
        return str;
    }
    std::string s;
    if (pReplaced)
    {
        *pReplaced = 0;
    }
    std::string::size_type prevpos = 0, pos;
    while (true)
    {
        pos = str.find(old_str, prevpos);
        if (pos == std::string::npos)
        {
            pos = str.size();
        }
        s.insert(s.size(), str, prevpos, pos - prevpos);
        if (pos == str.size())
        {
            break;
        }
        s.insert(s.size(), new_str);
        if (pReplaced)
        {
            ++(*pReplaced);
        }
        prevpos = pos + old_str.size();
    }
    return s;
}

inline std::string ReplaceStartImpl(const std::string& str, const std::string& old_str, const std::string& new_str, std::string::size_type* pReplaced)
{
    //  TODO: reimplement, optimize
    if (pReplaced)
    {
        *pReplaced = 0;
    }
    if (old_str.empty())
    {
        return str;
    }
    std::string s = str;
    if (s.find(old_str) == 0)
    {
        s.replace(0, old_str.size(), new_str);
        if (pReplaced)
        {
            ++(*pReplaced);
        }
    }
    return s;
}

inline std::string ReplaceEndImpl(const std::string& str, const std::string& old_str, const std::string& new_str, std::string::size_type* pReplaced)
{
    if (pReplaced)
    {
        *pReplaced = 0;
    }
    //  TODO: reimplement, optimize
    if (old_str.empty())
    {
        return str;
    }
    std::string s = str;
    std::string::size_type pos = s.rfind(old_str);
    if (pos != std::string::npos && pos + old_str.size() == s.size())
    {
        s.replace(pos, old_str.size(), new_str);
        if (pReplaced)
        {
            ++(*pReplaced);
        }
    }
    return s;
}

template <typename TElem, typename TOutputIter>
inline TOutputIter SplitSpaceImpl(std::istream& IS, TOutputIter result) {
    TElem elem;
    while (IS >> elem) {
        *result = elem;
        ++result;
    }
    return result;
}

template <typename TElem, typename TOutputIter>
inline TOutputIter SplitStringImpl(const std::string& str, TOutputIter result, const std::string& sep, bool output_empty) {
    std::string::size_type pos1 = 0;
    std::string::size_type pos2 = 0;
    TElem elem;
    if (str.empty() || sep.empty()) {
        return result;
    }
    while (true) {
        pos2 = str.find(sep, pos1);
        if (pos2 == std::string::npos) {
            pos2 = str.size();
        }
        if (pos2 != pos1 || output_empty) {
            *result = String2OtherImpl<TElem>(str.substr(pos1, pos2 - pos1), elem);
            ++result;
        }
        pos1 = pos2 + sep.size();
        if (pos2 == str.size()) {
            break;
        }
    }
    return result;
}

inline std::string& TrimLeftInplaceImpl(std::string& str, const std::string& trim_str) {
    if (!str.empty()) {
        std::string::size_type pos = str.find_first_not_of(trim_str, 0);
        if (pos == std::string::npos) {
            pos = str.size();
        }
        str.erase(0, pos);
    }
    return str;
}

inline std::string& TrimRightInplaceImpl(std::string& str, const std::string& trim_str) {
    if (!str.empty()) {
        std::string::size_type pos = str.find_last_not_of(trim_str, str.size());
        if (pos != std::string::npos) {
            str.erase(pos + 1, str.size() - pos);
        }
        else {
            str.erase(0, str.size());
        }
    }
    return str;
}

}   //  namespace detail

/** @} */

/////////////////////////////////////////////////////////////////////////////////
//  implementation

/** @name Convert Functions
 *
 *  Functions in this group convert between string and other types.
 *  @note These function do not check whether actually converted.
 */
/** @{ */
/** @brief Convert string to other type.
 *
 *  @param[in] str The input string.
 *  @return Return converted value of other type.
 */
template <typename TOther>
inline TOther String2Other(const std::string& str) {
    TOther other;
    return detail::String2OtherImpl(str, other);
}

template <typename TOther>
inline TOther& String2Other(const std::string& str, TOther& other) {
    return detail::String2OtherImpl(str, other);
}

/** @brief Convert other type to string.
 *
 *  @param[in] other The input value of other type.
 *  @param[in] prec The precision used in string stream.
 *  @return Return converted string.
 *  @note If input is a string, return the input, do NOT use string stream, i.e. spaces are kept.
 */
template <typename TOther>
inline std::string Other2String(const TOther& other, std::streamsize prec = 6) {
    return detail::Other2StringImpl(other, prec);
}
/** @} */

/** @name Concatenate/Split Functions
 *
 *  Functions in this group concatenate multiple item to string, or split string into multiple items.
 */
/** @{ */
/** @brief Concatenate every item in range to string, separated by given separator.
 *
 *  @param[in] begin The begin iterator.
 *  @param[in] end The end iterator.
 *  @param[in] sep The separator string written between two items.
 *  @return Return the concatenated string.
 */
template <typename TInputIter>
inline std::string Concat(TInputIter begin, TInputIter end, const std::string& sep = " ") {
    return detail::ConcatImpl(begin, end, sep);
}

template <typename TInputIter, typename FIterSelect>
inline std::string Concat(TInputIter begin, TInputIter end, const std::string& sep, FIterSelect fIterSelect) {
    return detail::ConcatImpl(begin, end, sep, fIterSelect);
}

/** @brief Split items from stream separated by spaces.
 *
 *  The items of given element type are separated by space characters, and output to iterator.
 *
 *  @param[in] IS The input stream.
 *  @param[in] result The output iterator.
 *  @return Return the output iterator pointed to next output position.
 */
template <typename TElem, typename TOutputIter>
inline TOutputIter SplitSpace(std::istream& IS, TOutputIter result) {
    return detail::SplitSpaceImpl<TElem, TOutputIter>(IS, result);
}

/** @brief Split items from string separated by spaces.
 *
 *  The items of given element type are separated by space characters, and output to iterator.
 *  @param[in] str The input string.
 *  @param[in] result The output iterator.
 *  @return Return the output iterator pointed to next output position.
 */
template <typename TElem, typename TOutputIter>
inline TOutputIter SplitSpace(const std::string& str, TOutputIter result) {
    std::istringstream iss(str);
    return detail::SplitSpaceImpl<TElem, TOutputIter>(iss, result);
}

/** @brief Split items from string separated by given string.
 *
 *  @param[in] str The input string.
 *  @param[in] result The output iterator.
 *  @param[in] sep The separating string.
 *  @param[in] output_empty Whether to output, if there is nothing between two adjacent separators.
 *  @return Return the output iterator pointed to next output position.
 */
template <typename TElem, typename TOutputIter>
inline TOutputIter SplitString(const std::string& str, TOutputIter result, const std::string& sep = " ", bool output_empty = false) {
    return detail::SplitStringImpl<TElem, TOutputIter>(str, result, sep, output_empty);
}

/** @brief Split items from string separated by given character.
 *
 *  @param[in] str The input string.
 *  @param[in] result The output iterator.
 *  @param[in] sep The separating character.
 *  @param[in] output_empty Whether to output, if there is nothing between two adjacent separators.
 *  @return Return the output iterator pointed to next output position.
 */
template <typename TElem, typename TOutputIter>
inline TOutputIter SplitChar(const std::string& str, TOutputIter result, char sep = ' ', bool output_empty = false) {
    std::string s(1, sep);
    return detail::SplitStringImpl<TElem, TOutputIter>(str, result, s, output_empty);
}

template <typename TOutputIter>
TOutputIter SplitUTF8(const std::string& str, TOutputIter result) {
    for (auto it=str.begin(); it<str.end(); )
    {
        char symbol[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};   //  actually 5 zeros should be enough
        uint32_t code = utf8::next(it, str.end());
        if (code == 0)
        {
            continue;
        }
        utf8::append(code, symbol); // initialize array `symbol`
        (*result++) = std::string(symbol);
    }
    return result;
}

inline std::string UTF16to8(const std::wstring& str) {
    std::string utf_str;
    utf8::utf16to8(str.begin(), str.end(), std::back_inserter(utf_str));
    return utf_str;
}

inline std::wstring UTF8to16(const std::string& str) {
    std::wstring utf_str;
    utf8::utf8to16(str.begin(), str.end(), std::back_inserter(utf_str));
    return utf_str;
}

/** @} */

/** @name Padding Functions
 *
 *  Functions in this group pad string to given length.
 */
/** @{ */
/** @brief Pad characters on the right of given string to given length.
 *
 *  @param[in] str The input string.
 *  @param[in] size The new string size.
 *  @param[in] c The padding character.
 *  @return Return the padded string with given length.
 *  @note If the length of input string is greater than the given size, do nothing.
 */
inline std::string& PadRightInplace(std::string& str, std::string::size_type size, char c = ' ') {
    if (str.size() > size) {
        return str;
    }
    return str.insert(str.size(), size - str.size(), c);
}

inline std::string PadRight(const std::string& str, std::string::size_type size, char c = ' ') {
    std::string result = str;
    return PadRightInplace(result, size, c);
}

/** @brief Pad characters on the right of given string to given length in the strict way.
 *
 *  @param[in] str The input string.
 *  @param[in] size The new string size.
 *  @param[in] c The padding character.
 *  @return Return the padded string with given length.
 *  @note If the length of input string is greater than the given size, throw exception.
 */
inline std::string& PadRightStrictInplace(std::string& str, std::string::size_type size, char c = ' ') {
    if (str.size() > size) {
        throw std::runtime_error("PadRightStrict: length of input string is greater than given size");
    }
    return str.insert(str.size(), size - str.size(), c);
}

inline std::string PadRightStrict(const std::string& str, std::string::size_type size, char c = ' ') {
    std::string result = str;
    return PadRightStrictInplace(result, size, c);
}
/** @} */


/** @name Case change Functions
 *
 *  Functions in this group could change the lower/upper case of char or string.
 *  @note The string or character must be encoded in UTF-8. <br>
 */
/** @{ */
/** @brief Change case of single character.
 *  kU2L  If true, convert from upper to lower, otherwise from lower to upper.
 *
 *  @param[in] c The character.
 *
 *  @return If converted, return the converted character, otherwise return the input character.
 */
template <bool kU2L>
inline char ChangeCase(char c) {
    char srcb = (kU2L ? 'A' : 'a');
    char tgtb = (kU2L ? 'a' : 'A');
    char srce = srcb + ('Z' - 'A');
    //  change case
    if (c >= srcb && c <= srce) {
        c = c - srcb + tgtb;
    }
    return c;
}

/** @brief Change case of range.
 *
 *  @param[in] begin The begin iterator.
 *  @param[in] end   The end iterator.
 *  @param[in] U2L  If true, convert from upper to lower, otherwise from lower to upper.
 */
template <bool kU2L, typename TForwardIter>
inline void ChangeCase(TForwardIter begin, TForwardIter end) {
    std::transform(begin, end, begin, std::bind(ChangeCase<kU2L>, std::placeholders::_1));
}

/** @brief Convert single character to lower case in "C" locale.
 *
 *  @param[in]  c The character.
 *  @return If converted, return the converted character, otherwise return the input   character.
 */
inline char ToLower(char c) {
    return ChangeCase<true>(c);
}

/** @brief Convert string to lower case in "C" locale.
 *
 *  @param[in]  str The input string.
 *  @return Return the converted string.
 */
inline std::string ToLower(const std::string& str) {
    std::string s = str;
    ChangeCase<true>(s.begin(), s.end());
    return s;
}

/** @brief Convert range to lower case in "C" locale in place.
 *
 *  @param[in] begin The begin iterator.
 *  @param[in] end   The end iterator.
 */
template <typename TForwardIter>
inline void ToLowerInplace(TForwardIter begin, TForwardIter end) {
    ChangeCase<true>(begin, end);
}

/** @brief Convert string to lower case in "C" locale in place.
 *
 *  @param[in]  str The input string.
 *  @param[out] str The output string.
 */
inline std::string& ToLowerInplace(std::string& str) {
    ChangeCase<true>(str.begin(), str.end());
    return str;
}

/** @brief Convert single character to upper case in "C" locale.
 *
 *  @param[in]  c The character.
 *  @return If converted, return the converted character, otherwise return the input   character.
 */
inline char ToUpper(char c) {
    return ChangeCase<false>(c);
}

/** @brief Convert string to upper case in "C" locale.
 *
 *  @param[in]  str The input string.
 *  @return Return the converted string.
 */
inline std::string ToUpper(const std::string& str) {
    std::string s = str;
    ChangeCase<false>(s.begin(), s.end());
    return s;
}

/** @brief Convert range to upper case in "C" locale in place.
 *
 *  @param[in] begin The begin iterator.
 *  @param[in] end   The end iterator.
 */
template <typename TForwardIter>
void ToUpperInplace(TForwardIter begin, TForwardIter end) {
    ChangeCase<false>(begin, end);
}

/** @brief Convert string to upper case in "C" locale in place.
 *
 *  @param[in]  str The input string.
 *  @param[out] str The output string.
 */
inline std::string& ToUpperInplace(std::string& str) {
    ChangeCase<false>(str.begin(), str.end());
    return str;
}
/** @} */

/** @name Trim Functions
 *
 *  Functions in this group could trim left/right characters of string.
 */
/** @{ */

//  unique
inline std::string& UniqueInplace(std::string& str, const char c = ' ') {
    auto fEqual = [](const char x, const char y, const char c) -> bool { return (x == c) && (y == c); };
    str.erase(std::unique(str.begin(), str.end(), bind(fEqual, std::placeholders::_1, std::placeholders::_2, c)), str.end());
    return str;
}

/** @brief Trim all given characters in left of string.
 *
 *  @param[in]  str  The input string.
 *  @param[in]  trim_str The character set to trim.
 *  @param[out] str The output string after trimming.
 *  @return Return the reference of trimmed string.
 */
inline std::string TrimLeft(const std::string& str, const std::string& trim_str = " \r\n\t") {
    auto result = str;
    detail::TrimLeftInplaceImpl(result, trim_str);
    return result;
}

inline std::string& TrimLeftInplace(std::string& str, const std::string& trim_str = " \r\n\t") {
    return detail::TrimLeftInplaceImpl(str, trim_str);
}

/** @brief Trim all given characters in right of string.
 *
 *  @param[in]  str The input string.
 *  @param[in]  trim_str The character set to trim.
 *  @param[out] str The output string after trimming.
 *  @return Return the reference of trimmed string.
 */
inline std::string TrimRight(const std::string& str, const std::string& trim_str = " \r\n\t") {
    std::string result = str;
    detail::TrimRightInplaceImpl(result, trim_str);
    return result;
}

inline std::string& TrimRightInplace(std::string& str, const std::string& trim_str = " \r\n\t") {
    return detail::TrimRightInplaceImpl(str, trim_str);
}

/** @brief Trim all given characters in both left and right of string.
 *
 *  @param[in]  str The input string.
 *  @param[in]  trim_str The character set to trim.
 *  @param[out] str The output string after trimming.
 *  @return Return the reference of trimmed string.
 */
inline std::string& TrimInplace(std::string& str, const std::string& trim_str = " \r\n\t") {
    detail::TrimLeftInplaceImpl(str, trim_str);
    detail::TrimRightInplaceImpl(str, trim_str);
    return str;
}

inline std::string Trim(const std::string& str, const std::string& trim_str = " \r\n\t") {
    std::string result = str;
    TrimInplace(result, trim_str);
    return result;
}

/** @} */


/** @name Find/Replace Functions
 *
 *  Find/Replace functions.
 */
/** @{ */

inline bool StartsWith(const std::string& str, const std::string& sub) {
    if (str.size() < sub.size()) {
        return false;
    }
    return std::equal(sub.begin(), sub.end(), str.begin());
}

inline bool EndsWith(const std::string& str, const std::string& sub) {
    if (str.size() < sub.size()) {
        return false;
    }
    return std::equal(sub.begin(), sub.end(), str.begin() + str.size() - sub.size());
}

/** @brief Replace old sub string to new sub string.
 *
 *  @param[in]  str The input string.
 *  @param[in]  old_str The old sub string.
 *  @param[in]  new_str The new sub string.
 *  @param[out] pReplaced The number of replacing.
 *  @return Return replaced string.
 */

inline std::string Replace(const std::string& str, const std::string& old_str, const std::string& new_str, std::string::size_type* pReplaced = NULL) {
    return detail::ReplaceImpl(str, old_str, new_str, pReplaced);
}

inline std::string& ReplaceInplace(std::string& str, const std::string& old_str, const std::string& new_str, std::string::size_type* pReplaced = NULL) {
    str = detail::ReplaceImpl(str, old_str, new_str, pReplaced);
    return str;
}

inline std::string ReplaceStart(const std::string& str, const std::string& old_str, const std::string& new_str, std::string::size_type* pReplaced = NULL) {
    return detail::ReplaceStartImpl(str, old_str, new_str, pReplaced);
}

inline std::string& ReplaceStartInplace(std::string& str, const std::string& old_str, const std::string& new_str, std::string::size_type* pReplaced = NULL) {
    str = detail::ReplaceStartImpl(str, old_str, new_str, pReplaced);
    return str;
}

inline std::string ReplaceEnd(const std::string& str, const std::string& old_str, const std::string& new_str, std::string::size_type* pReplaced = NULL) {
    return detail::ReplaceEndImpl(str, old_str, new_str, pReplaced);
}

inline std::string& ReplaceEndInplace(std::string& str, const std::string& old_str, const std::string& new_str, std::string::size_type* pReplaced = NULL) {
    str = detail::ReplaceEndImpl(str, old_str, new_str, pReplaced);
    return str;
}

/** @} */

}   //  namespace StringHelper
}   //  namespace ekho

#endif

