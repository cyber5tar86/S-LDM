/**
 * @file phpoints.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-07-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef PHPOINTS_H
#define PHPOINTS_H

#include <inttypes.h>
#include <cfloat>
#include <vector>
#include "vehicleDataDef.h"

/**
 * @brief 
 * 
 */
#define INVALID_PHDATA DBL_MAX

/**
 * @brief 
 * 
 */
#define PHDATAITER_INITIALIZER(var) ldmmap::PHpoints::PHDataIter_t var = {.ptrDataArray = NULL}

namespace ldmmap
{
/**
 * @brief 
 * 
 */
class PHpoints
{
public:
    /**
     * @brief Construct a new PHpoints object
     * 
     */
    PHpoints();

    /**
     * @brief Construct a new PHpoints object
     * 
     * @param distance_limit 
     * @param min_dist_m 
     * @param max_dist_m 
     * @param max_heading_diff_degs 
     */
    PHpoints(double distance_limit, double min_dist_m, double max_dist_m, double max_heading_diff_degs);

    /**
     * @brief Destroy the PHpoints object
     * 
     */
    ~PHpoints();

    /**
     * @brief 
     * 
     * @param newVehicleData 
     * @return PHpoints_retval_t 
     */
    PHpoints_retval_t insert(vehicleData_t newVehicleData);

    /**
     * @brief 
     * 
     * @param PHDataIter 
     * @param nextPHData 
     * @return PHpoints_retval_t 
     */
    PHpoints_retval_t iterate(PHDataIter_t &PHDataIter, PHData_t *nextPHData);

    /**
     * @brief 
     * 
     */
    void clear(void);

    /**
     * @brief These are the only two parameters which can be set after construcing a PHpoints objects, as it reserves
     *        a vector whose size depends on m_distance_limit and m_min_dist_m
     * 
     * @param max_distance_meters 
     */
    void setPHMaxDist(double max_distance_meters)
    {
        m_max_dist_m = max_distance_meters;
    }

    /**
     * @brief 
     * 
     * @param max_heading_diff_degs 
     */
    void setPHMaxHeadingDiff(double max_heading_diff_degs)
    {
        m_max_heading_diff_degs = max_heading_diff_degs;
    }

    /**
     * @brief This function returns the value for the newly set "Remove Policy"
     * 
     * @return true 
     * @return false 
     */
    bool switchRemoveOnlyOne(void);

    /**
     * @brief Set the Iterate Full object
     *        This function set the "Iterate Policy" (i.e. sets if "iterate()" should return also the current up-to-date point,
     *        or only points from the Path History itself)
     * 
     * @param iterateFull 
     */
    void setIterateFull(bool iterateFull)
    {
        m_iterateFull = iterateFull;
    }

    int getCardinality(void)
    {
        return m_PHpoints_size;
    };

public:
    /**
     * @brief Information stored for each PH point in this object
     * 
     */
    typedef struct PHData
    {
        double lat;             /*! < */
        double lon;             /*! < */
        double elev;            /*! < */
        double heading;         /*! < */
        double point_distance;  /*! < */
    } PHData_t;

    /**
     * @brief Structure used to iterate over the PH points of a vehicle
     * 
     */
    typedef struct PHDataIter
    {
        /*! @brief */
        std::vector<PHData_t> *ptrDataArray;

        /*! @brief */
        PHData_t data;

        /*! @brief */
        int idx;

        /*! @brief */
        int cyclic_idx;

        /*! @brief */
        int pDataArraySize;
    } PHDataIter_t;

    typedef enum
    {
        PHP_INSERTED,               /*! < */
        PHP_SKIPPED,                /*! < */
        PHP_CONTINUE_ITERATION,     /*! < */
        PHP_TERMINATE_ITERATION,    /*! < */
        PHP_ERROR                   /*! < */
    } PHpoints_retval_t;

private:
    /*! @brief */
    double m_distance_limit;
    /*! @brief */
    double m_min_dist_m;

    /*! @brief */
    double m_max_dist_m;

    /*! @brief */
    double m_max_heading_diff_degs;

    /*
     * PH points management variables
     */

    /*! @brief */
    int m_next_idx;

    /*! @brief */
    int m_oldest_idx;

    /*! @brief */
    double m_stored_distance;

    /*! @brief */
    int m_PHpoints_size;

    /*! @brief */
    int m_vectorReservedSize;

    /*! @brief */
    std::vector<PHData_t> m_pDataArray;

    /**
     * @brief "Remove Policy"
     *        Set to "true" for "remove, if needed, at most one point when a new update arrives"
     *        Set to "false" for "remove, if needed, any number of points until the stored distance is the minimum one >= distance_limit"
     */
    bool m_removeOnlyOne{false};

    /**
     * @brief "Iterate Policy"
     *        Set to "true" to iterate over all the PH points, including the most up-to-date point
     *        Set to "false" to iterate over only the PH History points (the current point data is not returned)
     * 
     */
    bool m_iterateFull{false};
};

} // namespace ldmmap

#endif // PHPOINTS_H