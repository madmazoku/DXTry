#include "ConfigClass.h"

#include <windows.h>
#include <processenv.h>
#include <iostream>

ConfigClass::ConfigClass(int argc, char** argv)
{
	m_currentDir = std::filesystem::current_path();
	m_appPath = argv[0];
	m_appDir = m_appPath.parent_path();
}

ConfigClass::~ConfigClass()
{

}
