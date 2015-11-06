# generators
if (BUILD_DEB_PACKAGE)
    set(CPACK_GENERATOR ${CPACK_GENERATOR} "DEB")
endif ()
if (BUILD_RPM_PACKAGE)
    set(CPACK_GENERATOR ${CPACK_GENERATOR} "RPM")
endif ()

# build properties
set(CPACK_PACKAGING_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# general metadata
set(CPACK_PACKAGE_NAME "quadro")
set(CPACK_PACKAGE_CONTACT "${PROJECT_AUTHOR}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_RELEASE "1")
# doesn't match to official arch :/
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")

# deb specific
set(CPACK_DEBIAN_PACKAGE_DEPENDS "")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "")
set(CPACK_DEBIAN_PACKAGE_ENHANCES "")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "")
set(CPACK_DEBIAN_PACKAGE_SECTION "")

# rpm specific
set(CPACK_RPM_PACKAGE_DESCRIPTION "")
set(CPACK_RPM_PACKAGE_GROUP "User Interface/Desktops")
set(CPACK_RPM_PACKAGE_LICENSE "${PROJECT_LICENSE}")
set(CPACK_RPM_PACKAGE_REQUIRES "")
set(CPACK_RPM_PACKAGE_SUGGESTS "")
set(CPACK_RPM_PACKAGE_URL "")

if (BUILD_DEB_PACKAGE OR BUILD_RPM_PACKAGE)
    include(CPack)
endif ()