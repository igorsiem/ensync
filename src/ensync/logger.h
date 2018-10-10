/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <functional>
#include <iostream>
#include <map>
#include <mutex>
#include <set>
#include <tuple>
#include <vector>

#include <fmt/format.h>

#ifndef _ensync_logger_h_included
#define _ensync_logger_h_included

namespace ensync {

/**
 * \brief A thread-safe, singleton logging interface
 *
 * Note: log messages are excluded from internationalisation and
 * string-management
 *
 * \todo Expand this documentation
 */
class logger final
{

    // --- External Interface ---

    public:

    /**
     * \brief Enumerate the logging levels
     */
    enum class level_t
    {
        error       ///< Error messages
        , warning   ///< Warning messages
        , info      ///< Information
        , debug     ///< Debug messages
    };  // end level_t enum

    /**
     * \brief A set of level enumerators
     */
    using level_set = std::set<level_t>;

    /**
     * \brief A callable object for logging a given message at a given level
     */
    using endpoint_fn = std::function<void(level_t, const std::wstring&)>;

    /**
     * \brief Access the single instance of this class
     * 
     * \return A reference to the single logging instance
     */
    static logger& instance(void) { static logger l; return l; }

    /**
     * \brief Convert a level enumerator to a wide string
     * 
     * \param l The level enumerator
     * 
     * \return The human-readable string equivalent
     */
    static std::wstring to_wstring(level_t l)
    {
        switch (l)
        {
            case level_t::error: return L"ERR";
            case level_t::warning: return L"WAR";
            case level_t::info: return L"INF";
            case level_t::debug: return L"DEB";
            default: return L"***";
        }
    }

    /**
     * \brief Add a new endpoint to the collection of logging endpoints
     * 
     * \param ls The set of logging levels to which this endpoint should be
     * added
     * 
     * \param fn The logging endpoint function
     * 
     * \param fmt_str The fmt-style formatting string to use for log
     * messages; this should take two parammeters - the logging level
     * abbreviation ("ERR", "WAR", etc.) and the message itself
     */
    void add(
            level_set ls,
            endpoint_fn fn,
            std::wstring fmt_str = L"[{}] {}")
    {
        std::lock_guard<std::mutex> lck(m_mtx);

        for (auto l : ls)
            m_endpoints[l].push_back(
                std::make_tuple(fn, fmt_str));
    }   // end add method

    /**
     * \brief Clear all logging endpoints
     */
    void clear(void)
    {
        std::lock_guard<std::mutex> lck(m_mtx);
        m_endpoints = endpoint_map();
    }   // end clear method

    /**
     * \brief Log the given message at the given level
     * 
     * The message is sent to all logging endpoints registered for the given
     * level.
     * 
     * \param l The logging level
     * 
     * \param msg The message to log
     */
    void log(level_t l, const std::wstring& msg)
    {
        std::lock_guard<std::mutex> lck(m_mtx);
        for (auto endpoint : m_endpoints[l])
        {
            std::get<0>(endpoint)(
                l,
                fmt::format(
                    std::get<1>(endpoint),
                    to_wstring(l),
                    msg));
        }
    }   // log function

    /**
     * \brief 'Helper' method for quickly setting up logging to the console
     */
    void set_for_console(void)
    {
        // Error go to the error stream - everything else to stdout
        add({ level_t::error }, [](level_t, const std::wstring& msg)
        {
            std::wcerr << msg << std::endl;
        });

        add(
            { level_t::warning, level_t::info, level_t::debug },
            [](level_t, const std::wstring& msg)
            {
                std::wcout << msg << std::endl;
            });
    }   // end set_for_console

    // --- Internal Declarations ---

    private:

    /**
     * \brief An enpoint function, paired with its formatting string
     */
    using endpoint_t = std::tuple<endpoint_fn, std::wstring>;

    /**
     * \brief A collection of logging endpoints
     */
    using endpoint_vector = std::vector<endpoint_t>;

    /**
     * \brief A map of logging endpoint collections, indexed by logging level
     */
    using endpoint_map = std::map<level_t, endpoint_vector>;

    /**
     * \brief Trivial constructor
     *
     * This cannot be called externally - use `instance` to access the single
     * instance of this class.
     */
    logger(void) : m_mtx(), m_endpoints() {}

    /**
     * \brief Mutex protecting all logging operations
     */
    std::mutex m_mtx;

    /**
     * \brief The collection of endpoints we are using
     */
    endpoint_map m_endpoints;

};  // end logger

}   // end ensync namespace

#endif
