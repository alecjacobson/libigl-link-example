cmake_minimum_required(VERSION 3.1)
project(example)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# libigl
option(LIBIGL_USE_PREBUILT_LIBRARIES "Use prebuilt libraries"       ON)
option(LIBIGL_USE_STATIC_LIBRARY     "Use libigl as static library" ${LIBIGL_USE_PREBUILT_LIBRARIES})
option(LIBIGL_OPENGL                 "Use OpenGL"                   ON)
option(LIBIGL_GLFW                   "Use GLFW"                     ON)
option(LIBIGL_COPYLEFT_CGAL          "Use CGAL"                     ON)
option(LIBIGL_EMBREE                 "Use Embree"                   ON)
option(LIBIGL_IMGUI                  "Use ImGui"                    ON)
option(LIBIGL_PNG                    "Use PNG"                      ON)
option(LIBIGL_COPYLEFT_TETGEN        "Use Tetgen"                   ON)
option(LIBIGL_RESTRICTED_TRIANGLE    "Use Triangle"                 ON)
option(LIBIGL_XML                    "Use XML"                      ON)
option(LIBIGL_RESTRICTED_MATLAB      "Use Matlab"                   OFF)
option(LIBIGL_RESTRICTED_MOSEK       "Use MOSEK"                    OFF)
option(LIBIGL_RESTRICTED_PREDICATES  "Use exact predicates"         OFF)
option(LIBIGL_COMISO                 "Use CoMiso"                   OFF)
find_package(LIBIGL REQUIRED QUIET)

# Define executable here
file(GLOB SRCFILES *.cpp)
add_executable(${PROJECT_NAME} ${SRCFILES})

# fix mosek
IF(APPLE)
  INCLUDE(cmake/OSXFixDylibReferences.cmake)
  OSX_FIX_DYLIB_REFERENCES(${PROJECT_NAME} "${MOSEK_LIBRARIES}")
ENDIF()

# For each libigl module, link to target (and flip definition flag)
target_link_libraries(${PROJECT_NAME} igl::core)
list(APPEND modules "copyleft_cgal" "comiso" "embree" "restricted_matlab" "restricted_mosek" "opengl" "glfw" "imgui" "png
foreach(module_name IN LISTS modules)
  # mangle new module names
  set(orig_module_name ${module_name})
  string(REGEX MATCH ".*_" suffix ${module_name})
  if(suffix)
    string(REGEX REPLACE "_" "" suffix ${suffix})
    set(suffix "_${suffix}")
    string(REGEX REPLACE ".*_" "" module_name ${module_name})
  endif()
  string(TOUPPER "LIBIGL_${orig_module_name}" option_string)
  set(option_variable ${${option_string}})
  if(option_variable)
    target_compile_definitions(${PROJECT_NAME} PRIVATE -D${option_string})
    target_link_libraries(${PROJECT_NAME} "igl${suffix}::${module_name}")
  endif()
endforeach()
