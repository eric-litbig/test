DESCRIPTION = "Core Library for Telechips Linux AVN"
SECTION = "libs"
LICENSE = "Telechips"
LIC_FILES_CHKSUM = "file://include/TCDBusRawAPI.h;beginline=1;endline=24;md5=e181d0d510a65f510a268f48278fd5a3"

SRC_URI = "${TELECHIPS_AUTOMOTIVE_APP_GIT}/libtc-utils.git;protocol=${ALS_GIT_PROTOCOL};branch=${ALS_BRANCH}"
SRCREV = "eb3d335213e3046d1acc6565b2863a160cffde21"

inherit autotools pkgconfig

DEPENDS += "glib-2.0 dbus"
PATCHTOOL = "git"

S = "${WORKDIR}/git"

#EXTRA_OECONF += "${@bb.utils.contains('INVITE_PLATFORM', 'genivi', '--enable-session-bus', '', d)}"
