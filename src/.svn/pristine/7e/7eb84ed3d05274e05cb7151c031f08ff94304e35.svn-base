// ---------------------------------------------------------------------
// pion:  a Boost C++ framework for building lightweight HTTP interfaces
// ---------------------------------------------------------------------
// Copyright (C) 2007-2014 Splunk Inc.  (https://github.com/splunk/pion)
//
// Distributed under the Boost Software License, Version 1.0.
// See http://www.boost.org/LICENSE_1_0.txt
//

#ifndef __PION_HTTP_CONNECTION_HEADER__
#define __PION_HTTP_CONNECTION_HEADER__

#ifdef PION_HAVE_SSL
    #if defined(__APPLE__)
        // suppress warnings about OpenSSL being deprecated in OSX
        #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    #endif
    #include <boost/asio/ssl.hpp>
#endif

#include "pion/tcp/connection.hpp"
#include "pion/http/request_reader.hpp"

namespace pion {    // begin namespace pion
namespace http {     // begin namespace http

//class request_reader;
//typedef boost::shared_ptr<pion::http::request_reader> request_reader_ptr;

///
/// connection: represents a single http connection
/// 
class connection :
	public pion::tcp::connection,
    //public boost::enable_shared_from_this<connection>,
    private boost::noncopyable
{
public:
   
    /**
     * creates new shared connection objects
     *
     * @param io_service asio service associated with the connection
     * @param ssl_context asio ssl context associated with the connection
     * @param ssl_flag if true then the connection will be encrypted using SSL 
     * @param finished_handler function called when a server has finished
     *                         handling the connection
     */
    static inline boost::shared_ptr<connection> create(boost::asio::io_service& io_service,
                                                          ssl_context_type& ssl_context,
                                                          const bool ssl_flag, size_t max_content_length,
                                                          connection_handler finished_handler,
														  request_reader::finished_handler_t request_handler)
    {
		auto conn = boost::shared_ptr<connection>(new connection(io_service, ssl_context,
                                                                  ssl_flag, finished_handler));
		conn->m_reader_ptr = request_reader::create(conn, request_handler);
		conn->m_reader_ptr->set_max_content_length(max_content_length);
		return conn;
    }
    
    /**
     * creates a new connection object
     *
     * @param io_service asio service associated with the connection
     * @param ssl_flag if true then the connection will be encrypted using SSL 
     */
    explicit connection(boost::asio::io_service& io_service, const bool ssl_flag = false)
        : pion::tcp::connection(io_service, ssl_flag)
	{ }
    
    /**
     * creates a new connection object for SSL
     *
     * @param io_service asio service associated with the connection
     * @param ssl_context asio ssl context associated with the connection
     */
    connection(boost::asio::io_service& io_service, ssl_context_type& ssl_context)
        : pion::tcp::connection(io_service, ssl_context)
    { }
    
	boost::shared_ptr<connection> shared_from_this()
	{
		return boost::static_pointer_cast<connection>(pion::tcp::connection::shared_from_this());
	}

	request_reader_ptr& get_reader()
	{
		return m_reader_ptr;
	}

	virtual void close(void) {
		pion::tcp::connection::close();
		m_reader_ptr.reset();
	}
protected:
        
    /**
     * protected constructor restricts creation of objects (use create())
     *
     * @param io_service asio service associated with the connection
     * @param ssl_context asio ssl context associated with the connection
     * @param ssl_flag if true then the connection will be encrypted using SSL 
     * @param finished_handler function called when a server has finished
     *                         handling the connection
     */
    connection(boost::asio::io_service& io_service,
                  ssl_context_type& ssl_context,
                  const bool ssl_flag,
                  connection_handler finished_handler)
        : pion::tcp::connection(io_service, ssl_context, ssl_flag, finished_handler)
    {
    }
    

private:
	request_reader_ptr m_reader_ptr;
};


/// data type for a connection pointer
typedef boost::shared_ptr<connection>    connection_ptr;


}   // end namespace http
}   // end namespace pion

#endif
