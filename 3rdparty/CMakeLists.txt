add_library(3rdparty_stub INTERFACE)
include(FetchContent)

# stb
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
)
FetchContent_GetProperties(stb)
if (NOT stb_POPULATED)
    FetchContent_Populate(stb)
    target_include_directories(3rdparty_stub INTERFACE ${stb_SOURCE_DIR})
endif ()

# glog
FetchContent_Declare(
    glog
    GIT_REPOSITORY https://github.com/google/glog.git
    GIT_TAG v0.4.0
)
FetchContent_GetProperties(glog)
if (NOT glog_POPULATED)
    FetchContent_Populate(glog)
    set(WITH_GFLAGS OFF CACHE BOOL "" FORCE)
    set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
    add_subdirectory(${glog_SOURCE_DIR} ${glog_BINARY_DIR})
    target_link_libraries(3rdparty_stub INTERFACE glog::glog)
endif ()