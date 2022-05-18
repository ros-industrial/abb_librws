cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)

# Read version from the package.xml file.
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/package.xml package_xml_str)
if(NOT package_xml_str MATCHES "<version>([0-9]+.[0-9]+.[0-9]+)</version>")
  message(FATAL_ERROR "Could not parse project version from package.xml. Aborting.")
endif()

# At this point we either have a proper version string, or we've errored
# out with a FATAL_ERROR above. So assume CMAKE_MATCH_1 contains our
# package's version.
project(abb_librws VERSION ${CMAKE_MATCH_1} LANGUAGES CXX)

include(GNUInstallDirs)

list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/third_party")

if(WIN32)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

########################
## POCO C++ Libraries ##
########################
# We need at least 1.4.3 because of WebSocket support.
find_package(Poco 1.4.3 REQUIRED COMPONENTS Foundation Net Util XML NetSSL)

## Find boost exception library
find_package(Boost REQUIRED COMPONENTS exception log log_setup)

###########
## Build ##
###########
if(NOT DEFINED BUILD_SHARED_LIBS)
  option(BUILD_SHARED_LIBS "Build dynamically-linked binaries" ON)
endif()

set(
  SRC_FILES
    src/system_constants.cpp
    src/rws_poco_client.cpp
    src/rws_poco_result.cpp
    src/rws_rapid.cpp
    src/rws_resource.cpp
    src/rws_state_machine_interface.cpp
    src/rws_subscription.cpp
    src/rws_websocket.cpp
    src/rws.cpp
    src/parsing.cpp
    src/connection_options.cpp
    src/logger.cpp

    src/common/rw/rapid.cpp
    src/common/rw/panel.cpp

    src/v1_0/rws.cpp
    src/v1_0/rws_client.cpp
    src/v1_0/rws_interface.cpp
    src/v1_0/rw/rapid.cpp
    src/v1_0/rw/panel.cpp
    src/v1_0/rw/io.cpp
    src/v1_0/rw/cfg.cpp
    src/v1_0/rw/ctrl.cpp

    src/v2_0/rws.cpp
    src/v2_0/rws_client.cpp
    src/v2_0/rws_interface.cpp
    src/v2_0/rw/rapid.cpp
    src/v2_0/rw/panel.cpp
    src/v2_0/rw/cfg.cpp
    src/v2_0/rw/ctrl.cpp
)

add_library(${PROJECT_NAME} ${SRC_FILES})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME}
  EXPORT_FILE_NAME ${PROJECT_NAME}/${PROJECT_NAME}_export.h
)

target_include_directories(${PROJECT_NAME} PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include;${CMAKE_CURRENT_BINARY_DIR}>"
  $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>
  ${Poco_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC ${Poco_LIBRARIES}

  PRIVATE ${Boost_LOG_LIBRARY}
  PRIVATE ${Boost_LOG_SETUP_LIBRARY}
)

target_compile_definitions(${PROJECT_NAME} PUBLIC "BOOST_LOG_DYN_LINK")

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(${PROJECT_NAME} PUBLIC "ABB_LIBRWS_STATIC_DEFINE")
endif()

##########
## Test ##
##########

option(MYPROJECT_BUILD_TESTING "Build the testing tree when building as a dependency" OFF)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MYPROJECT_BUILD_TESTING)

  enable_testing()

  find_package(GTest REQUIRED)
  find_package(Boost REQUIRED COMPONENTS exception log log_setup)

  add_executable(${PROJECT_NAME}-test
      test/rws_rapid_test.cpp
  )

  target_link_libraries(${PROJECT_NAME}-test
      ${PROJECT_NAME}
      GTest::GTest
      GTest::Main

      ${Boost_LOG_LIBRARY}
      ${Boost_LOG_SETUP_LIBRARY}
  )

  gtest_discover_tests(${PROJECT_NAME}-test)
endif()


#############
## Install ##
#############
install(
  DIRECTORY include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}_export.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)

install(
  DIRECTORY cmake/modules/third_party/
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
)

install(
  TARGETS ${PROJECT_NAME}
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

include(CMakePackageConfigHelpers)

# Create the ${PROJECT_NAME}Config.cmake.
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
  "${PROJECT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake" @ONLY
)

# Create the ${PROJECT_NAME}ConfigVersion.cmake.
write_basic_package_version_file(
  ${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  COMPATIBILITY AnyNewerVersion
)

install(
  FILES
    "${PROJECT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    "${CMAKE_CURRENT_SOURCE_DIR}/package.xml"
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}
)

# Export targets.
set(export_targets ${export_targets};${PROJECT_NAME})
export(
  EXPORT export_${PROJECT_NAME}
  FILE "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake"
  NAMESPACE ${PROJECT_NAME}::
)

install(
  EXPORT export_${PROJECT_NAME}
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
)
