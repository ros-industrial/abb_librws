set(PYTHON_VERSIONS "2" "3")
foreach(PYTHON_VERSION ${PYTHON_VERSIONS})
  unset(PYTHON_EXECUTABLE CACHE)
  unset(PYTHON_INCLUDE_DIR CACHE)
  unset(PYTHON_LIBRARY CACHE)
  unset(PYTHON_DEFAULT_EXECUTABLE CACHE)
  find_package(PythonInterp ${PYTHON_VERSION})
  find_package(PythonLibs ${PYTHON_VERSION})

  if(PYTHON_VERSION VERSION_LESS 3)
    find_package(Boost COMPONENTS python)
  else()
    find_package(Boost COMPONENTS python-py${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR})
  endif()
  if (NOT Boost_FOUND)
    message(WARNING "Boost was not found for python ${PYTHON_VERSION}")
    continue()
  endif()

  add_library(${PROJECT_NAME}_py${PYTHON_VERSION} rws_interface_py.cpp)
  target_link_libraries(${PROJECT_NAME}_py${PYTHON_VERSION}
    ${PROJECT_NAME}
    ${Boost_LIBRARIES}
    ${Poco_LIBRARIES}
    ${PYTHON_LIBRARIES}
  )
  target_include_directories(${PROJECT_NAME}_py${PYTHON_VERSION} PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include;${CMAKE_CURRENT_BINARY_DIR}>"
    $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>
    ${Boost_INCLUDE_DIRS}
    ${Poco_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_DIRS}
  )
  set_target_properties(${PROJECT_NAME}_py${PYTHON_VERSION}
    PROPERTIES
      PREFIX ""
      OUTPUT_NAME ${PROJECT_NAME}_py
      LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${PROJECT_NAME}_py${PYTHON_VERSION}/)

  install(TARGETS ${PROJECT_NAME}_py${PYTHON_VERSION}
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/dist-packages/${PROJECT_NAME}"
  )

  install(FILES __init__.py
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/dist-packages/${PROJECT_NAME}"
  )
endforeach()
