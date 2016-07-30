#define BOOST_ALL_DYN_LINK
#include <iostream>
#include <string>
#include <pthread.h>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/common.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>

//void init()
//{
//    typedef boost::log::sinks::synchronous_sink< boost::log::sinks::text_file_backend > filesink;

//    boost::shared_ptr< filesink > sink = boost::make_shared< filesink >
//    (
//        boost::log::keywords::file_name = "ASHENVALE %Y-%m-%d %H:%M:%S:%f-%N.log",
//        boost::log::keywords::rotation_size = 1024,
//        boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0)
//    );

//    sink->locked_backend()->set_file_collector
//    (
//        boost::log::sinks::file::make_collector
//        (
//            boost::log::keywords::target = "/home/dev/logs"
//        )
//     );

//    sink->set_formatter
//    (
//        boost::log::expressions::format("%1% [%2%]<%3%>: %4%")
//                % boost::log::expressions::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S:%f")
//                % boost::log::expressions::attr< boost::log::attributes::current_thread_id::value_type >("ThreadID")
//                % boost::log::expressions::attr< boost::log::trivial::severity_level >("Severity")
//                % boost::log::expressions::smessage
//    );

//    boost::log::core::get()->add_sink(sink);
//    boost::log::add_common_attributes();
//}

void init()
{
    typedef boost::log::sinks::asynchronous_sink< boost::log::sinks::text_file_backend > filesink;

    boost::shared_ptr< filesink > sink = boost::make_shared< filesink >
            (
                boost::log::keywords::file_name = "%Y-%m-%d %H:%M:%S-%N.log",
                boost::log::keywords::rotation_size = 10 * 1024,
                boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0)
            );
    sink->locked_backend()->set_file_collector
            (
                boost::log::sinks::file::make_collector
                (
                    boost::log::keywords::target = "./logs"
                )
            );
    sink->set_formatter
            (
                boost::log::expressions::format("%1% [%2%]<%3%>: %4%")
                    % boost::log::expressions::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S:%f")
                    % boost::log::expressions::attr< boost::log::attributes::current_thread_id::value_type >("ThreadID")
                    % boost::log::expressions::attr< boost::log::trivial::severity_level >("Severity")
                    % boost::log::expressions::smessage
            );

    boost::log::core::get()->add_sink(sink);
    boost::log::add_common_attributes();
}

void *routine(void *)
{
    for(int j = 0; j < 10; ++j){
        BOOST_LOG_TRIVIAL(info) << "0x" << std::setw(8) << std::setfill('0') << std::setbase(16) << j << " times log  out!";
    }

    return 0;
}

int main(int argc, char *argv[])
{
    pthread_t pid;
    std::string startinfo = "[boostlog start]: ";
    unsigned long x = ~0;
    for(int i = 0; i < argc; ++i){
        startinfo += argv[i];
    }
    init();
    BOOST_LOG_TRIVIAL(info) << "0x" << std::setbase(16) << x <<" Hello hex!";
    BOOST_LOG_TRIVIAL(info) << &startinfo;
    BOOST_LOG_TRIVIAL(info) << startinfo;
    BOOST_LOG_TRIVIAL(fatal) << "A fatal message!";
    BOOST_LOG_TRIVIAL(error) << "A error message!";
    BOOST_LOG_TRIVIAL(debug) << "A debug message!";
    BOOST_LOG_TRIVIAL(info) << "A info message!";

    pthread_create(&pid, 0, routine, 0);


    for(int j = 0; j < 10; ++j){
        BOOST_LOG_TRIVIAL(info) << "0x" << std::setw(8) << std::setfill('0') << std::setbase(16) << j << " times log  out!";
    }

    pthread_join(pid, 0);

    boost::log::core::get()->flush();
    return 0;
}
