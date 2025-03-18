#pragma once

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <string>
#include <sstream>
#include <unordered_map>

#if defined(_WIN32)  
	#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#elif defined(__linux__)
	#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#define	LOG_TRACE(logger) \
	BOOST_LOG_SEV(logger, boost::log::trivial::severity_level::trace) << __FILENAME__ << ":" << std::setiosflags(std::ios::left) << std::setw(5) << __LINE__ << " | "
#define	LOG_DEBUG(logger) \
	BOOST_LOG_SEV(logger, boost::log::trivial::severity_level::debug) << __FILENAME__ << ":" << std::setiosflags(std::ios::left) << std::setw(5) << __LINE__ << " | "
#define	LOG_INFO(logger) \
	BOOST_LOG_SEV(logger, boost::log::trivial::severity_level::info) << __FILENAME__ << std::setiosflags(std::ios::left) << std::setw(5) << ":" << __LINE__ << " | "
#define	LOG_WARNING(logger) \
	BOOST_LOG_SEV(logger, boost::log::trivial::severity_level::warning) << __FILENAME__ << std::setiosflags(std::ios::left) << std::setw(5) << ":" << __LINE__ << " | "
#define	LOG_ERROR(logger) \
	BOOST_LOG_SEV(logger, boost::log::trivial::severity_level::error) << __FILENAME__ << std::setiosflags(std::ios::left) << std::setw(5) << ":" << __LINE__ << " | "
#define	LOG_FATAL(logger) \
	BOOST_LOG_SEV(logger, boost::log::trivial::severity_level::fatal) << __FILENAME__ << std::setiosflags(std::ios::left) << std::setw(5) << ":" << __LINE__ << " | "

class Logger
{
public:
	static Logger& getInstance();
	static boost::log::sources::severity_logger<boost::log::trivial::severity_level>& get();
	void toggleLogging(bool option);
	void setLogPath(const std::string logPath);
	void setFilterSeverity(const std::string& severityLevel);

private:
	Logger();
	~Logger() = default;
	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;

	boost::log::trivial::severity_level getFilterSeverity(const std::string& severityLevel);
	std::string generateFilename() const;

private:
	bool m_enableLogging;
	std::string m_severityLevel;
	boost::log::sources::severity_logger<boost::log::trivial::severity_level> m_slg;
};
