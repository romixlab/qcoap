#ifndef COAPENDPOINTINFO_H
#define COAPENDPOINTINFO_H

#include <QStringList>

#include "coaplib_global.h"

/**
 * @brief The CoapEndpointInfo class describes CoAP endpoint
 * It is not a part of a standard, but very convenient.
 * For example you can find nodes by their names, regexp
 * by their groups, or both using url extensions like:
 * "coap://lamp-kitchen/value" - find by name
 * "coap://{lamp-.*}/value" - find all the lamps by regexp
 * "coap://[basement]/power-save" - find all the nodes in group "basement"
 * "coap://[basement]{lamp-.*}/value" - find all the lamps in basement
 * For details @see CoapDiscovery
 */
class COAPLIB_SHARED_EXPORT CoapEndpointInfo
{
public:
    /**
     * @brief CoapEndpointInfo Construct an invalid object
     */
    CoapEndpointInfo();
    /**
     * @brief CoapEndpointInfo Construct object with given name and possibly groups
     * @param endpointName
     * @param endpointGroups
     */
    CoapEndpointInfo(const QString &endpointName,
                     const QStringList &endpointGroups = QStringList());
    /**
     * @brief setName Set endpoint name visible from network
     * @param endpointName
     * This name may be used for CoAP node search by name or regexp
     * @see CoapDiscovery
     */
    void setName(const QString &endpointName);
    /**
     * @brief name Name of an endpoint, local or remote
     * @return
     */
    QString name() const;
    /**
     * @brief setGroups Set endpoint groups visible from network
     * @param endpointGroups
     * This groups can be used for CoAP node search by its groups
     * For example all lamps on first floor can be in group floor-on
     * You can then find them all through @see CoapDiscovery
     * using query coap://[floor-one]{lamp-.*}/someresource
     */
    void setGroups(const QStringList &endpointGroups);
    /**
     * @brief groups Groups of an endpoint, local or remote
     * @return
     */
    QStringList groups() const;
    /**
     * @brief isValid
     * @return true if name is not empty
     */
    bool isValid();
};

#endif // COAPENDPOINTINFO_H
