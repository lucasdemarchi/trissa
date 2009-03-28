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
#ifndef _CONFIGMANAGER_H_
#define _CONFIGMANAGER_H_ 1

#include <string>
#include <boost/program_options.hpp>
#include <boost/filesystem/path.hpp>


namespace trissa
{
namespace po = boost::program_options;
namespace fs = boost::filesystem;

class PlayerFactory;

/** This $name class manage all settings for Trissa.
*
*/
class ConfigManager
{
public:
    static const char * DEFAULT_UI; /**< Default UI to be used when there's no configuration about it */
    static const char * DEFAULT_CONFIG_FILE; /**< Default config file to be used when not specified in command line */
    static const char * DEFAULT_PLAYERS_PATH; /**< Default path where to look for Players classes */

    /** Constructor
      * @param argc Parameter <i>argc</i> received from main, which must be passed to this class in order to parse command line
      * @param argv Parameter <i>argv</i> received from main, which must be passed to this class in order to parse command line
    */
    ConfigManager( int argc, char* argv[] );

    ~ConfigManager();

    /** In loading phase, this is used to actually pass a PlayerFactory to ConfigManager
      * @remarks Current value of mPlayersPath is used to populate mPlayerFactory with players
      * @param playerFactory a pointer to PlayerFactory (the only 1 that is instantiated at any given time),
      * instantiated by Game
      */
    void attachPlayerFactory(PlayerFactory* playerFactory);

    /** Print Trissa's version to standard output */
    void printVersion() const;

    /** Print Trissa's version plus usage options to standard output */
    void printUsage() const;

    //Getters and setters
    /** Get dimension of game
      * @return Dimension
      */
    unsigned int getDimension() const;
    /** Set dimenions of game
      * @remarks To be called when parsing command line, getting configuration from config file or whether there's
      * an option in GUI, before starting the game, to set the dimension
      */
    void setDimension(unsigned int dimension);

    /** Get path where to look for Players classes
      */
    std::string getPlayersPath() const;

    /** Set Player's path
      * @remarks Destroy all currently instantiated players, close all dynamic libraries and search for libraries in
      * path
      * @param path to location where to look for dynamic libraries, i.e. players. Attention that is not checked if
      * current path is the same that was already loaded
      */
    void setPlayersPath(std::string path);

    /** Get PlayerA's name
      * @remarks Must be the same name used by Player to register itself in PlayerFactory
      * @note Limitation: there's no two players with same name
      */
    std::string getPlayerA() const;
    /** Set PlayerA's name
      * @remarks To be called when parsing command line, getting configuration from config file or whether there's
      * an option in GUI, before game begins, to select player
      */
    void setPlayerA(std::string player);

    /** Get PlayerA's name
      * @remarks Must be the same name used by Player to register itself in PlayerFactory
      * @note Limitation: there's no two players with same name
      */
    std::string getPlayerB() const;
    /** Set PlayerB's name
      * @remarks To be called when parsing command line, getting configuration from config file or whether there's
      * an option in GUI, before game begins, to select player.
      */
    void setPlayerB(std::string player);

    /** Test if option to print usage is set by command line option
      * @return True if option is set, false otherwise
      */
    bool isGetUsage() const;
    /** Test if option to print Trissa's version is set by command line option
      * @return True if option is set, false otherwise
      */
    bool isGetVersion() const;

private:
    /** Member function responsible to set class' attribute of available options either for command line or config file
      *
      */
    void setAvailableOptions();

    po::options_description mConfigFile; /**< Attribute that holds available options to be set in config file*/
    po::options_description mCommandLine; /**< Attribute that holds available options to be set as command line arguments*/
    po::options_description mGameOptions; /**< Attribute that holds available options*/

    /** A map that holds pairs of type <key,value> for all available options either in command line or in config file
      * Each option is a key and values are that set by command line, config file and GUI
      */
    po::variables_map mOptionsMap;

    unsigned int mDimension; /**< Attibute that holds game's dimension */
    std::string mPlayersPath; /**< Attibute that holds player's path */
    std::string mPlayerA; /**< Attribute that holds selected player for PlayerA*/
    std::string mPlayerB; /**< Attribute that holds selected player for PlayerB*/

    PlayerFactory *mPlayerFactory; /**< Attribute that holds a pointer to PlayerFactory*/

};

}


#endif
