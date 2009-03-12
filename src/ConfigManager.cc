/*
 * ConfigManager.cc
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
#include "ConfigManager.h"
#include <boost/filesystem/operations.hpp>
#include <fstream>
#include <iostream>

namespace trissa {

ConfigManager::ConfigManager(std::string path, int argc, char* argv[]) :
    configFile("Configuration File Options"),
    commandLine("Command Line Options"),
    gameOptions("Game Options") {

    fs::path full_path(fs::system_complete(path));

    commandLine.add_options()
        ("help,h", "print this help message")
        ("vesion,v", "print version number")
        ;

    configFile.add_options()
        ("path,p", po::value<std::string>(), "Path to search for Player's implementations."
                                             "This option overrides path from configuration file ")
        ("dimension,d", po::value<int>(), "Game's dimension. This option overrides that from configuration file")
        ("selectedA", po::value<std::string>()->default_value(""), "Selected player A. Must be the same name exported by one"
                                                                    "of the player in players' path")
        ("selectedB", po::value<std::string>()->default_value(""), "Selected player B. Must be the same name exported by one"
                                                                    "of the player in players' path")
        ;

    gameOptions.add( commandLine ).add( configFile );

    po::store(po::parse_command_line(argc, argv, gameOptions), optionsMap);

    if( fs::exists( full_path ) && !fs::is_directory( full_path )){
        std::ifstream ifs( full_path.string().c_str() );
        po::store( po::parse_config_file( ifs, gameOptions ), optionsMap );
    }
    po::notify( optionsMap );
}

ConfigManager::~ConfigManager(){
}

void ConfigManager::printUsage(){
    std::cout << gameOptions;
}


}
