# Dmitriy Vetutnev 2018
# ODANT 2018

set(Pion_MAJOR_VERSION 5)
set(Pion_MINOR_VERSION 0)
set(Pion_PATCH_VERSION 7)
set(Pion_TWEAK_VERSION 0)

set(Pion_VERSION "${Pion_MAJOR_VERSION}.${Pion_MINOR_VERSION}.${Pion_PATCH_VERSION}.${Pion_TWEAK_VERSION}")

find_path(Pion_INCLUDE_DIR
    NAMES pion/plugin.hpp
    PATHS ${CONAN_INCLUDE_DIRS_PION}
    NO_DEFAULT_PATH
)

find_library(Pion_LIBRARY
    NAMES pion piond
    PATHS ${CONAN_LIB_DIRS_PION}
    NO_DEFAULT_PATH
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Pion
    REQUIRED_VARS Pion_LIBRARY Pion_INCLUDE_DIR
    VERSION_VAR Pion_VERSION
)

if(Pion_FOUND)
    set(Pion_INCLUDE_DIRS ${Pion_INCLUDE_DIR})
    set(Pion_LIBRARIES ${Pion_LIBRARY})
    mark_as_advanced(Pion_INCLUDE_DIR Pion_LIBRARY)
    
    if(NOT TARGET Pion::pion)
      add_library(Pion::pion UNKNOWN IMPORTED)
      set_target_properties(Pion::pion PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES ${Pion_INCLUDE_DIR}
        IMPORTED_LOCATION ${Pion_LIBRARY}
        # Add defines from package_info
        INTERFACE_COMPILE_DEFINITIONS ${CONAN_COMPILE_DEFINITIONS_PION}
      )
    endif()

    set(_enable_services)
    foreach(_item ${${CMAKE_FIND_PACKAGE_NAME}_FIND_COMPONENTS})
        if(${_item} STREQUAL services)
            set(_enable_services ON)
        endif()
    endforeach()
    if(_enable_services)
        find_library(Pion_services_LIBRARY
            NAMES pion_services pion_servicesd
            PATHS ${CONAN_LIB_DIRS_PION}
            NO_DEFAULT_PATH
        )

        set(Pion_services_FIND_REQUIRED ON)
        find_package_handle_standard_args(Pion_services
            REQUIRED_VARS Pion_services_LIBRARY
        )

        set(Pion_LIBRARIES ${Pion_LIBRARIES} ${Pion_services_LIBRARY})
        mark_as_advanced(Pion_services_LIBRARY)
        
        if(NOT TARGET Pion::services)
            add_library(Pion::services UNKNOWN IMPORTED)
            set_target_properties(Pion::services PROPERTIES
                IMPORTED_LOCATION ${Pion_services_LIBRARY}
                    INTERFACE_LINK_LIBRARIES Pion::pion
            )
        endif()
    endif()
endif()
