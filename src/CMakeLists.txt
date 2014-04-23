set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Settings ${TOXCORE_DIR})

#aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} QTox_SRCS)
#aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Settings QTox_SRCS_SETTINGS)

set(QTox_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/aboutdialog.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/account.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/addfrienddialog.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/chatpagewidget.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/closeapplicationdialog.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/copyableelidelabel.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/core.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/createnewaccountwizardpage.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/customhinttextedit.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/customhinttreeview.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/customhintwidget.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/editablelabelwidget.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/elidelabel.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/emoticonmenu.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/esclineedit.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/filterwidget.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/frienditemdelegate.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/frienditemwidget.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/friendproxymodel.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/friendrequestdialog.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/friendswidget.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/inputtextwidget.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/loginwindow.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/messagedisplaywidget.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/messagelabel.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/opacitywidget.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ouruseritemwidget.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/pageswidget.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Settings/abstractsettingspage.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Settings/basicsettingsdialog.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Settings/customhintlistwidget.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Settings/dhtbootstrapsettingspage.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Settings/dhtserverdialog.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Settings/emojifontcombobox.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Settings/emojifontsettingsdialog.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Settings/guisettingspage.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Settings/loggingsettingspage.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Settings/settings.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Settings/settingsdialog.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/smileypack.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/starter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/status.cpp)

set(QRC_FILE ${RESOURCES_DIR}/resources.qrc)

qt5_add_resources(RESOURCES ${QRC_FILE})
set(RESOURCES ${RESOURCES} PARENT_SCOPE)

generate_translations(TRANSLATION_FILES "${TRANSLATIONS}" "${QTox_SRCS}")

add_executable(QTox ${QTox_SRCS} ${RESOURCES} ${TRANSLATION_FILES})
add_dependencies(QTox translations)
target_link_libraries(QTox toxcore sodium)
qt5_use_modules(QTox Core Gui Widgets Xml)
install(TARGETS QTox RUNTIME DESTINATION bin)

if(KDE_INTEGRATION)
  add_subdirectory(KDE)
endif(KDE_INTEGRATION)