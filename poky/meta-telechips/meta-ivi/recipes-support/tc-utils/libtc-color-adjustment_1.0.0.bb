DESCRIPTION = "Color Adjustment Library for Telechips Linux AVN"
SECTION = "libs"
LICENSE = "Telechips"
LIC_FILES_CHKSUM = "file://include/tc_api_color_adjustment.h;beginline=1;endline=24;md5=2049edde3161f6e902e653f8717341e3"

SRC_URI = "${TELECHIPS_AUTOMOTIVE_APP_GIT}/libtc-color-adjustment.git;protocol=${ALS_GIT_PROTOCOL};branch=${ALS_BRANCH}"
SRCREV = "54493f9385abe5b7e9777a95515d002422b493f3"

inherit autotools pkgconfig

DEPENDS += "linux-libc-headers"
PATCHTOOL = "git"

S = "${WORKDIR}/git"

