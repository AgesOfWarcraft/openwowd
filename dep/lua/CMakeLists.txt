if (NOT LUA_FOUND)
    message(FATAL_ERROR "Lua wasn't found on your system but it's required to build the servers!")
endif()

set(CMAKE_REQUIRED_INCLUDES ${LUA_INCLUDE_DIR})

add_library(lua STATIC IMPORTED GLOBAL)

set_target_properties(lua
        PROPERTIES
        IMPORTED_LOCATION
        "${LUA_LIBRARIES}"
        INTERFACE_INCLUDE_DIRECTORIES
        "${LUA_INCLUDE_DIR}")
