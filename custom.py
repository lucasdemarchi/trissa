# -*- coding: utf-8 -*-
import os
import sys
import string
import re
import SCons

version='0.98'
#try:
#    f = open("subversion", 'r')
#    subversion = f.readline().strip()
#    f.close()
#except:
subversion = '0'

def create_new_version(autoversion):
#    if autoversion == 'auto':
#        f = open('subversion', 'w')
#        s = str(int(subversion) + 1) + '\n'
#        f.write(s)
#        f.close()
#        return version + '.' + s
#    elif autoversion == 'keepold':
#        return version + '.' + subversion
#    else:
     return version

#Taken from Gazebo
def optimize_for_cpu(env):
    cpuAtr = ['mmx', 'sse', 'sse2', 'sse3', '3dnow']
    line_atr =""
    if (os.path.isfile("/proc/cpuinfo")):
        f_proc = open("/proc/cpuinfo")
        for line in f_proc:
            if (re.match('flags', line)):
                for atr in cpuAtr:
                    if (re.search(atr, line) and not re.search(atr, line_atr)):
                        line_atr+= "-m" + atr + " "
    env['CCFLAGS'] += Split (line_atr)
    print "detected CPU atributes: " +line_atr


def CheckPkgConfig(context, version):
    context.Message( 'Checking for pkg-config... ' )
    ret = context.TryAction('pkg-config --atleast-pkgconfig-version=%s' % version)[0]
    context.Result( ret )
    return ret

def CheckPkg(context, name):
    context.Message('Configuring path for %s...' % name)

    pkg_config_cmd = 'PKG_CONFIG_PATH=' + context.env['PKG_CONFIG_PATH'] + ' pkg-config'
    action = '%s --exists \'%s\'' % (pkg_config_cmd, name);

    ret = context.TryAction(action)[0]
    if(ret):
        action = pkg_config_cmd + " --libs --cflags " + name
        try:
            context.env.ParseConfig(pkg_config_cmd + " --libs --cflags " + name)
        except:
            ret=0

    context.Result(ret)
    return ret


def Config(env, packages):
    conf = Configure(env, custom_tests = {  'CheckPkgConfig' : CheckPkgConfig,
                                            'CheckPkg' : CheckPkg })
    if not conf.CheckPkgConfig(PKG_CONFIG_VERSION):
        print '  Error: pkg-config version >= ' + PKG_CONFIG_VERSION + ' not found.'
        Exit(1)
    
    for key in packages:
        check = packages[key]['check']
        libcheck = packages[key]['libcheck']
        headercheck = packages[key]['headercheck']
        msg = packages[key]['msg']
        web = packages[key]['web']
        if conf.CheckPkg(key):
            print 'Trying to link to %s with header %s... ' % (libcheck,headercheck)
            if not conf.CheckLibWithHeader(libcheck, headercheck, 'C++'):
                print 'fail'
                print 'Unable to link to lib or find required header'
                Exit(1)
        else: 
            print '  !! ' + msg + key + ' not found.'
            print '  See: ' + web
            Exit(1)
   
    env = conf.Finish()

def GenerateTags():    
    os.system('ctags -R --c++-kinds=+p --fields=+iaS --extra=+q src/')

def GenerateCscope():
	os.system('cscope -b -R -I./players/ -I./ui3d/ -I/usr/include/OGRE/ -I/usr/include/')

def CheckBoost(context, version):
    # Boost versions are in format major.minor.subminor
    v_arr = version.split(".")
    version_n = 0
    if len(v_arr) > 0:
        version_n += int(v_arr[0])*100000
    if len(v_arr) > 1:
        version_n += int(v_arr[1])*1000
    if len(v_arr) > 2:
        version_n += int(v_arr[2])

    context.Message('Checking for Boost version >= %s... ' % (version))
    ret = context.TryRun("""
        #include <boost/version.hpp>

        int main() 
        {
            return BOOST_VERSION >= %d ? 0 : 1;
        }
    """ % version_n, '.cpp')[0]
    context.Result(ret)
    return ret

