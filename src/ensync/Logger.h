/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <string>
#include <map>
#include "thread-safety.h"

#ifndef enSync_Logger_h_included
#define enSync_Logger_h_included

namespace sync {

/**
 * \brief The core logging object for enSync
 *
 * The Logger object uses one or more log outputs to send different types of
 * log messages to logging endpoints. Logging endpoints are defined as C++
 * callable objects (e.g. lambdas) that are stored and called when there is
 * a message.
 *
 * Logging endpoints are organised into channels, so that messages can be
 * sent to multiple outputs simultaneously. Channels are simple integers,
 * but they can be associated with text labels for naming convenience.
 *
 * Log operations are thread-safe, and the singleton Logger object is
 * guaranteed to exist from the first time it is used until the end of the
 * process.
 *
 * This class is instantiated the first time it is used. To enable logging,
 * at least one log must be added with a logging type (using the `add`
 * method. For very low-level logs (e.g. error logs) this should be done as
 * early as possible.
 *
 * See the \ref logging page for an overview of logging in enSync.
 *
 * \todo A code example
 */
class Logger final
{

    // --- External Interface ---

public:

    // -- Public Sub-types --

    /**
     * \brief A numeric channel for a log
     *
     * Logs are organised into channels, to make them easier to locate and
     * reference. Different log endpoints (e.g. the console, a file, etc.)
     * 'subscribe' to channels to receive their notifications. Log channels
     * also have a string label, as well as other formatting, that may be
     * attached to messages.
     * 
     * The library will likely establish a number of 'conventional' channels
     * (e.g. errors, warnings, etc.), as well as allowing other 'special
     * purpose' log channels.
     */
    using channel = unsigned int;

    /**
     * \brief A text label for a channel
     *
     * By convention, labels should be three characters, in upper-case, but
     * this is not enforced. A channel may have at most one label, but need
     * not have a label.
     */
     using label = std::wstring;

    // -- Methods --

    /**
     * \brief Retrieve the single instance of the Logger
     *
     * \return The single Logger instance
     */
    static Logger& instance(void);

    /**
     * \brief Set the label for a given channel
     *
     * If the channel already has a label, it is overwritten.
     *
     * \param c The channel number
     *
     * \param l The label for the channel
     */
    void set_channel_label(channel c, const label& l);

    /**
     * \brief Retrieve the label for the given channel
     *
     * An empty string is returned if the channel has not label.
     *
     * Note that return value is by value, rather than by reference. This is
     * because we want to be able to return a temporary empty string when
     * necessary. Move semantics should be able to optimise this?
     *
     * \param c The channel whose label is to be retrieved
     *
     * \return The channel label, if present, otherwise an empty string
     */
    const label channel_label(channel c) const;

    // --- Internal Declarations ---

private:

    /**
     * Standard constructor, only accessed by the `instance` method
     */
    Logger(void);

    /**
     * \brief A single mutex for protecting logging operations
     */
    mutable ::sync::mutex m_mutex;

    /**
     * The map of channels to labels
     *
     * This container is thread-protected by the object mutex
     */
    std::map<channel, label> m_channel_labels;

    // Disable copy semantics
    Logger(const Logger&) = delete;
    Logger operator=(const Logger&) = delete;

};  // end Logger class

}   // end sync namespace

#endif

/**
 * \page logging Logging in enSync
 *
 * Logging in this libary is implemented using a singleton object (the only
 * singleton!) accessed by all components that require logging services. The
 * core logging functionality is atomic, thread-safe, and relies only on core
 * functionality.
 *
 * The logging functionality is intended to be light-weight, and usable for
 * many purposes. These purposes include:
 *
 *  *   Error reporting
 *  *   Status reporting
 *  *   Debugging
 *
 * Where logging is turned off, logging operations have a low (non-zero)
 * cost.
 *
 * \todo Expand on logging in *enSync*
 */
