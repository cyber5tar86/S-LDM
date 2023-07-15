#ifndef VEHICLEDATADEF_H
#define VEHICLEDATADEF_H

#include <unordered_map>
#include <vector>
#include <shared_mutex>


/**
 * @brief 
 * 
 */
#define vehicleDataVector_t(name) std::vector<ldmmap::vehicleData_t> name;

/**
 * @brief Facility macro to convert from DEG to RAD
 * 
 */
#define DEG_2_RAD(val) ((val)*M_PI / 180.0)

namespace ldmmap
{
/**
 * @brief Class to store optional data
 *        If the data is not available, m_available is 'false' and no actual data is stored (getData() does not return any meaningful data)
 *        If the data is available (isAvailable() returns 'true'), then the actual data can be retrieved with getData()
 * 
 * @tparam T 
 */
template <class T>
class OptionalDataItem
{
public:
    /**
     * @brief Construct a new Optional Data Item object
     * 
     */
    OptionalDataItem() {}

    /**
     * @brief Construct a new Optional Data Item object
     * 
     * @param data 
     */
    explicit OptionalDataItem(T data) :
        m_dataitem(data),
        m_available(true)
    {}

    /**
     * @brief Construct a new Optional Data Item object
     * 
     * @param availability 
     */
    explicit OptionalDataItem(bool availability) :
        m_available(availability)
    {}

    /**
     * @brief Get the Data object
     * 
     * @return T 
     */
    T getData()
    {
        return m_dataitem;
    }

    /**
     * @brief 
     * 
     * @return true 
     * @return false 
     */
    bool isAvailable()
    {
        return m_available;
    }

    /**
     * @brief Set the Data object
     * 
     * @param data 
     * @return T 
     */
    T setData(T data)
    {
        m_dataitem = data;
        m_available = true;
    }

private:
    /*! @brief */
    bool m_available{false};

    /*! @brief */
    T m_dataitem;
};

/**
 * @brief 
 * 
 */
typedef enum StationTypeLDM
{
    StationType_LDM_unknown = 0,                    /*! < */
    StationType_LDM_pedestrian = 1,                 /*! < */
    StationType_LDM_cyclist = 2,                    /*! < */
    StationType_LDM_moped = 3,                      /*! < */
    StationType_LDM_motorcycle = 4,                 /*! < */
    StationType_LDM_passengerCar = 5,               /*! < */
    StationType_LDM_bus = 6,                        /*! < */
    StationType_LDM_lightTruck = 7,                 /*! < */
    StationType_LDM_heavyTruck = 8,                 /*! < */
    StationType_LDM_trailer = 9,                    /*! < */
    StationType_LDM_specialVehicles = 10,           /*! < */
    StationType_LDM_tram = 11,                      /*! < */
    StationType_LDM_roadSideUnit = 15,              /*! < */
    StationType_LDM_specificCategoryVehicle1 = 100, /*! < */
    StationType_LDM_specificCategoryVehicle2 = 101, /*! < */
    StationType_LDM_specificCategoryVehicle3 = 102, /*! < */
    StationType_LDM_specificCategoryVehicle4 = 103, /*! < */
    StationType_LDM_specificCategoryVehicle5 = 104, /*! < */
    StationType_LDM_specificCategoryVehicle6 = 105, /*! < */
    StationType_LDM_specificCategoryVehicle7 = 106, /*! < */
    StationType_LDM_specificCategoryVehicle8 = 107, /*! < */
    StationType_LDM_specificCategoryVehicle9 = 108, /*! < */
    StationType_LDM_detectedPedestrian = 110,       /*! < */
    StationType_LDM_detectedPassengerCar = 115,     /*! < */
    StationType_LDM_detectedTruck = 117,            /*! < */
    StationType_LDM_unspecified = 120               /*! < */
} e_StationTypeLDM;

/**
 * @brief This structure contains all the data stored in the database for each vehicle (except for the PHPoints)
 * 
 */
typedef struct vehicleData
{
    /*! @brief */
    uint64_t stationID;
    /*! @brief */
    double lat;

    /*! @brief */
    double lon;

    /*! @brief */
    double elevation;

    /*! @brief Heading between 0 and 360 degrees */
    double heading;

    /*! @brief */
    double speed_ms;

    /*! @brief */
    uint64_t gnTimestamp;

    /*! @brief This is the CAM message GenerationDeltaTime */
    long camTimestamp;

    /*! @brief */
    uint64_t timestamp_us;

    /*! @brief */
    uint64_t on_msg_timestamp_us;

    /*! @brief */
    OptionalDataItem<long> vehicleWidth;

    /*! @brief */
    OptionalDataItem<long> vehicleLength;

    /*! @brief */
    e_StationTypeLDM stationType;

    /*! @brief */
    std::string sourceQuadkey;

    /*
     * Low frequency container data
     */

    /*! @brief Bit string with exterior lights status*/
    OptionalDataItem<uint8_t> exteriorLights; // 
} vehicleData_t;

} // namespace ldmmap

#endif // VEHICLEDATADEF_H
