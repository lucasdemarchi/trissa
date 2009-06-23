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
#include "PlayerFactory.h"
#include "common.h"
#include <boost/filesystem/operations.hpp>
#include <fstream>
#include <iostream>

namespace trissa
{

ConfigManager::ConfigManager(int argc, char* argv[]) :
        mConfigFile("Configuration File Options"),
        mCommandLine("Command Line Options"),
        mGameOptions("Game Options"),
        mPlayerFactory( 0 )
{


    setAvailableOptions();

    //parse Command Line
    po::store(po::parse_command_line(argc, argv, mGameOptions), mOptionsMap);
    po::notify( mOptionsMap );
    if ( mOptionsMap.count("version") )
        return;
    if ( mOptionsMap.count("help") )
        return;

    std::string path;
    //If set, the string passed by command line, otherwise,
    //the default value DEFAULT_CONFIG_FILE
    po::notify( mOptionsMap );
    path = mOptionsMap["config"].as<std::string>();
    fs::path full_path(fs::system_complete(path));

    //If config file exist: open, parse and save configs in optionsMap
    if ( fs::exists( full_path ) && !fs::is_directory( full_path )) {
        std::ifstream ifs( full_path.string().c_str() );
        po::store( po::parse_config_file( ifs, mGameOptions ), mOptionsMap );
        po::notify( mOptionsMap );
    }

    mPlayersPath = mOptionsMap["path"].as<std::string>();
    mDimension = mOptionsMap["dimension"].as<unsigned int>();
}

ConfigManager::~ConfigManager()
{
}

void ConfigManager::attachPlayerFactory(PlayerFactory* playerFactory)
{
    mPlayerFactory = playerFactory;
    mPlayerFactory->loadPlayerLibraries(mPlayersPath);
}

void ConfigManager::printVersion() const
{
    using namespace std;
    cout << "Trissa v" << TRISSA_VERSION << endl;
    cout << "Copyright (C) 2008 - Lucas De Marchi" << endl;
    cout << "Distributed under GPLv2" << endl << endl;
}
void ConfigManager::printUsage() const
{
    using namespace std;
    cout << "Usage: trissa [options] " << endl << endl << mCommandLine << endl;
}

ConfigManager::UI_OPTIONS ConfigManager::getUIType() const
{
    std::string ui = mOptionsMap["ui"].as<std::string>();
    if(ui == "text")
        return UI_TEXT;
#ifdef _TRISSA_UI3D_
    else if (ui == "3d" || ui == "3D")
        return UI_3D;
#endif
    return UI_UNAVAILABLE;
}

unsigned int ConfigManager::getDimension() const
{
    return mDimension;
}

void ConfigManager::setDimension(unsigned int dimension)
{
    mDimension = dimension;
}

std::string ConfigManager::getPlayersPath() const
{
    return std::string(mPlayersPath);
}
void ConfigManager::setPlayersPath(std::string path)
{
    mPlayerFactory->unloadPlayerLibraries();
    mPlayerFactory->loadPlayerLibraries(path);
}

std::string ConfigManager::getPlayerA() const
{
    return std::string( mPlayerA );
}
void ConfigManager::setPlayerA(std::string player)
{
    mPlayerA = std::string(player);
}

std::string ConfigManager::getPlayerB() const
{
    return std::string( mPlayerB );
}
void ConfigManager::setPlayerB( std::string player )
{
    mPlayerB = std::string(player);
}

bool ConfigManager::isGetVersion() const
{
    return (mOptionsMap.count("version") >= 1);
}
bool ConfigManager::isGetUsage() const
{
    return (mOptionsMap.count("help") >= 1);
}

void ConfigManager::setAvailableOptions()
{

    mCommandLine.add_options()
    ("help,h", "print this help message and exit")
    ("version,v", "print version number and exit")
    ("config,c", po::value<std::string>()->default_value(DEFAULT_CONFIG_FILE), "set config file to parse")
    ("path,p", po::value<std::string>()->default_value(DEFAULT_PLAYERS_PATH), "Path to search for Player's implementations."
     "This option overrides path from configuration file ")
    ("dimension,d", po::value<unsigned int>(), "Game's dimension. This option overrides that from configuration file")
#ifdef _TRISSA_UI3D_    
    ("ui,u", po::value<std::string>()->default_value(DEFAULT_UI), "Available interfaces: Text ('text' option) and 3D ('ui3d' option)");
#else
    ("ui,u", po::value<std::string>()->default_value(DEFAULT_UI), "Available interfaces: Text ('text' option)");
#endif
    mConfigFile.add_options()
    ("selectedA", po::value<std::string>()->default_value(""), "Selected player A. Must be the same name exported by one"
     "of the player in players' path")
    ("selectedB", po::value<std::string>()->default_value(""), "Selected player B. Must be the same name exported by one"
     "of the player in players' path")
    ;

    mGameOptions.add( mCommandLine ).add( mConfigFile );
}
const char * ConfigManager::DEFAULT_UI("text");
const char * ConfigManager::DEFAULT_CONFIG_FILE ("./trissa.cfg");
const char * ConfigManager::DEFAULT_PLAYERS_PATH("./players/");

}
