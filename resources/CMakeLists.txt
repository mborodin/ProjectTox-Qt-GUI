set(QRC_FILE resources.qrc)

qt5_add_resources(RESOURCES ${QRC_FILE})
set(RESOURCES ${RESOURCES} PARENT_SCOPE)