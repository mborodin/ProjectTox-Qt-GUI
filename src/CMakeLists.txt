set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Settings ${TOXCORE_DIR})

#aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} QTox_SRCS)
#aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Settings QTox_SRCS_SETTINGS)

set(QTox_SRCS aboutdialog.cpp
        account.cpp
        addfrienddialog.cpp
        chatpagewidget.cpp
        closeapplicationdialog.cpp
        copyableelidelabel.cpp
        core.cpp
        createnewaccountwizardpage.cpp
        customhinttextedit.cpp
        customhinttreeview.cpp
        customhintwidget.cpp
        editablelabelwidget.cpp
        elidelabel.cpp
        emoticonmenu.cpp
        esclineedit.cpp
        filterwidget.cpp
        frienditemdelegate.cpp
        frienditemwidget.cpp
        friendproxymodel.cpp
        friendrequestdialog.cpp
        friendswidget.cpp
        inputtextwidget.cpp
        loginwindow.cpp
        main.cpp
        mainwindow.cpp
        messagedisplaywidget.cpp
        messagelabel.cpp
        opacitywidget.cpp
        ouruseritemwidget.cpp
        pageswidget.cpp
        Settings/abstractsettingspage.cpp
        Settings/basicsettingsdialog.cpp
        Settings/customhintlistwidget.cpp
        Settings/dhtbootstrapsettingspage.cpp
        Settings/dhtserverdialog.cpp
        Settings/emojifontcombobox.cpp
        Settings/emojifontsettingsdialog.cpp
        Settings/guisettingspage.cpp
        Settings/loggingsettingspage.cpp
        Settings/settings.cpp
        Settings/settingsdialog.cpp
        smileypack.cpp
        starter.cpp
        status.cpp)

set(QRC_FILE ${RESOURCES_DIR}/resources.qrc)

qt5_add_resources(RESOURCES ${QRC_FILE})
set(RESOURCES ${RESOURCES} PARENT_SCOPE)

generate_translations(TRANSLATION_FILES "${TRANSLATIONS}" "${QTox_SRCS}")

add_executable(QTox ${QTox_SRCS} ${RESOURCES} ${TRANSLATION_FILES})
add_dependencies(QTox translations)
target_link_libraries(QTox toxcore sodium)
qt5_use_modules(QTox Core Gui Widgets Xml)
install(TARGETS QTox RUNTIME DESTINATION bin)