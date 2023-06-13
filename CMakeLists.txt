cmake_minimum_required(VERSION 3.16.0)

#
## finalcut source/header files
#
file(GLOB finalcut_SRC
    "src/final/dialog/*.cpp"
    "src/final/dialog/*.h"
    "src/final/font/*.h"
    "src/final/input/*.cpp"
    "src/final/input/*.h"
    "src/final/menu/*.cpp"
    "src/final/menu/*.h"
    "src/final/output/*.cpp"
    "src/final/output/*.h"
    "src/final/output/tty/*.cpp"
    "src/final/output/tty/*.h"
    "src/final/util/*.cpp"
    "src/final/util/*.h"
    "src/final/vterm/*.cpp"
    "src/final/vterm/*.h"
    "src/final/widget/*.cpp"
    "src/final/widget/*.h"
    "src/final/*.cpp"
    "src/final/*.h"
)

option(F_COMPILE_EXAMPLES "Compile examples" off)
option(F_COMPILE_STATIC "Compile finalcut static library" off)
option(F_CMAKE_OPTS "" on)

message(CHECK_START "Compile examples")
if (F_COMPILE_EXAMPLES)
    message(CHECK_PASS "enabled")
else ()
    message(CHECK_PASS "disabled")
endif (F_COMPILE_EXAMPLES)
message(CHECK_START "Static library")
if (F_COMPILE_STATIC)
    message(CHECK_PASS "enabled")
else ()
    message(CHECK_PASS "disabled")
endif (F_COMPILE_STATIC)

# Cross-compile options
option(CROSS_COMPILE_ARM "Cross-compile for 32-bit ARM" off)
option(CROSS_COMPILE_AARCH64 "Cross-compile for 64-bit ARM" off)
option(CROSS_COMPILE_RPI_ARM "Cross-compile for (old RPi) 32-bit ARM" off)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(ARCH amd64)
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE amd64)

message(CHECK_START "Cross compiling for 32-bit ARM")
if (CROSS_COMPILE_ARM)
    set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
    set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
    set(ARCH arm)
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE arm)
    message(CHECK_PASS "yes")
else ()
    message(CHECK_PASS "no")
endif (CROSS_COMPILE_ARM)
message(CHECK_START "Cross compiling for 64-bit ARM")
if (CROSS_COMPILE_AARCH64)
    set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
    set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)
    set(ARCH arm64)
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE arm64)
    message(CHECK_PASS "yes")
else ()
    message(CHECK_PASS "no")
endif (CROSS_COMPILE_AARCH64)

option(WITH_RPI_ARM_TOOLS "Specifies the location for the RPI ARM tools" off)
if (WITH_RPI_ARM_TOOLS)
    set(RPI_ARM_TOOLS ${WITH_RPI_ARM_TOOLS})
    message(CHECK_START "With RPi 1 Tools: ${RPI_ARM_TOOLS}")
endif (WITH_RPI_ARM_TOOLS)

message(CHECK_START "Cross compiling for (old RPi) 32-bit ARM")
if (CROSS_COMPILE_RPI_ARM)
    if (NOT WITH_RPI_ARM_TOOLS)
        message("-- Cloning legacy Raspberry Pi compilation toolchain")
        Include(FetchContent)
        FetchContent_Declare(
            RPiTools
            GIT_REPOSITORY https://github.com/raspberrypi/tools.git
        )
        FetchContent_MakeAvailable(RPiTools)
        set(CMAKE_C_COMPILER ${CMAKE_CURRENT_BINARY_DIR}/_deps/rpitools-src/arm-bcm2708/arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc)
        set(CMAKE_CXX_COMPILER ${CMAKE_CURRENT_BINARY_DIR}/_deps/rpitools-src/arm-bcm2708/arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++)
    else()
        set(CMAKE_C_COMPILER ${RPI_ARM_TOOLS}/arm-bcm2708/arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc)
        set(CMAKE_CXX_COMPILER ${RPI_ARM_TOOLS}/arm-bcm2708/arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++)
    endif ()

    set(ARCH armhf)
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE armhf)
    message(CHECK_PASS "yes")
