project(QTox)
cmake_minimum_required(VERSION 2.8.11)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5LinguistTools REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(RESOURCES_DIR ${CMAKE_SOURCE_DIR}/resources)
set(TRANSLATIONS_DIR ${CMAKE_SOURCE_DIR}/translations)
set(TOX_DIR ${CMAKE_SOURCE_DIR}/submodules/ProjectTox-Core/)
set(TOXCORE_DIR ${TOX_DIR}/toxcore)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${SRC_DIR} ${SRC_DIR}/Settings ${TOXCORE_DIR})

aux_source_directory(${TOXCORE_DIR} TOXCORE_SRCS)

add_library(toxcore ${TOXCORE_SRCS})

#add_subdirectory(resources)
add_subdirectory(translations)
add_subdirectory(src)

#aux_source_directory(${SRC_DIR} QTox_SRCS)
#aux_source_directory(${SRC_DIR}/Settings QTox_SRCS_SETTINGS)

#add_executable(QTox ${QTox_SRCS} ${QTox_SRCS_SETTINGS})
#target_link_libraries(QTox toxcore sodium)
#qt5_use_modules(QTox Core Gui Widgets Xml)
#install(TARGETS QTox RUNTIME DESTINATION bin)
