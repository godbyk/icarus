# vim:shiftwidth=4:tabstop=4:expandtab:filetype=python:nowrap

import SCons.Script
import sys, platform, os, glob

def CheckPKGConfig(context, version):
    context.Message('Checking for pkg-config... ')
    ret = context.TryAction('pkg-config --atleast-pkgconfig-version=%s' % version)[0]
    context.Result(ret)
    return ret
              
def CheckFlagpoll(context, version):
    context.Message('Checking for flagpoll... ')
    ret = context.TryAction('flagpoll flagpoll --atleast-version=%s' % version)[0]
    context.Result(ret)
    return ret

def CheckPKG(context, name):
    context.Message('Checking for %s... ' % name)
    ret = context.TryAction('pkg-config --exists \'%s\'' % name)[0]
    context.Result(ret)
    return ret

def CheckVRJConfig(context, version):
    context.Message('Checking for vrjuggler-config... ')
    ret = context.TryAction('vrjuggler-config --version')
    # FIXME check returned version number
    # print "vrjuggler-config --version returned '%s'" % ret[1]
    context.Result(ret[0])
    return ret[0]

def CheckOSGConfig(context, version):
    context.Message('Checking for osg-config... ')
    ret = context.TryAction('osg-config --version')
    # FIXME check returned version number
    # print "osg-config --version returned '%s'" % ret[1]
    context.Result(ret[0])
    return ret[0]

def CheckGDALConfig(context, version):
    context.Message('Checking for gdal-config... ')
    ret = context.TryAction('gdal-config --version')
    # FIXME check returned version number
    # print "gdal-config --version returned '%s'" % ret[1]
    context.Result(ret[0])
    return ret[0]

def CheckVPRConfig(context, version):
    context.Message('Checking for vpr-config... ')
    ret = context.TryAction('vpr-config --version')
    # FIXME check returned version number
    # print "vpr-config --version returned '%s'" % ret[1]
    context.Result(ret[0])
    return ret[0]

def CheckDependencies():
    global env, conf
    #conf = Configure(env, custom_tests = { 'CheckPKGConfig' : CheckPKGConfig,
    #                                       'CheckPKG' : CheckPKG })

    if not conf.CheckPKGConfig('0.15.0'):
        print 'pkg-config >= 0.15.0 not found.'
        Exit(1)

    # Check for OpenSceneGraph installation
    if not conf.CheckLib('osg'):
        print 'libosg.so not found.'
        print 'Download and install OpenSceneGraph from http://www.openscenegraph.org/.'
        Exit(1)
    LIBS.extend('osg osgDB osgGA osgUtil'.split())

    if not conf.CheckLib('ode'):
        print 'libode.so not found.'
        print 'Download and install ODE from ___.'  # FIXME
        Exit(1)
    LIBS.extend(['ode'])

    #if not conf.CheckLib('opal'):
    #    print 'libopal.so not found.'
    #    print 'Download and install OPAL from ___.' # FIXME
    #    Exit(1)
    #LIBS.extend(['opal'])

    if not conf.CheckLib('opal-ode'):
        print 'libopal-ode.so not found.'
        print 'Download and install OPAL from http://opal.sourceforge.net/.' # FIXME
        Exit(1)
    LIBS.extend(['opal-ode'])

    # Check for VR Juggler installation
    if not conf.CheckFlagpoll('0.8.1'):
        print 'flagpoll >= 0.8.1 not found.'
        print 'Download Flagpoll from https://realityforge.vrsource.org/view/FlagPoll/WebHome.'
        Exit(1)
    env.ParseConfig('flagpoll --libs --cflags vrjuggler --get-vrj-ogl-libs')
    env.ParseConfig('flagpoll --libs --cflags vpr')
    env.ParseConfig('flagpoll --libs --cflags sonix')

    # Check for OpenCV installation
    if not conf.CheckPKG("opencv >= 0.9.7"):
        print 'opencv >= 0.9.7 not found.'
        print 'Download and install OpenCV from http://opencvlibrary.sourceforge.net/.'
        Exit(1)
    env.ParseConfig('pkg-config --cflags --libs opencv')