else ()
    message(CHECK_PASS "no")
endif (CROSS_COMPILE_RPI_ARM)

# Standard CMake options
if (F_CMAKE_OPTS)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY .)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif(NOT CMAKE_BUILD_TYPE)
message(CHECK_START "Build Type is ${CMAKE_BUILD_TYPE}")
if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -Wall -fexceptions -std=c++14")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -fexceptions -std=c++14")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -g -O0 -Wall -fexceptions -std=c++14")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g -O0 -Wall -fexceptions -std=c++14")
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O3 -Wall -fexceptions -std=c++14 -s")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3 -Wall -fexceptions -std=c++14 -s")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -g -O3 -Wall -fexceptions -std=c++14 -s")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g -O3 -Wall -fexceptions -std=c++14 -s")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O3 -Wall -fexceptions -std=c++14")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3 -Wall -fexceptions -std=c++14")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -g -O3 -Wall -fexceptions -std=c++14")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g -O3 -Wall -fexceptions -std=c++14")
endif()
if (CROSS_COMPILE_ARM)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-psabi")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-psabi")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-psabi")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wno-psabi")
endif (CROSS_COMPILE_ARM)

set(CMAKE_INSTALL_PREFIX "/usr/local")
endif ()

add_definitions(-DF_VERSION="0.9.0")
add_definitions(-DCOMPILE_FINAL_CUT)

#
## finalcut project
#
project(finalcut)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
find_package(Curses REQUIRED)

if (F_COMPILE_STATIC)
    add_library(finalcut ${finalcut_SRC})
else ()
    add_library(finalcut SHARED ${finalcut_SRC})
endif (F_COMPILE_STATIC)
target_include_directories(finalcut PRIVATE src)
target_link_libraries(finalcut PRIVATE ncurses)

#
## example sources
#
if (F_COMPILE_EXAMPLES)
file(GLOB 7segmentExample_SRC
    "src/examples/7segment.cpp"
)
file(GLOB backgroundColorExample_SRC
    "src/examples/background-color.cpp"
)
file(GLOB busyExample_SRC
    "src/examples/busy.cpp"
)
file(GLOB calculatorExample_SRC
    "src/examples/calculator.cpp"
)
file(GLOB checklistExample_SRC
    "src/examples/checklist.cpp"
)
file(GLOB choiceExample_SRC
    "src/examples/choice.cpp"
)
file(GLOB dialogExample_SRC
    "src/examples/dialog.cpp"
)
file(GLOB eventLogExample_SRC
    "src/examples/event-log.cpp"
)
file(GLOB fullwidthCharacterExample_SRC
    "src/examples/fullwidth-character.cpp"
)
file(GLOB helloExample_SRC
    "src/examples/hello.cpp"
)
file(GLOB highlightTextExample_SRC
    "src/examples/highlight-text.cpp"
)
file(GLOB inputDialogExample_SRC
    "src/examples/input-dialog.cpp"
)
file(GLOB keyboardExample_SRC
    "src/examples/keyboard.cpp"
)
file(GLOB listboxExample_SRC
    "src/examples/listbox.cpp"
)
file(GLOB listviewExample_SRC
    "src/examples/listview.cpp"
)
file(GLOB mandelbrotExample_SRC
    "src/examples/mandelbrot.cpp"
)
file(GLOB menuExample_SRC
    "src/examples/menu.cpp"
)
file(GLOB mouseExample_SRC
    "src/examples/mouse.cpp"
)
file(GLOB optiMoveExample_SRC
    "src/examples/opti-move.cpp"
)
file(GLOB parallaxScrollingExample_SRC
    "src/examples/parallax-scrolling.cpp"
)
file(GLOB rotozoomerExample_SRC
    "src/examples/rotozoomer.cpp"
)
file(GLOB scrollviewExample_SRC
    "src/examples/scrollview.cpp"
)
file(GLOB stringOperationsExample_SRC
    "src/examples/string-operations.cpp"
)
file(GLOB termAttributesExample_SRC
    "src/examples/term-attributes.cpp"
)
file(GLOB termcapExample_SRC
    "src/examples/termcap.cpp"
)
file(GLOB terminalFocusExample_SRC
    "src/examples/terminal-focus.cpp"
)
file(GLOB timerExample_SRC
    "src/examples/timer.cpp"
)
file(GLOB transparentExample_SRC
    "src/examples/transparent.cpp"
)
file(GLOB treeviewExample_SRC
    "src/examples/treeview.cpp"
)
file(GLOB uiExample_SRC
    "src/examples/ui.cpp"
)
file(GLOB watchExample_SRC
    "src/examples/watch.cpp"
)
file(GLOB windowsExample_SRC
    "src/examples/windows.cpp"
)
file(GLOB xpmviewExample_SRC
    "src/examples/xpmview.cpp"
    "src/examples/xpmimage.h"
)

