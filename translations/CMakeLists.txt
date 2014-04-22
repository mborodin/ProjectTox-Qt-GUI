find_package(Qt5LinguistTools REQUIRED)

macro(generate_translations QMFILES LOCALES SRCS)
  message(STATUS ${Qt5_LUPDATE_EXECUTABLE})
  set(TSFILES )
  foreach(locale ${LOCALES})
    message(STATUS "Adding ${locale} locale")
    list(APPEND TSFILES "${TRANSLATIONS_DIR}/i18n_${locale}.ts")
  endforeach(locale)
  qt5_create_translation(${QMFILES} "${SRCS}" "${TSFILES}")
endmacro(generate_translations)

set(TRANSLATIONS en de uk pl ru cz fr PARENT_SCOPE)

set(SOURCES "${SRC_DIR} ${SRC_DIR}/Settings")