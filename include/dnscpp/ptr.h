/**
 *  PTR.h
 *
 *  If you have a Record object that holds a PTR record, you can use
 *  this extra class to extract the value hostname from it.
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
#include "extractor.h"
#include "decompressed.h"

/**
 *  Begin of namespace
 */
namespace DNS {

/**
 *  Class definition
 */
class PTR : public Extractor
{
private:
    /**
     *  The target server name
     *  @var char[]
     */
    Decompressed _target;

public:
    /**
     *  The constructor
     *  @param  response        the response from which the record was extracted
     *  @param  record          the record holding the PTR
     *  @throws std::runtime_error
     */
    PTR(const Response &response, const Record &record) : 
        Extractor(record, TYPE_PTR, 0), 
        _target(response, record.data()) {}
    
    /**
     *  Destructor
     */
    virtual ~PTR() = default;
    
    /**
     *  The target hostname
     *  @return const char *
     */
    const char *target() const
    {
        return _target;
    }
};
    
/**
 *  End of namespace
 */
}

