/**
 *  Context.cpp
 * 
 *  Implementation file for the Context class
 * 
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2020 Copernica BV
 */

/**
 *  Dependencies
 */
#include "../include/dnscpp/context.h"
#include "remotelookup.h"
#include "locallookup.h"

/**
 *  Begin of namespace
 */
namespace DNS {

/**
 *  Do a dns lookup
 *  @param  name        the record name to look for
 *  @param  type        type of record (normally you ask for an 'a' record)
 *  @param  bits        bits to include in the query
 *  @param  handler     object that will be notified when the query is ready
 *  @return Operation   object to interact with the operation while it is in progress
 */
Operation *Context::query(const char *domain, ns_type type, const Bits &bits, Handler *handler)
{
    // for A and AAAA lookups we also check the /etc/hosts file
    if (type == ns_t_a    && _hosts.lookup(domain, 4)) return new LocalLookup(_loop, _hosts, domain, type, handler);
    if (type == ns_t_aaaa && _hosts.lookup(domain, 6)) return new LocalLookup(_loop, _hosts, domain, type, handler);
    
    // the request can throw (for example when the domain is invalid
    try
    {
        // we are going to create a self-destructing request
        return new RemoteLookup(this, domain, type, bits, handler);
    }
    catch (...)
    {
        // invalid parameters were supplied
        return nullptr;
    }
}

/**
 *  Do a reverse IP lookup, this is only meaningful for PTR lookups
 *  @param  ip          the ip address to lookup
 *  @param  bits        bits to include in the query
 *  @param  handler     object that will be notified when the query is ready
 *  @return operation   object to interact with the operation while it is in progress
 */
Operation *Context::query(const Ip &ip, const Bits &bits, Handler *handler) 
{
    // if the /etc/hosts file already holds a record
    if (_hosts.lookup(ip)) return new LocalLookup(_loop, _hosts, ip, handler);

    // pass on to the regular query method
    return query(Reverse(ip), TYPE_PTR, bits, handler);
}

/**
 *  Do a dns lookup and pass the result to callbacks
 *  @param  name        the record name to look for
 *  @param  type        type of record (normally you ask for an 'a' record)
 *  @param  bits        bits to include in the query
 *  @param  success     function that will be called on success
 *  @param  failure     function that will be called on failure
 *  @return operation   object to interact with the operation while it is in progress
 */
Operation *Context::query(const char *domain, ns_type type, const Bits &bits, const SuccessCallback &success, const FailureCallback &failure)
{
    // use a self-destructing wrapper for the handler
    return query(domain, type, bits, new Callbacks(success, failure));
}

/**
 *  Do a reverse dns lookup and pass the result to callbacks
 *  @param  ip          the ip address to lookup
 *  @param  bits        bits to include in the query
 *  @param  success     function that will be called on success
 *  @param  failure     function that will be called on failure
 *  @return operation   object to interact with the operation while it is in progress
 */
Operation *Context::query(const DNS::Ip &ip, const Bits &bits, const SuccessCallback &success, const FailureCallback &failure)
{
    // use a self-destructing wrapper for the handler
    return query(ip, bits, new Callbacks(success, failure));
}

/**
 *  End of namespace
 */
}

