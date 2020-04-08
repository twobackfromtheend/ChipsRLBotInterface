cmake_minimum_required(VERSION 3.8)
project(RLUtilities LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(TARGET_LANGUAGE "cpp" CACHE STRING "Build shared library for \"cpp\", \"python\" or \"both\"")
set_property(CACHE TARGET_LANGUAGE PROPERTY STRINGS "cpp" "python" "both")
message(STATUS "Building libraries for ${TARGET_LANGUAGE}")

add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(rlu_sources

  # simulation
  ${PROJECT_SOURCE_DIR}/src/simulation/ball.cc
  ${PROJECT_SOURCE_DIR}/src/simulation/geometry.cc
  ${PROJECT_SOURCE_DIR}/src/simulation/mesh.cc
  ${PROJECT_SOURCE_DIR}/src/simulation/field.cc
  ${PROJECT_SOURCE_DIR}/src/simulation/bvh.cc

  # resource files for meshes
  ${PROJECT_SOURCE_DIR}/assets/assets.rc

)

if (TARGET_LANGUAGE STREQUAL "cpp" OR TARGET_LANGUAGE STREQUAL "both")

  add_library(rlutilities_cpp SHARED
    ${rlu_sources}
  )
  target_include_directories(rlutilities_cpp PUBLIC inc)
  target_compile_features(rlutilities_cpp PUBLIC cxx_std_17)

  # ensures a consistent name when used with gcc, clang, msvc
  set_target_properties(rlutilities_cpp PROPERTIES PREFIX "")
  set_target_properties(rlutilities_cpp PROPERTIES OUTPUT_NAME "rlutilities")
  set_target_properties(rlutilities_cpp PROPERTIES
      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )

endif()


if (TARGET_LANGUAGE STREQUAL "python" OR TARGET_LANGUAGE STREQUAL "both")

  add_subdirectory(python/pybind11)

  add_compile_options($<$<CXX_COMPILER_ID:GNU>:-D_hypot=hypot>)

  pybind11_add_module(rlutilities
    ${PROJECT_SOURCE_DIR}/src/python_extensions.cc
    ${rlu_sources}
  )
  target_include_directories(rlutilities PUBLIC inc/)
  target_compile_definitions(rlutilities PUBLIC -DGENERATE_PYTHON_BINDINGS)
  target_compile_features(rlutilities PUBLIC cxx_std_17)

endif()
