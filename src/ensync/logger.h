/*
 * This file is part of the enSync project, written by Igor Siemienowicz.
 *
 * Copyright (c) 2017 Igor Siemienowicz All rights reserved
 * Distributed under the Boost Software License, Version 1.0 (see
 * http://www.boost.org/LICENSE_1_0.txt).
 */

#include <string>
#include <sstream>
#include <map>
#include <set>
#include <functional>
#include "thread-safety.h"

#ifndef enSync_logger_h_included
#define enSync_logger_h_included

namespace sync {

/**
 * \brief The core logging object for enSync
 *
 * The logger class uses one or more log outputs to send different types of
 * log messages to logging endpoints. Logging endpoints are defined as C++
 * callable objects (e.g. lambdas) that are stored and called when there is
 * a message.
 *
 * Logging endpoints are organised into channels, so that messages can be
 * sent to multiple outputs simultaneously. Channels are simple integers,
 * but they can be associated with text labels for naming convenience.
 *
 * Log operations are thread-safe, and the singleton logger object is
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
class logger final
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
     * \brief A set of channels
     */
    using channel_set = std::set<channel>;

    /**
     * \brief A text label for a channel
     *
     * By convention, labels should be three characters, in upper-case, but
     * this is not enforced. A channel may have at most one label, but need
     * not have a label.
     */
    using label = std::wstring;

    /**
     * \brief An identifier for a logging endpoint
     *
     * Logging endpoints are destinations for log messages. Identifiers are
     * assigned when they are registered with the logger, and used to
     * subseqeuntly deregister them when necessary.
     */
    using endpoint_id = unsigned int;

    /**
     * \brief Defines a log message destination as a callable object
     *
     * Endpoints are called to output a log message. Endpoints may be
     * registered with any number of channels, and are called when there is
     * a message for one of the channels for which they are registered.
     */
    using endpoint =
        std::function<
            void(channel c, const label& l, const std::wstring& msg)>;

    // -- Methods --

    /**
     * \brief Retrieve the single instance of the Logger
     *
     * \return The single Logger instance
     */
    static logger& instance(void);

    /**
     * \brief Set the label for a given channel
     *
     * If the channel already has a label, it is overwritten. This method
     * write-locks the internal mutex.
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
     * necessary.
     *
     * This method read-locks the internal mutex.
     *
     * \param c The channel whose label is to be retrieved
     *
     * \return The channel label, if present, otherwise an empty string
     */
    const label channel_label(channel c) const;

    /**
     * \brief Add a new endpoint to the logger
     *
     * This method adds a new endpoint callable to the collection of
     * endpoints, subscribing to the given logging channels. The internal
     * mutex is write-locked for this operation.
     *
     * Note that if a `nullptr` function is passed to this method, it is
     * *not* registered, and a zero ID is returned.
     *
     * \param channel_set The channels to which the endpoint should be
     * subscribed
     *
     * \param e The endpoint to add; should not be `nullptr`
     *
     * \return The newly assigned ID for the endpoint; this should be
     * retained if the endpoint will be removed from the logger at some
     * later time; IDs are 1 or greater; zero is returned if an attempt is
     * made to add a `nullptr` function endpoint
     */
    endpoint_id add(const channel_set& channels, endpoint e);

    /**
     * \brief Log a message to the given channel
     *
     * All endpoints subscribed to this channel are called with the channel
     * number, its label and the message text. Note that if there are no
     * endpoints subscribed to the given channel number, then no message
     * is logged, and no error is signalled.
     *
     * This operation write-locks the internal mutex.
     *
     * \param c The channel number on which to log the message
     *
     * \param msg The message to log
     */
    void log(channel c, const std::wstring& msg);

    /**
     * \brief Deregister a previously registered endpoint
     *
     * This method removes an endpoint, given its ID (assigned on add). If an
     * endpoint with the given ID is not found, no error is signalled. Note
     * that deregistered endpoint IDs will not be reassigned on subsequent
     * called to the `add` method.
     *
     * This operation write-locks the internal mutex.
     */
    void remove(endpoint_id id);

    // --- Internal Declarations ---

private:

    // -- Internal Sub-types --

    /**
     * \brief A map of endpoint IDs to their corresponding endpoints
     */
    using endpoint_map = std::map<endpoint_id, endpoint>;

    /**
     * \brief A collection of (unique) endpoint IDs
     */
    using endpoint_id_set = std::set<endpoint_id>;

    /**
     * \brief A map of channels to endpoint ID sets
     */
    using channel_endpoint_id_set_map = std::map<channel, endpoint_id_set>;

    // -- Internal Methods --

    /**
     * Standard constructor, only accessed by the `instance` method
     */
    logger(void);

    // -- Attributes --

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

    /**
     * The next ID to assign to an endpoint
     *
     * IDs are 1 or greater. This attribute is incremented every time a new
     * endpoint is registered. It is protected by the `m_mutex` attribute.
     */
    endpoint_id m_next_endpoint_id;

    /**
     * \brief The collection of logging endpoints, indexed by endpoint ID
     *
     * This collection is protected by the `m_mutex` attribute.
     */
    endpoint_map m_endpoints;

    /**
     * \brief The map of channels to endpoint ID sets
     *
     * This collection is protected by the `m_mutex` attribute.
     */
    channel_endpoint_id_set_map m_channel_endpoint_map;

    // Disable copy semantics
    logger(const logger&) = delete;
    logger operator=(const logger&) = delete;

};  // end logger class

}   // end sync namespace

#define ENSYNC_LOG( ch, wmsg ) do { \
    std::wstringstream wstrm; \
    wstrm << wmsg; \
    ::sync::logger::instance().log(ch, wstrm.str()); \
} while (false)

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
 * Where logging is turned off, logging operations have a low (but non-zero)
 * cost.
 *
 * \todo Expand on logging in *enSync*
 */
