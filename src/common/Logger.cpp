#include "Logger.hpp"

Logger::Logger()
	: m_enableLogging(false)
	, m_severityLevel("trace")
{}

Logger& Logger::getInstance()
{
	static Logger instance;
	return instance;
}

boost::log::sources::severity_logger<boost::log::trivial::severity_level>& Logger::get()
{
	return Logger::getInstance().m_slg;
}

void Logger::toggleLogging(bool option)
{
	m_enableLogging = option;
}

void Logger::setLogPath(const std::string logPath)
{
	boost::log::add_file_log
	(
		logPath + generateFilename(),
		boost::log::keywords::format = boost::log::expressions::format("[%1%] %2%: [%3%:%4%] %5%")
			% boost::log::expressions::max_size_decor<char>(26)[boost::log::expressions::stream << std::setw(26) << boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f")]
			% boost::log::expressions::max_size_decor<char>(7)[boost::log::expressions::stream << std::setw(7) << boost::log::trivial::severity]
			% boost::log::expressions::attr<boost::log::attributes::current_process_id::value_type>("ProcessID")
			% boost::log::expressions::attr<boost::log::attributes::current_thread_id::value_type>("ThreadID")
			% boost::log::expressions::smessage,
		boost::log::keywords::auto_flush = true
	);
	boost::log::add_common_attributes();
}

void Logger::setFilterSeverity(const std::string& severityLevel)
{
	boost::log::core::get()->set_filter(boost::log::trivial::severity >= getFilterSeverity(severityLevel));
}

boost::log::trivial::severity_level Logger::getFilterSeverity(const std::string& severityLevel)
{
	static const std::unordered_map<std::string, boost::log::trivial::severity_level> m_filterMap =
	{
		{ "trace", boost::log::trivial::trace },
		{ "debug", boost::log::trivial::debug },
		{ "info", boost::log::trivial::info },
		{ "warning", boost::log::trivial::warning },
		{ "error", boost::log::trivial::error },
		{ "fatal", boost::log::trivial::fatal }
	};

	try 
	{
		m_severityLevel = severityLevel;
		return m_filterMap.at(severityLevel);
	}
	catch(...)
	{
		m_severityLevel = "trace";
		return m_filterMap.at("trace");
	}
}

std::string Logger::generateFilename() const
{
	std::string filename = m_severityLevel + "_";
	boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
	std::stringstream ss;
	boost::posix_time::time_facet* facet = new boost::posix_time::time_facet();
	facet->format("%Y-%m-%d_%H-%M-%S");
	ss.imbue(std::locale(std::locale::classic(), facet));
	ss << now;
	filename = filename + ss.str() + ".log";
	return filename;
}
