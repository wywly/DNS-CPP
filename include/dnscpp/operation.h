/**
 *  Operation.h
 * 
 *  When you make a call to Context::query() a pointer to an operation
 *  object is returned. This pointer can be ignored (it is managed
 *  internally by the DNS library. However, you can store it and 
 *  call methods on it (for example when you are no longer interested
 *  in the operation.
 * 
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2020 Copernica BV
 */

/**
 *  Include guard
 */
#pragma once

/**
 *  Begin of namespace
 */
namespace DNS {

/**
 *  Forward declarations
 */
class Handler;

/**
 *  Class definition
 */
class Operation
{
protected:
    /**
     *  The user-space handler
     *  @var Handler
     */
    Handler *_handler;
    
    /**
     *  The query that we're going to send
     *  @var Query
     */
    const Query _query;
    
    /**
     *  Constructor
     *  @param  handler     user space handler
     *  @param  op          the type of operation (normally a regular query)
     *  @param  dname       the domain to lookup
     *  @param  type        record type to look up
     *  @param  bits        extra bits to be included in the query
     *  @param  data        optional data (only for type = ns_o_notify)
     *  @throws std::runtime_error
     */
    Operation(Handler *handler, int op, const char *dname, int type, const Bits &bits, const unsigned char *data = nullptr) : 
        _handler(handler), _query(op, dname, type, bits, data) {}

    /**
     *  Destructor
     */
    virtual ~Operation() = default;

public:
    /**
     *  No copying
     *  @param  that
     */
    Operation(const Operation &that) = delete;

    /**
     *  Expose the original query
     *  @return Query
     */
    const Query &query() const { return _query; }
    
    /**
     *  Change the handler / install a different object to be notified of changes
     *  @param  handler
     */
    void install(Handler *handler)
    {
        // update the handler
        _handler = handler;
    }
    
    /**
     *  Cancel the operation
     */
    void cancel()
    {
        // the object can just be destructed
        delete this;
    }
};

/**
 *  End of namespace
 */
}

