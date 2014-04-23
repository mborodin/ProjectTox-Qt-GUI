find_package(KF5WebKit REQUIRED)



add_executable(KTox ${QTox_SRCS})
target_link_libraries(KTox toxcore sodium)
qt5_use_modules(KTox Core Gui Widgets Xml)
install(TARGETS KTox RUNTIME DESTINATION bin)