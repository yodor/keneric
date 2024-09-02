cmake_minimum_required(VERSION 3.16)

# KDE Applications Version, managed by release script
set (RELEASE_SERVICE_VERSION_MAJOR "1")
set (RELEASE_SERVICE_VERSION_MINOR "0")
set (RELEASE_SERVICE_VERSION_MICRO "1")
set (RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

project(keneric VERSION ${RELEASE_SERVICE_VERSION})

set(QT_MIN_VERSION "6.5.0")
set(KF_MIN_VERSION "6.0.0")

find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FeatureSummary)
include(WriteBasicConfigVersionFile)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)


find_package(Qt6 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Core Gui)

find_package(KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS

    KIO

)



include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
)



kcoreaddons_add_plugin(keneric INSTALL_NAMESPACE "kf6/thumbcreator")

target_sources(keneric PRIVATE
    keneric.cpp
)

target_link_libraries(keneric
    KF6::KIOGui
    Qt::Gui
)


feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

install(
  FILES scripts/kenericProcess DESTINATION bin
          PERMISSIONS
          OWNER_READ OWNER_WRITE OWNER_EXECUTE
          GROUP_READ GROUP_EXECUTE
          WORLD_READ WORLD_EXECUTE
)

# project(keneric)
#
# cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)
# set(QT_MIN_VERSION 6.5.0)
#
# find_package(ECM 2.0.0 REQUIRED NO_MODULE)
# set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
#
# include(FeatureSummary)
# include(WriteBasicConfigVersionFile)
# include(KDEInstallDirs)
# include(KDECMakeSettings)
# include(KDECompilerSettings)
#
# find_package(Qt6 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Gui)
# find_package(KF6 REQUIRED COMPONENTS KIO)
#
# include_directories(
#     ${CMAKE_CURRENT_BINARY_DIR}
#     )
#
# set( keneric_PART_SRCS
#   keneric.cpp
# )
#
# add_library(keneric MODULE ${keneric_PART_SRCS})
#
# target_link_libraries(keneric Qt6::Gui KF6::KIOWidgets)
#
# install(TARGETS keneric DESTINATION ${PLUGIN_INSTALL_DIR})


