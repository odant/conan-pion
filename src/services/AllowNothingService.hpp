// ---------------------------------------------------------------------
// pion:  a Boost C++ framework for building lightweight HTTP interfaces
// ---------------------------------------------------------------------
// Copyright (C) 2007-2014 Splunk Inc.  (https://github.com/splunk/pion)
//
// Distributed under the Boost Software License, Version 1.0.
// See http://www.boost.org/LICENSE_1_0.txt
//

#ifndef __PION_ALLOW_NOTHING_SERVICE_HEADER__
#define __PION_ALLOW_NOTHING_SERVICE_HEADER__

#include <pion/http/plugin_service.hpp>


namespace pion {        // begin namespace pion
namespace plugins {     // begin namespace plugins
        
///
/// This class has a corresponding create function (pion_create_AllowNothingService) and
/// destroy function (pion_destroy_AllowNothingService), as required for use by plugin.
/// 
class AllowNothingService : public pion::http::plugin_service
{
public:
    AllowNothingService(void) {}
    ~AllowNothingService() {}
    virtual void operator()(const pion::http::request_ptr& http_request_ptr,
                            const pion::tcp::connection_ptr& tcp_conn);
};
    
}   // end namespace plugins
}   // end namespace pion

#endif
