cmake_minimum_required(VERSION 3.6)
project(abb_librws)

set(${PROJECT_NAME}_MAJOR_VERSION 1)
set(${PROJECT_NAME}_MINOR_VERSION 0)
set(${PROJECT_NAME}_PATCH_VERSION 0)
set(${PROJECT_NAME}_VERSION ${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_PATCH_VERSION})

include(GNUInstallDirs)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

########################
## POCO C++ Libraries ##
########################
# We need at least 1.4.3 because of WebSocket support.
find_package(Poco 1.4.3 REQUIRED COMPONENTS Net Util Foundation XML)

###########
## Build ##
###########
set(
  SRC_FILES
    src/rws_client.cpp
    src/rws_common.cpp
    src/rws_interface.cpp
    src/rws_poco_client.cpp
    src/rws_rapid.cpp
    src/rws_state_machine_interface.cpp
)

add_library(${PROJECT_NAME} SHARED ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  PUBLIC ${Poco_INCLUDE_DIRS}
)
target_link_libraries(${PROJECT_NAME}
 ${Poco_LIBRARIES}
)

# Causes the visibility macros to use dllexport rather than dllimport,
# which is appropriate when building the dll but not consuming it.
target_compile_definitions(${PROJECT_NAME} PRIVATE "ABB_LIBRWS_BUILDING_LIBRARY")

#############
## Install ##
#############
install(
  DIRECTORY include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
  TARGETS ${PROJECT_NAME}
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Export targets.
set(export_targets ${export_targets};${PROJECT_NAME})
export(
  EXPORT export_${PROJECT_NAME}
  FILE "${PROJECT_BINARY_DIR}/export_${PROJECT_NAME}.cmake"
)

# Create the ${PROJECT_NAME}Config.cmake.
set(CONF_INCLUDE_DIRS "${CMAKE_INSTALL_PREFIX}/include")
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
  "${PROJECT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake" @ONLY
)

# Create the ${PROJECT_NAME}ConfigVersion.cmake.
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}ConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake" @ONLY
)

install(
  FILES
    "${PROJECT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  DESTINATION "share/${PROJECT_NAME}/cmake"
  COMPONENT dev
)

install(
  EXPORT export_${PROJECT_NAME}
  DESTINATION "share/${PROJECT_NAME}/cmake"
  FILE export_${PROJECT_NAME}.cmake
  COMPONENT dev
)
