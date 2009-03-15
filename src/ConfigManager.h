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

namespace trissa {
    namespace po = boost::program_options;
    namespace fs = boost::filesystem;

    class ConfigManager {
    public:
        static const char * DEFAULT_CONFIG_FILE;
        static const char * DEFAULT_PLAYERS_PATH;

        ConfigManager( int argc, char* argv[] );
        ~ConfigManager();

        void printVersion() const;
        void printUsage() const;

        //Getters and setters
        unsigned int getDimension() const;
        void setDimension(unsigned int dimension);

        std::string getPlayersPath() const;
        //TODO:
        //void setPlayersPath(std::string path);

        std::string getPlayerA() const;
        void setPlayerA(std::string player);

        std::string getPlayerB() const;
        void setPlayerB(std::string player);

        bool isGetUsage() const;
        bool isGetVersion() const;

    private:
        po::options_description mConfigFile;
        po::options_description mCommandLine;
        po::options_description mGameOptions;
        po::variables_map mOptionsMap;

        unsigned int mDimension;
        std::string mPlayersPath;
        std::string mPlayerA;
        std::string mPlayerB;

        bool mGetUsage;
        bool mGetVersion;

        void setAvailableOptions();
    };

}


#endif
