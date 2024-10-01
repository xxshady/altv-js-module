include(../shared/cmake/DepsHelpers.cmake)

# Set this to false, when using a custom nodejs build for testing
set(__deps_check_enabled true)
set(NODEJS_VERSION "22.8.0")

function(DownloadDeps)
    set(__base_path "${PROJECT_SOURCE_DIR}/deps/nodejs/lib")

    GetBranchAndOS(__deps_branch __deps_os_path_name)
    set(__deps_url_base_path "https://cdn.alt-mp.com/deps/nodejs/${NODEJS_VERSION}/${__deps_os_path_name}")

    if(__deps_check_enabled)
        if(WIN32)
            message("Checking release binaries...")

            GetCDNInfo("${__deps_url_base_path}/Release" __deps_release_hashes __deps_current_version)

            DownloadFile("libnode.lib" "${__base_path}/Release" "Release" ${__deps_release_hashes})
            DownloadFile("libnode.dll" "${__base_path}/Release" "Release" ${__deps_release_hashes})

            # Only download debug binary in Debug builds
            if(CMAKE_BUILD_TYPE STREQUAL "Debug")
                message("Checking debug binaries...")

                GetCDNInfo("${__deps_url_base_path}/Debug" __deps_debug_hashes __deps_current_version)

                DownloadFile("libnode.lib" "${__base_path}/Debug" "Debug" ${__deps_debug_hashes})
                DownloadFile("libnode.dll" "${__base_path}/Debug" "Debug" ${__deps_debug_hashes})
            endif()
        elseif(UNIX)
            message("Checking binaries...")

            GetCDNInfo("${__deps_url_base_path}" __deps_linux_hashes __deps_current_version)

            DownloadFile("libnode.so" "${__base_path}" "" ${__deps_linux_hashes})
        endif()

        message("NodeJS deps version: ${NODEJS_VERSION}")
    endif()
endfunction()
