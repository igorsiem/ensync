/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <locale>
#include <string>
#include <sstream>
#include <vector>
#include <codecvt>

#ifndef ensync_strutils_h_included
#define ensync_strutils_h_included

namespace sync {

/**
 * \brief A class for string utilities - all its methods are static
 */
class strutils
{

public:

	/**
	 * \brief Specify a string type conversion between standard and wide
	 */
	using convert_type = std::codecvt<wchar_t, char, mbstate_t>;

	/**
	 * \brief Convert any type to a standard string
	 *
	 * The default implementation of this method uses a stringstream for the
	 * conversion, but some specialisations are also provided.
	 */
	template <typename T>
	static std::string& to_string(std::string& str, const T& t)
	{
		std::stringstream strm;
		strm << t;
		str = strm.str();
		return str;
	}	// end to_string method

	/**
	* \brief Convert any type to a standard string
	*
	* The default implementation of this method uses a stringstream for the
	* conversion, but some specialisations are also provided.
	*/
	template <typename T>
	static std::string to_string(const T& t)
	{
		std::string str;
		to_string(str, t);
		return str;
	}	// end to_string method


	/**
	 * \brief Convert any type to a wstring
	 *
	 * The default implementation of this template uses a stream, but there
	 * are specialisations.
	 */
	template <typename T>
	static std::wstring& to_wstring(std::wstring& wstr, const T& t)
	{
		std::wstringstream strm;
		strm << t;
		wstr = strm.str();
		return wstr;
	}

	/**
	* \brief Convert any type to a wstring
	*
	* The default implementation of this template uses a stream, but there
	* are specialisations.
	*/
	template <typename T>
	static std::wstring to_wstring(const T& t)
	{
		std::wstring wstr;
		to_wstring(wstr, t);
		return wstr;
	}	// end to_wstring

    // --- String Concatenation ---

    /**
     * \brief Concatenate the given strings, with the given (optional)
     * delimiter
     *
     * \tparam chartype The character type of the strings - either `char` or
     * `wchar_t`
     *
     * \param output_string A reference to an output string (which is also
     * returned)
     *
     * \param strings The strings to concatenate
     *
     * \param delimiter The (optional) delimiter to use
     *
     * \return A reference to `output_string`
     */
    template <typename chartype>
    static std::basic_string<chartype>& concatenate(
            std::basic_string<chartype>& output_string,
            const std::vector<std::basic_string<chartype> >& strings,
            const std::basic_string<chartype>& delimiter =
                std::basic_string<chartype>())
    {
        // Use a stream to concatenate
        std::basic_stringstream<chartype> strm;

        if (strings.size() > 0)
        {
            for (std::size_t i = 0; i < strings.size() - 1; i++)
                strm << strings[i] << delimiter;

            strm << strings.back();
        }

        // Return our result
        output_string = strm.str();
        return output_string;
    }   // end concatenate method

    /**
     * \brief Concatenate the given vector of strings, with an optional
     * delimiter
     *
     * \tparam chartype The character type of the strings - either `char` or
     * `wchar_t`
     *
     * \param strings The strings to concatenate
     *
     * \param delimiter The (optional) delimiter to use
     *
     * \return The concatenated string
     */
    template <typename chartype>
    static std::basic_string<chartype> concatenate(
            const std::vector<std::basic_string<chartype> >& strings,
            const std::basic_string<chartype>& delimiter =
                std::basic_string<chartype>())
    {
        std::basic_string<chartype> result;
        return concatenate(result, strings, delimiter);
    }   // end concatenate method

    protected:

    template <class facet_base>
    struct deletable_facet : public facet_base
    {
        template <class ...arg_types>
        deletable_facet(arg_types&& ...args) :
            facet_base(std::forward<arg_types>(args)...) {}
        virtual ~deletable_facet(void) {}
    };  // end deletable_facet class

};	// end strutils class

/**
 * \brief Convert a wide string to a string
 *
 * This method specialises the template method of the same name.
 */
template<>
std::string& strutils::to_string<std::wstring>(
		std::string& str,
		const std::wstring& wstr);

/**
 * \brief Convert a standard string to a wide string
 *
 * This method specialises the template method of the same name.
 */
template <>
std::wstring& strutils::to_wstring<std::string>(
		std::wstring& wstr,
		const std::string& str);

}   // end sync namespace

#endif
