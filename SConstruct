# -*- coding: utf-8 -*-
#SConscript('src/SConscript', variant_dir='build', duplicate=0)
#SConscript('src/players/SConscript', variant_dir='build/players', duplicate=0)

exec(open('custom.py'))

PKG_CONFIG_VERSION='0.22'
OGRE_VERSION='>=1.6.0'
BOOST_VERSION='1.3.0'

###################
# Setup the Options
###################
opts = Variables()
opts.AddVariables(
    ('prefix', 'The install path "prefix"', '/usr/local'),
    #('destdir', 'The root directory to install into. Useful mainly for binary package building', '/'),
    EnumVariable('mode','Defines how Trissa will be built','debug', allowed_values=('optimized', 'optimized_thishost', 'debug',)),
    PathVariable('configfile', 'Path to configuration file', '/etc/trissa.conf', PathVariable.PathAccept),
    EnumVariable('version', 'Auto-update version options', 'auto', allowed_values=('auto','release','keepold')),
    BoolVariable('enable_ui3d','Compile 3D interface',True)
    )
unknown = opts.UnknownVariables()
if unknown:
    print 'Unknown options:', unknown.keys()
    Exit(1)

###################
# Setup Environment
###################
env = Environment(
    CC = 'gcc',
    CXX = 'g++',
    CPPPATH = [ '#src' ],
    LIBS = ['dl','boost_filesystem', 'boost_program_options'],
    LINKFLAGS = ['-rdynamic'],
    
    #CXXCOMSTR = 'Compiling $TARGET',
    #CCCOMSTR = 'Compiling $TARGET',
    #LINKCOMSTR = 'Linking $TARGET',
    
    #SHCXXCOMSTR = 'Compiling $TARGET',
    #SHCCCOMSTR = 'Compiling $TARGET',
    #SHLINKCOMSTR = 'Linking $TARGET',
    variables = opts
    )

#####################
# Generate Help Texts
#####################
Help(opts.GenerateHelpText(env))

if 'help' in COMMAND_LINE_TARGETS:
    print opts.GenerateHelpText(env)
    Exit(0)

if not env.GetOption('clean') and not 'tags' in COMMAND_LINE_TARGETS:
    version = create_new_version(env['version'])

print 'Trissa ' + version

#######
# Set the compile mode
#######
if env['mode'] == 'debug':
    print 'Debug mode enabled'
    env['CCFLAGS'] += ['-ggdb', '-pg','-Wall', '-g3', '-D_TRISSA_DEBUG_']
    env['LINKFLAGS'] += ['-pg']
elif env['mode'] == 'optimized':
    print 'Optimized mode enabled'
    env['CCFLAGS'] = ['-O3', '-Wall', '-fPIC']
elif env['mode'] == 'optimized_thishost':
    print 'Optmized-for-this-host mode enabled'
    env['CCFLAGS'] = ['-O3', '-Wall', '-fPIC']
    optimize_for_cpu(env)


#######
# List of all the required packages to build 3D UI
#######
packages_ui3d = {
    'OGRE' : {
        'check' : 'OGRE ' + OGRE_VERSION,
        'libcheck': 'OgreMain',
        'headercheck': 'Ogre.h',
        'msg' : 'Error: Ogre3d and development files are required to use 3D UI',
        'web' : 'http://www.ogre3d.org'
    },
        
    'OIS' : {
        'check': '',
        'libcheck': 'OIS',
        'headercheck': 'OISPrereqs.h',
        'msg' : 'Error: OIS needed to use 3D UI',
        'web' : 'http://sourceforge.net/projects/wgois/'
    },
        
    'CEGUI' : {
        'check' : '',
        'libcheck': 'CEGUIBase',
        'headercheck': 'CEGUI.h',
        'msg' : 'Error: CEGUI needed to use 3D UI',
        'web' : 'http://www.cegui.org.uk/'
    },
    'CEGUI-OGRE' : {
        'check' : '',
        'libcheck': 'CEGUIOgreRenderer',
        'headercheck': 'OgreCEGUIRenderer.h',
        'msg' : 'Error: CEGUI-OGRE needed to use 3D UI',
        'web' : 'http://www.ogre3d.org'
    }
}

#######
# Export the environment
#######
if 'players' in COMMAND_LINE_TARGETS:
    subdirs = ['src/players']
else:
    subdirs = ['src','src/players']
    if env['enable_ui3d']:
        env['CCFLAGS']+=['-DEXT_HASH', '-D_TRISSA_UI3D_']
        if not env.GetOption('clean'):
            Config(env, packages_ui3d)

#Boost is needed by players, ui3d and core:
if not env.GetOption('clean'):
    conf = Configure(env, custom_tests = { 'CheckBoost' : CheckBoost })
    if not (conf.CheckBoost(BOOST_VERSION)):
        print 'Boost version >=' + BOOST_VERSION + ' is needed'
        Exit(1)

headers=[]
objs=[]
sharedobjs=[]
Export('env version subdirs headers objs sharedobjs COMMAND_LINE_TARGETS opts')

#######
# Parse subdirectories
#######
for subdir in subdirs:
    print 'Parsing sub directory ' + subdir
    SConscript('%s/SConscript' % subdir)

print ''

if 'tags' in COMMAND_LINE_TARGETS:
    print 'Making ctags...'
    GenerateTags()
    Exit(0)

if not 'configure' in COMMAND_LINE_TARGETS:
    #VariantDir('build','src')
    trissa = env.Program('build/trissa', objs)

#env.Alias ("install", env.Install(os.path.join(prefix, "share/trissa"), players)
#env.Alias ("install", env.Install(os.path.join(prefix, "bin"), trissa)
