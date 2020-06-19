/**
 *  ExtendedRequest.h
 * 
 *  Class that encapsulates all data that is needed for a single ExtendedRequest
 * 
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2020 Copernica BV
 */

/**
 *  Include guard
 */
#pragma once

/**
 *  Dependencies
 */
#include <memory>
#include "../include/dnscpp/request.h"
#include "../include/dnscpp/nameserver.h"
#include "../include/dnscpp/timer.h"
#include "query.h"
#include "connection.h"
#include "now.h"

/**
 *  Begin of namespace
 */
namespace DNS {

/**
 *  Forward declarations
 */
class Core;
class Handler;

/**
 *  Class definition
 */
class ExtendedRequest : 
    public Request,
    private Nameserver::Handler, 
    private Connection::Handler, 
    private Timer
{
private:
    /**
     *  Pointer to the core of the DNS library
     *  @var Core
     */
    Core *_core;

    /**
     *  The query that we're going to send
     *  @var Query
     */
    Query _query;
    
    /**
     *  When was the ExtendedRequest started?
     *  @var Now
     */
    Now _started;
    
    /**
     *  Identifier of the timer
     *  @var void*
     */
    void *_timer;

    /**
     *  If we got a truncated response, we start a tcp connection to get the full response
     *  @var Connection
     */
    std::unique_ptr<Connection> _connection;
    

    /**
     *  Method that is called when a dgram response is received
     *  @param  nameserver  the reporting nameserver
     *  @param  response    the received response
     */
    virtual void onReceived(Nameserver *nameserver, const Response &response) override;

    /**
     *  Called when the response has been received over tcp
     *  @param  connection
     *  @param  response
     */
    virtual void onReceived(Connection *connection, const Response &response) override;
    
    /**
     *  Called when the connection could not be used
     *  @param  connector
     */
    virtual void onFailure(Connection *connection) override;
    
    /**
     *  Method that is called by user space when the timer expires
     */
    virtual void expire() override;

    /**
     *  When does the ExtendedRequest expire?
     *  @return double
     */
    double expires() const;

    /** 
     *  Time out the ExtendedRequest because no appropriate response was received in time
     */
    void timeout();

    /**
     *  Retry / send a new message to the nameservers
     *  @param  now     current time
     */
    void retry(double now);

    /**
     *  Private destructor, the class is self-destructing
     */
    virtual ~ExtendedRequest();

    /**
     *  Method that can be called to cancel the request.
     */
    virtual void cancel() override;

public:
    /**
     *  Constructor
     *  @param  core        dns core object
     *  @param  domain      the domain of the lookup
     *  @param  type        type of records to look for
     *  @param  handler     user space object interested in the result
     */
    ExtendedRequest(Core *core, const char *domain, ns_type type, DNS::Handler *handler);
    
    /**
     *  No copying
     *  @param  that
     */
    ExtendedRequest(const ExtendedRequest &ExtendedRequest) = delete;
};

/**
 *  End of namespace
 */
}