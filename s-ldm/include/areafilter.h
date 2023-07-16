/**
 * @file areafilter.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-07-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef AREAFILTER_H
#define AREAFILTER_H

extern "C"
{
#include "options.h"
}

/**
 * @brief 
 * 
 */
class areaFilter
{
public:
    /**
     * @brief Construct a new area Filter object
     * 
     */
    areaFilter();

    /**
     * @brief Construct a new area Filter object
     * 
     * @param opts_ptr 
     */
    explicit areaFilter(struct options *opts_ptr) : m_opts_ptr(opts_ptr) {}

    /**
     * @brief Set the Options object
     * 
     * @param opts_ptr 
     */
    void setOptions(struct options *opts_ptr)
    {
        m_opts_ptr = opts_ptr;
    }

    /**
     * @brief This function expects the m_opts_ptr pointer to be non-NULL, otherwise it always returns 'false'
     *        This function checks if the specified lat and lon value are within the S-LDM coverage area specified
     *        by the user and stored inside the options structure (pointed by m_opts_ptr)
     * 
     * @param lat 
     * @param lon 
     * @return true 
     * @return false 
     */
    bool isInside(double lat, double lon);

    /**
     * @brief This function works as isInside() but returns 'true' only if the specified lat and lon values are located
     *        inside the internal area of the S-LDM (thus excluding the external area)
     * 
     * @param lat 
     * @param lon 
     * @return true 
     * @return false 
     */
    bool isInsideInternal(double lat, double lon);

private:
    struct options *m_opts_ptr{nullptr};
};

#endif // AREAFILTER_H