def BuildLinuxEnvironment():
    "Builds a base environment for other modules to build on set up for linux"
    global env, LIBS
    env = Environment(ENV=os.environ)

    # FIXME hard-coded greencan install dir
    CXXFLAGS = [
        '-fmessage-length=0', 
        '-Wall', 
        '-fexceptions', 
        '-Wunused', 
        '-Wno-conversion', 
        '-O2', 
        '-Iinclude'
        ]
    LINKFLAGS = []

    # Check dependencies
    CheckDependencies()

    # Check for 'debug = 1'
    debug = ARGUMENTS.get('debug', 0)
    if int(debug):
        # Enable debugging
        CXXFLAGS.extend(['-g', '-pg'])
        CXXFLAGS.extend(['-D_DEBUG', '-DOSG_DEBUG'])   # for the vrj app
        LINKFLAGS.extend(['-g', '-pg'])
    else:
        # Disable debugging
        CXXFLAGS.extend(['-DNDEBUG'])

    # Check for 'strict = 1'
    strict = ARGUMENTS.get('strict', 0)
    if int(strict):
        # Enable -Werror
        CXXFLAGS.extend(['-Werror'])

    base_dir = os.path.abspath('./')
    CPPPATH = [os.path.join(base_dir, 'src')]
    # FIXME hard-coded greencan install dir
    LIBPATH = ['/usr/local/lib', '/usr/lib', os.path.join(base_dir, 'lib')]

    env.Append(
        CXXFLAGS    = CXXFLAGS,
        LINKFLAGS   = LINKFLAGS,
        LIBPATH     = LIBPATH,
        CPPPATH     = CPPPATH,
        LIBS        = LIBS
    )

    return env


def CheckTargets():
    all_args = sys.argv[1:]
    parser = SCons.Script.OptParser()
    options, targets = parser.parse_args(all_args)

    if ('fixme' in targets):
        print "Print list of FIXMEs:"
        #os.system('grep -nr -E "FIXME|TODO|XXX|BROKEN" src/ | grep -v "/\."');
        grep = os.popen('grep -nr -E "FIXME|TODO|XXX|BROKEN" src/ | grep -v "/\."')
        grep_results = grep.read()
        todo = open("TODO", "w")
        todo.write(grep_results)
        todo.close()
        print grep_results
        Exit(0)
    if ('doc' in targets or 'docs' in targets):
        print "Generating documentation..."
        BuildDocumentation()


# Create options
opts = Options('opts.cache')
env = Environment(ENV = os.environ, options = opts)
opts.Add(BoolOption('debug', 'Generate debug code', 0))
opts.Add(BoolOption('strict', 'Treats compile warnings as errors', 0))
opts.Add(PathOption('prefix', 'Location to install Icarus', '/usr/local', PathOption.PathIsDirCreate))
opts.Update(env)
opts.Save('opts.cache', env)
Help(opts.GenerateHelpText(env))
opts_dict = env.Dictionary()

# Check command-line targets
CheckTargets()

# Register custom dependency checks
conf = Configure(env, custom_tests = { 'CheckPKGConfig' : CheckPKGConfig,
                                       'CheckFlagpoll' : CheckFlagpoll,
                                       'CheckOSGConfig' : CheckOSGConfig,
                                       'CheckVRJConfig' : CheckVRJConfig,
                                       'CheckVPRConfig' : CheckVPRConfig,
                                       'CheckPKG' : CheckPKG })


LIBS = []
baseEnv = BuildLinuxEnvironment()

#build_dir = 'build.' + platform.system() + '-' + platform.machine()
build_dir = 'bin'

# Export options and other vars
prefix     = opts_dict['prefix']
bindir     = os.path.normpath(os.path.join(prefix, 'bin'))
libdir     = os.path.normpath(os.path.join(prefix, 'lib', 'icarus'))
dbdir      = os.path.normpath(os.path.join(prefix, 'share', 'icarus'))
includedir = os.path.normpath(os.path.join(prefix, 'include', 'icarus'))
    
Export('baseEnv build_dir LIBS opts_dict bindir libdir dbdir includedir')

BuildDir(build_dir, 'src', duplicate=1) # XXX duplicate was 0
SConscript(build_dir + '/SConscript')
