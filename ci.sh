#!/bin/bash

# echo commands
set -x

STM32CUBEIDE_BUILD_TARGET=Debug


# This is generic (so why not put in the docker image?)

STM32CUBEIDE_PATH=${STM32CUBEIDE_PATH:-/opt/st/stm32cubeide_1.4.0}
STM32CUBEIDE_BUILD_TARGET=${STM32CUBEIDE_BUILD_TARGET:-all}

# Run build
# MDM: I had to remove the `-data .` parameter before import for the build to begin. 
${STM32CUBEIDE_PATH}/stm32cubeide --launcher.suppressErrors -nosplash -application org.eclipse.cdt.managedbuilder.core.headlessbuild  -import . -build ${STM32CUBEIDE_BUILD_TARGET} -no-indexer
build_ret=$?
exit $build_ret
