// Copyright 2019 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @file RemoteLocators.hpp
 */

#ifndef _FASTDDS_RTPS_COMMON_REMOTELOCATORS_HPP_
#define _FASTDDS_RTPS_COMMON_REMOTELOCATORS_HPP_

#include <fastdds/rtps/common/Locator.h>
#include <fastdds/utils/collections/ResourceLimitedVector.hpp>
#include <fastdds/dds/log/Log.hpp>

namespace eprosima {
namespace fastdds {
namespace rtps {

/**
 * Holds information about the locators of a remote entity.
 */
struct RemoteLocatorList
{
    /**
     * Default constructor of RemoteLocatorList for deserialize.
     */
    RemoteLocatorList()
    {
    }

    /**
     * Construct a RemoteLocatorList.
     *
     * @param max_unicast_locators Maximum number of unicast locators to hold.
     * @param max_multicast_locators Maximum number of multicast locators to hold.
     */
    RemoteLocatorList(
            size_t max_unicast_locators,
            size_t max_multicast_locators)
        : unicast(ResourceLimitedContainerConfig::fixed_size_configuration(max_unicast_locators))
        , multicast(ResourceLimitedContainerConfig::fixed_size_configuration(max_multicast_locators))
    {
    }

    /**
     * Copy-construct a RemoteLocatorList.
     *
     * @param other RemoteLocatorList to copy data from.
     */
    RemoteLocatorList(
            const RemoteLocatorList& other)
        : unicast(other.unicast)
        , multicast(other.multicast)
    {
    }

    /**
     * Assign locator values from other RemoteLocatorList.
     *
     * @param other RemoteLocatorList to copy data from.
     *
     * @remarks Using the assignment operator is different from copy-constructing as in the first case the
     * configuration with the maximum number of locators is not copied. This means that, for two lists with
     * different maximum number of locators, the expression `(a = b) == b` may not be true.
     */
    RemoteLocatorList& operator = (
            const RemoteLocatorList& other)
    {
        unicast = other.unicast;
        multicast = other.multicast;
        return *this;
    }

    /**
     * Adds a locator to the unicast list.
     *
     * If the locator already exists in the unicast list, or the maximum number of unicast locators has been reached,
     * the new locator is silently discarded.
     *
     * @param locator Unicast locator to be added.
     */
    void add_unicast_locator(
            const Locator_t& locator)
    {
        for (const Locator_t& loc : unicast)
        {
            if (loc == locator)
            {
                return;
            }
        }

        unicast.push_back(locator);
    }

    /**
     * Adds a locator to the multicast list.
     *
     * If the locator already exists in the multicast list, or the maximum number of multicast locators has been reached,
     * the new locator is silently discarded.
     *
     * @param locator Multicast locator to be added.
     */
    void add_multicast_locator(
            const Locator_t& locator)
    {
        for (const Locator_t& loc : multicast)
        {
            if (loc == locator)
            {
                return;
            }
        }

        multicast.push_back(locator);
    }

    //! List of unicast locators
    ResourceLimitedVector<Locator_t> unicast;
    //! List of multicast locators
    ResourceLimitedVector<Locator_t> multicast;
};

/*
 * multicast max_size , multicast size , unicast max_size , unicast size ( locator[0] , locator[1] , ... )
 */
inline std::ostream& operator <<(
        std::ostream& output,
        const RemoteLocatorList& remote_locators)
{
    // Stored multicast locators
    output << "{";
    if (!remote_locators.multicast.empty())
    {
        output << "MULTICAST:[";
        output << remote_locators.multicast[0];
        for (auto it = remote_locators.multicast.begin() + 1; it != remote_locators.multicast.end(); ++it)
        {
            output << "," << *it;
        }
        output << "]";
    }

    // Stored unicast locators
    if (!remote_locators.unicast.empty())
    {
        output << "UNICAST:[";
        output << remote_locators.unicast[0];
        for (auto it = remote_locators.unicast.begin() + 1; it != remote_locators.unicast.end(); ++it)
        {
            output << "," << *it;
        }
        output << "]";
    }
    output << "}";
    return output;
}

inline std::istream& operator >>(
        std::istream& input,
        RemoteLocatorList& locList)
{
    std::istream::sentry s(input);
    locList = RemoteLocatorList();

    if (s)
    {
        char punct;
        char letter;
        Locator_t l;
        std::ios_base::iostate excp_mask = input.exceptions();

        try
        {
            input.exceptions(excp_mask | std::ios_base::failbit | std::ios_base::badbit);
            std::stringbuf sb_aux;
            Locator_t locator;

            // Read {_
            input >> punct >> letter;

            if (letter == 'M')
            {
                input.get(sb_aux, '[');

                // Read every locator
                while (punct != ']')
                {
                    input >> locator;
                    locList.add_multicast_locator(locator);
                    input >> punct;
                }

                input >> letter;
            }

            if (letter == 'U')
            {
                input >> punct;

                // Read every locator
                while (punct != ']')
                {
                    input >> locator;
                    locList.add_unicast_locator(locator);
                    input >> punct;
                }

                input >> letter;
            }
        }
        catch (std::ios_base::failure& )
        {
            locList.unicast.clear();
            locList.multicast.clear();
            EPROSIMA_LOG_WARNING(REMOTE_LOCATOR_LIST, "Error deserializing RemoteLocatorList");
        }

        input.exceptions(excp_mask);
    }

    return input;
}

} /* namespace rtps */
} /* namespace fastdds */
} /* namespace eprosima */

#endif /* _FASTDDS_RTPS_COMMON_REMOTELOCATORS_HPP_ */
