DESCRIPTION = "Telechips UFSD Drivers"
SECTION = "kernel/modules"
LICENSE = "Telechips"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta-telechips-bsp/licenses/Telechips;md5=e23a23ed6facb2366525db53060c05a4"

inherit module

SRC_URI = "${TELECHIPS_AUTOMOTIVE_BSP_GIT}/filesystem_ufsd.git;protocol=${ALS_GIT_PROTOCOL};branch=${ALS_BRANCH}"
SRCREV = "c93bb23272c084158e39292c019a9f2850737bfb"

PATCHTOOL = "git"
LINKER_HASH_STYLE = "sysv"

S="${WORKDIR}/git"

UFSD_INSTALL_DIR = "${D}/lib/modules/${KERNEL_VERSION}/kernel/drivers/ufsd"

do_preconfigure[noexec] = "1"
do_configure[noexec] = "1"
do_compile[noexec] = "1"

do_install() {
	install -d ${D}${sysconfdir}/modules-load.d
	install -d ${UFSD_INSTALL_DIR}

	if ${@bb.utils.contains('TCC_ARCH_FAMILY', 'tcc803x', 'true', 'false', d)}; then
		install -m 0644 ${S}/${LINUX_VERSION}/fastboot/${TUNE_ARCH}/jnl_tcc803x.ko  ${UFSD_INSTALL_DIR}/jnl.ko
		install -m 0644 ${S}/${LINUX_VERSION}/fastboot/${TUNE_ARCH}/ufsd_tcc803x.ko  ${UFSD_INSTALL_DIR}/ufsd.ko
	elif ${@oe.utils.conditional('TCC_ARCH_FAMILY', 'tcc805x', 'true', 'false', d)}; then
		install -m 0644 ${S}/${LINUX_VERSION}/fastboot/${TUNE_ARCH}/jnl_tcc805x.ko  ${UFSD_INSTALL_DIR}/jnl.ko
		install -m 0644 ${S}/${LINUX_VERSION}/fastboot/${TUNE_ARCH}/ufsd_tcc805x.ko  ${UFSD_INSTALL_DIR}/ufsd.ko
	else
		echo "Not Supported TCC ARCH FAMLIY : ${TCC_ARCH_FAMILY}"
	fi
	echo "ufsd" >> ${D}${sysconfdir}/modules-load.d/ufsd.conf
}

FILES_kernel-module-ufsd += "${sysconfdir}/modules-load.d/ufsd.conf"
