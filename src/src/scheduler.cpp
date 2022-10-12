// ---------------------------------------------------------------------
// pion:  a Boost C++ framework for building lightweight HTTP interfaces
// ---------------------------------------------------------------------
// Copyright (C) 2007-2014 Splunk Inc.  (https://github.com/splunk/pion)
//
// Distributed under the Boost Software License, Version 1.0.
// See http://www.boost.org/LICENSE_1_0.txt
//

#include <boost/exception/diagnostic_information.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <pion/scheduler.hpp>

namespace pion {    // begin namespace pion


// static members of scheduler
    
const boost::uint32_t   scheduler::DEFAULT_NUM_THREADS = 8;
const boost::uint32_t   scheduler::NSEC_IN_SECOND = 1000000000; // (10^9)
const boost::uint32_t   scheduler::MICROSEC_IN_SECOND = 1000000;    // (10^6)

// scheduler member functions

void scheduler::shutdown(void)
{
    // lock mutex for thread safety
    boost::mutex::scoped_lock scheduler_lock(m_mutex);
    
    if (m_is_running) {

        PION_LOG_INFO(m_logger, "Shutting down the thread scheduler");

        while (m_active_users > 0) {
            // first, wait for any active users to exit
            PION_LOG_INFO(m_logger, "Waiting for " << m_active_users << " scheduler users to finish");
            m_no_more_active_users.wait(scheduler_lock);
        }

        // shut everything down
        m_is_running = false;
    }

    stop_services();
    stop_threads();
    finish_services();
    finish_threads();
        
    PION_LOG_INFO(m_logger, "The thread scheduler has shutdown");

    // Make sure anyone waiting on shutdown gets notified
    // even if the scheduler did not startup successfully
    m_scheduler_has_stopped.notify_all();
}

void scheduler::join(void)
{
    boost::mutex::scoped_lock scheduler_lock(m_mutex);
    while (m_is_running) {
        // sleep until scheduler_has_stopped condition is signaled
        m_scheduler_has_stopped.wait(scheduler_lock);
    }
}
    
void scheduler::add_active_user(void)
{
    if (++m_active_users == 1)
        if (!m_is_running)
            startup();
}

void scheduler::remove_active_user(void)
{
    if (--m_active_users == 0)
        m_no_more_active_users.notify_all();
}

boost::system_time scheduler::get_wakeup_time(boost::uint32_t sleep_sec,
    boost::uint32_t sleep_nsec)
{
    return boost::get_system_time() + boost::posix_time::seconds(sleep_sec) + boost::posix_time::microseconds(sleep_nsec / 1000);
}
                     
void scheduler::process_service_work(boost::asio::io_service& service) {
    boost::asio::io_service::work work(service);
    while (m_is_running) {
        try {
            service.run();
        } catch (std::exception& e) {
            PION_LOG_ERROR(m_logger, boost::diagnostic_information(e));
        } catch (...) {
            PION_LOG_ERROR(m_logger, "caught unrecognized exception");
        }
    }   
}
                     

// single_service_scheduler member functions

void single_service_scheduler::startup(void)
{
    // lock mutex for thread safety
    boost::mutex::scoped_lock scheduler_lock(m_mutex);
    
    if (! m_is_running) {
        PION_LOG_INFO(m_logger, "Starting thread scheduler");
        m_is_running = true;
        
        // schedule a work item to make sure that the service doesn't complete
        m_service.reset();
        
        // start multiple threads to handle async tasks
        for (boost::uint32_t n = 0; n < m_num_threads; ++n) {
            boost::shared_ptr<boost::thread> new_thread(new boost::thread( boost::bind(&scheduler::process_service_work,
                                                                                       this, boost::ref(m_service)) ));
            m_thread_pool.push_back(new_thread);
        }
    }
}

    
// one_to_one_scheduler member functions

void one_to_one_scheduler::startup(void)
{
    // lock mutex for thread safety
    boost::mutex::scoped_lock scheduler_lock(m_mutex);
    
    if (! m_is_running) {
        PION_LOG_INFO(m_logger, "Starting thread scheduler");
        m_is_running = true;
        
        // make sure there are enough services initialized
        while (m_service_pool.size() < m_num_threads) {
            boost::shared_ptr<service_pair_type>  service_ptr(new service_pair_type());
            m_service_pool.push_back(service_ptr);
        }

        // start multiple threads to handle async tasks
        for (boost::uint32_t n = 0; n < m_num_threads; ++n) {
            boost::shared_ptr<boost::thread> new_thread(new boost::thread( boost::bind(&scheduler::process_service_work,
                                                                                       this, boost::ref(m_service_pool[n]->first)) ));
            m_thread_pool.push_back(new_thread);
        }
    }
}

    
}   // end namespace pion
