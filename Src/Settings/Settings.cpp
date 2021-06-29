#include "pch.h"
#include "Settings.h"
#include <fstream>
#include <ostream>


namespace scuff3d {

	#define LOGGING true
	#if LOGGING
		#define LOG(message) DEVLOG(message)
	#else
		#define LOG(message)
	#endif

	Settings::Settings(const std::string fileName, const bool saveOnClose)
	{
		m_fileName = fileName;
		m_saveOnClose = saveOnClose;
		loadFromFile();
	}

	Settings::~Settings()
	{
		if (m_saveOnClose) {
			saveToFile();
		}
	}

	bool Settings::loadFromFile()
	{
		LOG("Loading from (" + m_fileName + ")");

		std::string fileContent = "";
		readFile(m_fileName, fileContent);
		
		while (fileContent.size() > 0) {
			size_t pos = fileContent.find('\n');
			std::string line = fileContent.substr(0, pos);
			fileContent.erase(0, pos+1);
			size_t delim = line.find('=');
			std::string name = line.substr(0, delim);
			std::string value = line.substr(delim+1);

			m_data[name] = value;
			LOG("\t" + name + "=" + value);
		}



		LOG("Done Loading\n");
		return false;
	}

	bool Settings::saveToFile()
	{
		std::string asString = toString();
#ifdef _DEBUG
		std::string debugString = asString;
		replaceAll(debugString, "\n", "\n\t");
		LOG("Writing to file ("+m_fileName+")\n\t" + debugString);

#endif
		if (scuff3d::toFile(m_fileName, asString, std::ofstream::trunc)) {

		}
		else {
			LOG("could not save to ("+m_fileName+")");
		}
		
		return true;
	}


	bool Settings::write(const std::string& name, const float& value)
	{
		return write(name, std::to_string(value));
	}

	bool Settings::write(const std::string& name, const int& value)
	{
		return write(name, std::to_string(value));
	}

	bool Settings::write(const std::string& name, const std::string& value)
	{
		m_data[name] = value;
		return false;
	}


	bool Settings::read(const std::string& name, float& value, float defaultValue)
	{
		if (exists(name)) {
			value = std::stof(m_data[name]);
			return true;
		}
		else {
			value = defaultValue;
			write(name, defaultValue);
		}
		return false;
	}

	bool Settings::read(const std::string& name, int& value, int defaultValue)
	{
		if (exists(name)) {
			value = std::stoi(m_data[name]);
			return true;
		}
		else {
			value = defaultValue;
			write(name, defaultValue);
		}
		return false;
	}

	bool Settings::read(const std::string& name, std::string& value, std::string defaultValue)
	{
		if (exists(name)) {
			value = m_data[name];
			return true;
		}
		else {
			value = defaultValue;
			write(name, defaultValue);
		}
		return false;
	}


	const int Settings::readInt(const std::string& name, const int defaultValue)
	{
		return std::stoi(readString(name, std::to_string(defaultValue)));
	}

	const float Settings::readFloat(const std::string& name, const float defaultValue)
	{
		return std::stof(readString(name, std::to_string(defaultValue)));
	}

	const std::string Settings::readString(const std::string& name, const std::string defaultValue)
	{
		if (exists(name)) {
			return m_data[name];
		}
		else {
			write(name, defaultValue);
		}
		return defaultValue;
	}


	const std::string Settings::toString()
	{
		std::string output;
		for (std::pair<std::string, std::string> value : m_data) {
			output += value.first + "=" + value.second + "\n";
		}
		return output;
	}

	bool Settings::exists(const std::string& name)
	{
		return m_data.find(name) != m_data.end();
	}

	bool Settings::createFile()
	{
		return false;
	}


}

