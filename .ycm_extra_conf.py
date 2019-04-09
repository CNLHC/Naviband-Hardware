from distutils.sysconfig import get_python_inc
import platform
import os
import subprocess
import ycm_core

DIR_OF_THIS_SCRIPT = os.path.abspath( os.path.dirname( __file__ ) )
SOURCE_EXTENSIONS = [ '.cpp', '.cxx', '.cc', '.c', '.m', '.mm' ]

flags = [
'--std=c11',
'-Wc11-extensions',
'-x',
'c',
'-DUSE_HAL_DRIVER',
'-DSTM32F103xB',
'-IInc/',
'-IDrivers/STM32F1xx_HAL_Driver/Inc/',
'-IDrivers/STM32F1xx_HAL_Driver/Inc/Legacy',
'-IDrivers/CMSIS/Device/ST/STM32F1xx/Include',
'-IDrivers/CMSIS/Include',

'-IMiddlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM3',
'-IMiddlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/',
'-IMiddlewares/Third_Party/FreeRTOS/Source/include/',
]

def IsHeaderFile( filename ):
  extension = os.path.splitext( filename )[ 1 ]
  return extension in [ '.h', '.hxx', '.hpp', '.hh' ]


def FindCorrespondingSourceFile( filename ):
  if IsHeaderFile( filename ):
    basename = os.path.splitext( filename )[ 0 ]
    for extension in SOURCE_EXTENSIONS:
      replacement_file = basename + extension
      if os.path.exists( replacement_file ):
        return replacement_file
  return filename


def PathToPythonUsedDuringBuild():
  try:
    filepath = os.path.join( DIR_OF_THIS_SCRIPT, 'PYTHON_USED_DURING_BUILDING' )
    with open( filepath ) as f:
      return f.read().strip()
  # We need to check for IOError for Python 2 and OSError for Python 3.
  except ( IOError, OSError ):
    return None


def Settings( **kwargs ):
  language = kwargs[ 'language' ]

  if language == 'cfamily':
    # If the file is a header, try to find the corresponding source file and
    # retrieve its flags from the compilation database if using one. This is
    # necessary since compilation databases don't have entries for header files.
    # In addition, use this source file as the translation unit. This makes it
    # possible to jump from a declaration in the header file to its definition
    # in the corresponding source file.
    filename = FindCorrespondingSourceFile( kwargs[ 'filename' ] )

    return {
    'flags': flags,
    'include_paths_relative_to_dir': DIR_OF_THIS_SCRIPT,
    'override_filename': filename
    }

  if language == 'python':
    return {
      'interpreter_path': PathToPythonUsedDuringBuild()
    }

  return {}


def GetStandardLibraryIndexInSysPath( sys_path ):
  for index, path in enumerate( sys_path ):
    if os.path.isfile( os.path.join( path, 'os.py' ) ):
      return index
  raise RuntimeError( 'Could not find standard library path in Python path.' )