#
## 7segment example project
#
project(7segment)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_executable(7segment ${7segmentExample_SRC})
target_link_libraries(7segment PRIVATE finalcut)
target_include_directories(7segment PRIVATE src)

#
## background-color example project
#
project(background-color)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_executable(background-color ${backgroundColorExample_SRC})
target_link_libraries(background-color PRIVATE finalcut)
target_include_directories(background-color PRIVATE src)

#
## busy example project
#
project(busy)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_executable(busy ${busyExample_SRC})
target_link_libraries(busy PRIVATE finalcut)
target_include_directories(busy PRIVATE src)

#
## calculator example project
#
project(calculator)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_executable(calculator ${calculatorExample_SRC})
target_link_libraries(calculator PRIVATE finalcut)
target_include_directories(calculator PRIVATE src)

#
## checklist example project
#
project(checklist)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_executable(checklist ${checklistExample_SRC})
target_link_libraries(checklist PRIVATE finalcut)
target_include_directories(checklist PRIVATE src)

#
## choice example project
#
project(choice)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_executable(choice ${choiceExample_SRC})
target_link_libraries(choice PRIVATE finalcut)
target_include_directories(choice PRIVATE src)

#
## dialog example project
#
project(dialog)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_executable(dialog ${dialogExample_SRC})
target_link_libraries(dialog PRIVATE finalcut)
target_include_directories(dialog PRIVATE src)

#
## event-log example project
#
project(event-log)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_executable(event-log ${eventLogExample_SRC})
target_link_libraries(event-log PRIVATE finalcut)
target_include_directories(event-log PRIVATE src)

#
## fullwidth-character project
#
project(fullwidth-character)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_executable(fullwidth-character ${fullwidthCharacterExample_SRC})
target_link_libraries(fullwidth-character PRIVATE finalcut)
target_include_directories(fullwidth-character PRIVATE src)

#
## hello example project
#
project(hello)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_executable(hello ${helloExample_SRC})
target_link_libraries(hello PRIVATE finalcut)
target_include_directories(hello PRIVATE src)

#
## highlight-text example project
#
project(highlight-text)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_executable(highlight-text ${highlightTextExample_SRC})
target_link_libraries(highlight-text PRIVATE finalcut)
target_include_directories(highlight-text PRIVATE src)

#
## input-dialog example project
#
project(input-dialog)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_executable(input-dialog ${inputDialogExample_SRC})
target_link_libraries(input-dialog PRIVATE finalcut)
target_include_directories(input-dialog PRIVATE src)

#
## keyboard example project
#
project(keyboard)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_executable(keyboard ${keyboardExample_SRC})
target_link_libraries(keyboard PRIVATE finalcut)
target_include_directories(keyboard PRIVATE src)

#
## listbox example project
#
project(listbox)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_executable(listbox ${listboxExample_SRC})
target_link_libraries(listbox PRIVATE finalcut)
target_include_directories(listbox PRIVATE src)

#
## listview example project
#
project(listview)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_executable(listview ${listviewExample_SRC})
target_link_libraries(listview PRIVATE finalcut)
target_include_directories(listview PRIVATE src)

