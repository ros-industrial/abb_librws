cmake_minimum_required(VERSION 3.12)
find_package(pybind11 REQUIRED)

file (GLOB PYTHON_FILES "*.cpp")
pybind11_add_module(_${PROJECT_NAME} ${PYTHON_FILES})
target_link_libraries(_${PROJECT_NAME} PRIVATE
    ${PROJECT_NAME}
)

set_target_properties(_${PROJECT_NAME}
    PROPERTIES
      PREFIX ""
      OUTPUT_NAME _${PROJECT_NAME}
      LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

# Set the RPATH so the dependent libraries can be found in the wheel.
if(APPLE)
  set(_rpath_value "@loader_path/lib")
elseif(UNIX)
  set(_rpath_value "$ORIGIN/lib")
endif()

if (NOT WIN32)
  set_target_properties(_${PROJECT_NAME} PROPERTIES
                        INSTALL_RPATH ${_rpath_value})
endif()

install(TARGETS _${PROJECT_NAME}
    DESTINATION .
)

install(FILES ${PROJECT_NAME}/__init__.py
    DESTINATION .
)