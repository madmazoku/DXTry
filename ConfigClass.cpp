#include "ConfigClass.h"

#include <windows.h>
#include <processenv.h>
#include <iostream>

ConfigClass::ConfigClass(int argc, char** argv)
{
	m_AppPath = argv[0];
	m_AppDir = m_AppPath.parent_path();
}

ConfigClass::~ConfigClass()
{

}
