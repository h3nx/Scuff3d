#pragma once
#include <string>
#include <map>
#include <filesystem>



namespace scuff3d {
	class Settings {
	public:
		Settings(const std::string fileName, const bool saveOnClose = true);
		~Settings();
		bool loadFromFile();
		bool saveToFile();

		bool write(const std::string& name, const float& value);
		bool write(const std::string& name, const int& value);
		bool write(const std::string& name, const std::string& value);

		bool read(const std::string& name, float& value, float defaultValue = 0.0f);
		bool read(const std::string& name, int& value, int defaultValue = 0);
		bool read(const std::string& name, std::string& value, std::string defaultValue = "0");

		const int readInt(const std::string& name, const int defaultValue = 0);
		const float readFloat(const std::string& name, const float defaultValue = 0.0f);
		const std::string readString(const std::string& name, const std::string defaultValue = "0");


		const std::string toString();
		const std::string getFileName();

	private:
		std::string m_fileName;
		bool m_saveOnClose;

		std::map<std::string, std::string> m_data;

		bool exists(const std::string& name);
		bool createFile();
	};

}


