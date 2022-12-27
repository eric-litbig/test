DESCRIPTION = "Setting for Telechips Automotive Linux SDK"
SECTION = "applications"
LICENSE = "Telechips"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta-telechips/meta-core/licenses/Telechips;md5=e23a23ed6facb2366525db53060c05a4"

SRC_URI = "${TELECHIPS_AUTOMOTIVE_APP_GIT}/tc-setting.git;protocol=${ALS_GIT_PROTOCOL};branch=${ALS_BRANCH}"
SRCREV = "4f61513140876ef4196adcf41cc67a921f9468ed"

inherit qmake5 pkgconfig

DEPENDS += "qtbase qtdeclarative libtcutils libtc-color-adjustment libxml2"
DEPENDS += "${@bb.utils.contains('INVITE_PLATFORM', 'fw-update', 'tc-update-engine', '', d)}"
EXTRA_QMAKEVARS_PRE = " \
	 ${@bb.utils.contains('INVITE_PLATFORM', 'fw-update', 'FWUPDATE=1', '', d)} \
	 ${@bb.utils.contains_any('TCC_ARCH_FAMILY', 'tcc897x tcc803x', 'DISABLE_3DLUT=1', '', d)} \
"

S = "${WORKDIR}/git"
B = "${S}"