#
## mandelbrot example project
#
project(mandelbrot)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_executable(mandelbrot ${mandelbrotExample_SRC})
target_link_libraries(mandelbrot PRIVATE finalcut)
target_include_directories(mandelbrot PRIVATE src)

#
## menu example project
#
project(menu)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_executable(menu ${menuExample_SRC})
target_link_libraries(menu PRIVATE finalcut)
target_include_directories(menu PRIVATE src)

#
## mouse example project
#
project(mouse)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_executable(mouse ${mouseExample_SRC})
target_link_libraries(mouse PRIVATE finalcut)
target_include_directories(mouse PRIVATE src)

#
## opti-move example project
#
project(opti-move)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_executable(opti-move ${optiMoveExample_SRC})
target_link_libraries(opti-move PRIVATE finalcut)
target_include_directories(opti-move PRIVATE src)

#
## parallax-scrolling example project
#
project(parallax-scrolling)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_executable(parallax-scrolling ${parallaxScrollingExample_SRC})
target_link_libraries(parallax-scrolling PRIVATE finalcut)
target_include_directories(parallax-scrolling PRIVATE src)

#
## rotozoomer example project
#
project(rotozoomer)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_executable(rotozoomer ${rotozoomerExample_SRC})
target_link_libraries(rotozoomer PRIVATE finalcut)
target_include_directories(rotozoomer PRIVATE src)

#
## scrollview example project
#
project(scrollview)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_executable(scrollview ${scrollviewExample_SRC})
target_link_libraries(scrollview PRIVATE finalcut)
target_include_directories(scrollview PRIVATE src)

#
## string-operations example project
#
project("string-operations")
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_executable("string-operations" ${stringOperationsExample_SRC})
target_link_libraries("string-operations" PRIVATE finalcut)
target_include_directories("string-operations" PRIVATE src)

#
## term-attributes example project
#
project(term-attributes)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_executable(term-attributes ${termAttributesExample_SRC})
target_link_libraries(term-attributes PRIVATE finalcut)
target_include_directories(term-attributes PRIVATE src)

#
## termcap example project
#
project(termcap)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_executable(termcap ${termcapExample_SRC})
target_link_libraries(termcap PRIVATE finalcut)
target_include_directories(termcap PRIVATE src)

#
## terminal-focus example project
#
project(terminal-focus)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_executable(terminal-focus ${terminalFocusExample_SRC})
target_link_libraries(terminal-focus PRIVATE finalcut)
target_include_directories(terminal-focus PRIVATE src)

#
## timer example project
#
project(timer)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_executable(timer ${timerExample_SRC})
target_link_libraries(timer PRIVATE finalcut)
target_include_directories(timer PRIVATE src)

#
## transparent example project
#
project(transparent)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_executable(transparent ${transparentExample_SRC})
target_link_libraries(transparent PRIVATE finalcut)
target_include_directories(transparent PRIVATE src)

#
## treeview example project
#
project(treeview)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_executable(treeview ${treeviewExample_SRC})
target_link_libraries(treeview PRIVATE finalcut)
target_include_directories(treeview PRIVATE src)

#
## ui example project
#
project(ui)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_executable(ui ${uiExample_SRC})
target_link_libraries(ui PRIVATE finalcut)
target_include_directories(ui PRIVATE src)

#
## watch example project
#
project(watch)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_executable(watch ${watchExample_SRC})
target_link_libraries(watch PRIVATE finalcut)
target_include_directories(watch PRIVATE src)

#
## windows example project
#
project(windows)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_executable(windows ${windowsExample_SRC})
target_link_libraries(windows PRIVATE finalcut)
target_include_directories(windows PRIVATE src)

#
## xpmview example project
#
project(xpmview)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_executable(xpmview ${xpmviewExample_SRC})
target_link_libraries(xpmview PRIVATE finalcut)
target_include_directories(xpmview PRIVATE src)
endif (F_COMPILE_EXAMPLES)
