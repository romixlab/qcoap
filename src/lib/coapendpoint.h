#ifndef COAPENDPOINT_H
#define COAPENDPOINT_H

#include <QObject>
#include <QHostAddress>
#include "coaplib_global.h"
#include "coapendpoint.h"
#include "coapendpointinfo.h"
#include "coapexchangeparameters.h"
#include "coappdu.h"
#include <QHostAddress>
#include <QUrl>

class CoapEndpointPrivate;
/** @file */
/**
 * @brief The CoapEndpoint class
 * Handles all the CoAP communications
 */
class COAPLIB_SHARED_EXPORT CoapEndpoint : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief CoapEndpoint Initialize endpoint
     * @param endpointType Client or ClientServer @see Type
     * @param endpointName Name of this endpoint, useful when there is more that one endpoint
     * @param parent
     * Note that endpointName is used only to find this endpoint later by calling Coap::endpoint("name")
     * CoapEndpointInfo::name() is in contrast used in communications with other CoAP nodes
     */
    CoapEndpoint(Type endpointType, const QString &endpointName = QStringLiteral("default"), QObject *parent = 0);
    /**
     * @brief ~CoapEndpoint Destroy everything and close sockets
     * If requestCertificate() was previously called, then before destroying multicast message is sended out,
     * to group provision, indicating that endpoint is shutting down
     */
    virtual ~CoapEndpoint();
    /**
     * @brief bind Bind on specific interface and port
     * @param address QHostAddress::LocalHost for example
     * @param port
     * @return true on success
     * If port is 0 then 5683 will be used in a case of ClientServer mode and some free port in a case of Client mode.
     * Note that multicast interface is configured using @see bindMulticast method
     */
    bool bind(const QHostAddress &address = QHostAddress::Any, quint16 port = 0);

    /**
     * @brief bindMulticast Bind for multicast exchanges
     * @param groupAddress
     * @param iface
     * @return true on success
     * Note that enabling multicast without any protection is not recommended by RFC7252 11.3
     */
    bool bindMulticast(const QHostAddress & groupAddress, const QNetworkInterface & iface);

    /**
     * @brief The Type enum
     */
    enum Type {
        Client, /**< Run on ane free port available */
        ClientServer /**< Run on 5683 port */
    };

    /**
     * @brief setEndpointInfo Provide an info about this endpoint
     * @param endpointInfo Class that contains endpoint name, endpoint groups, etc
     */
    void setEndpointInfo(const CoapEndpointInfo &endpointInfo);

    /**
     * @brief setTransmissionParameters Configure transmissions timeouts, speed, etc
     * @param parameters See CoAP section 4.8
     */
    void setExchangeParameters(const CoapExchangeParameters &parameters);

    /**
     * @brief setCertificate Set certificate for encrypting all the traffic
     * @param certificateFile PKCS file containing root ca certificate, endpoint certificate and key
     */
    void setCertificate(const QString &certificateFile);

    /**
     * @brief startProvision initiates a provision process.
     * If certificate is already exist it will be loaded through setCertificate(),
     * otherwise multicast request to group "provision" will be maded,
     * then administrator will decide issue the certificate to specific endpoint or not.
     * Additionally it creates several resources, that used in provision.
     * Loads exchange parameters from exchangeparameters.json and changes them through
     * resource /_provision/exchange-parameters.
     * /_provision/certificate is used for certificate reception and further updates.
     *
     * Note that bindMulticast() or routeRequestsToProxy() must be called for this method to work,
     * and that provision server must be running somewhere in your network
     * @todo Provision server
     * @todo certificate is stored in unencrypted way, and may be stolen, need a solution here.
     */
    void startProvision();

    /**
     * @brief routeRequestsToProxy Perform all coap:// and coaps:// exchanges through proxy
     * @param proxyAddress
     * This is very useful on mobile platforms, when device may be used
     * in private network (where all the other CoAP nodes are) or from outside,
     * in this case all exchanges will be transparently routed to proxy
     * Note that proxy must be configured properly for this method to function
     * @todo Such a proxy
     */
    void routeRequestsToProxy(const QUrl &proxyAddress);
protected:
    CoapEndpointPrivate * d_ptr;
    CoapEndpoint(CoapEndpointPrivate &dd, QObject *parent);

private:
    Q_DECLARE_PRIVATE(CoapEndpoint)
    Q_PRIVATE_SLOT(d_func(), void _q_state_changed(QAbstractSocket::SocketState))
    Q_PRIVATE_SLOT(d_func(), void _q_ready_read())
    Q_PRIVATE_SLOT(d_func(), void _q_error(QAbstractSocket::SocketError))
};

#endif // COAPENDPOINT_H
