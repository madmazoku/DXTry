#pragma once

#include <filesystem>

class ConfigClass
{
public:
	ConfigClass(int argc, char** argv);
	~ConfigClass();

	const std::filesystem::path pathToShader(const std::string& shaderName) const { return m_appDir / (shaderName + ".cso"); }
	const std::filesystem::path pathToDDS(const std::string& ddsName) const { return m_currentDir / (ddsName + ".dds"); }

private:
	std::filesystem::path m_currentDir;
	std::filesystem::path m_appPath;
	std::filesystem::path m_appDir;
};

