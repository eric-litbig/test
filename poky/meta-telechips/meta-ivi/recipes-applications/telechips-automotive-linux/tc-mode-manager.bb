DESCRIPTION = "Mode Manager Applications for Telechips Automoitve Linux SDK"
SECTION = "applications"
LICENSE = "CLOSED"

SRC_URI = "${TELECHIPS_AUTOMOTIVE_APP_GIT}/tc-mode-manager.git;protocol=${ALS_GIT_PROTOCOL};branch=${ALS_BRANCH} \
	  	   file://tc-mode-manager.service \
"
SRCREV = "96e9a79a46c4c2d3a0821a264d160317bedc7f85"

inherit autotools pkgconfig systemd

DEPENDS += "dbus glib-2.0 libtcutils libxml2"

PATCHTOOL = "git"

S = "${WORKDIR}/git"

# for systemd
SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE_${PN} = "tc-mode-manager.service"

do_install_append() {
	install -d ${D}/${systemd_unitdir}/system
	install -m 644 ${WORKDIR}/tc-mode-manager.service ${D}/${systemd_unitdir}/system
}

FILES_${PN} += " \
		${datadir} \
		${@bb.utils.contains('DISTRO_FEATURES', 'systemd', '${systemd_unitdir}', '', d)} \
"
