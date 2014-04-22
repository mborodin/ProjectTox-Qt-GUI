project(QTox)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(QTox_SRCS QTox.cpp main.cpp)
qt4_automoc(${QTox_SRCS})
add_executable(QTox ${QTox_SRCS})
target_link_libraries(QTox ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
install(TARGETS QTox RUNTIME DESTINATION bin)
