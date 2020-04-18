#pragma once

#include <filesystem>

class ConfigClass
{
public:
	ConfigClass(int argc, char** argv);
	~ConfigClass();

	const std::filesystem::path& getAppDir() const { return m_AppDir; }

private:
	std::filesystem::path m_AppPath;
	std::filesystem::path m_AppDir;
};

