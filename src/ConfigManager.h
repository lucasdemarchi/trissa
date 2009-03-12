/*
 * ConfigManager.h
 * This file is part of Trissa
 *
 * Copyright (C) 2008 - Lucas De Marchi
 *
 * Trissa is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * Trissa is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Trissa; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */
#ifndef CONFIGMANAGER_H_
#define CONFIGMANAGER_H_

#include <string>
#include <boost/program_options.hpp>
#include <boost/filesystem/path.hpp>

namespace trissa{
    namespace po = boost::program_options;
    namespace fs = boost::filesystem;

class ConfigManager {
public:
    ConfigManager(std::string path, int argc, char* argv[]);
    ~ConfigManager();

    void printUsage();
private:
    po::options_description configFile;
    po::options_description commandLine;
    po::options_description gameOptions;
    po::variables_map optionsMap;
};

}


#endif
