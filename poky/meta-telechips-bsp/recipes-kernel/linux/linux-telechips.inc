inherit kernel
inherit kernel-yocto

LICENSE = "GPLv2"

DEPENDS += "tc-make-image-native"

S = "${WORKDIR}/git"

KBRANCH = "${ALS_BRANCH}"

KERNEL_CC_append = " --sysroot=${WORKDIR}/recipe-sysroot "

KERNEL_IMAGETYPE = "${@bb.utils.contains("TUNE_FEATURES", "aarch64", "Image", "zImage", d)}"
KERNEL_IMAGETYPE_UNCOMPRESSED = "Image"

KERNEL_IMAGE_BASE_NAME_UNCOMPRESSED = "${KERNEL_IMAGETYPE_UNCOMPRESSED}-${PKGE}-${PKGV}-${PKGR}-${MACHINE}-${DATETIME}"
KERNEL_IMAGE_BASE_NAME_UNCOMPRESSED[vardepsexclude] = "DATETIME"
KERNEL_IMAGE_SYMLINK_NAME_UNCOMPRESSED ?= "${KERNEL_IMAGETYPE_UNCOMPRESSED}-${MACHINE}"

SRCREV_machine = "${SRCREV}"

PV = "${LINUX_VERSION}"

RAMDISK_NAME="ramdisk_dummy.rom"

BOOT_IMAGE_SUFFIX ?= "img"

BOOT_IMAGE = "${MACHINE}-tc-boot-${PV}-${PR}.${BOOT_IMAGE_SUFFIX}"
BOOT_IMAGE_BINARY = "tc-boot.${BOOT_IMAGE_SUFFIX}"
BOOT_IMAGE_SYMLINK = "tc-boot-${MACHINE}.${BOOT_IMAGE_SUFFIX}"

BOOT_IMAGE_UNCOMPRESSED = "${MACHINE}-tc-boot_uncompressed-${PV}-${PR}.${BOOT_IMAGE_SUFFIX}"
BOOT_IMAGE_BINARY_UNCOMPRESSED = "tc-boot_uncompressed.${BOOT_IMAGE_SUFFIX}"
BOOT_IMAGE_SYMLINK_UNCOMPRESSED = "tc-boot-${MACHINE}_uncompressed.${BOOT_IMAGE_SUFFIX}"

INSANE_SKIP_${PN} += "installed-vs-shipped"
PATCHTOOL = "git"

do_tc_make_image() {
	cd ${D}/${KERNEL_IMAGEDEST}

	touch ${RAMDISK_NAME}
	#compressed boot image
	${STAGING_BINDIR_NATIVE}/tc-make-bootimg --kernel ${KERNEL_IMAGETYPE}-${KERNEL_VERSION} --ramdisk ${RAMDISK_NAME} --base ${KERNEL_BASE_ADDR} --kernel_offset ${KERNEL_OFFSET} --output ${BOOT_IMAGE} --cmdline ${CMDLINE}
	#uncompressed boot image
	${STAGING_BINDIR_NATIVE}/tc-make-bootimg --kernel ${KERNEL_IMAGETYPE_UNCOMPRESSED}-${KERNEL_VERSION} --ramdisk ${RAMDISK_NAME} --base ${KERNEL_BASE_ADDR} --kernel_offset ${KERNEL_OFFSET} --output ${BOOT_IMAGE_UNCOMPRESSED} --cmdline ${CMDLINE}
	rm  ${RAMDISK_NAME}

	cd -
}

do_change_defconfig() {
	echo "CONFIG_STAGING=y"								>> ${WORKDIR}/defconfig
	echo "CONFIG_LINUX_ANDROID=y"						>> ${WORKDIR}/defconfig

	if ${@bb.utils.contains('INVITE_PLATFORM', 'dispman', 'true', 'false', d)}; then
		echo "CONFIG_TCC_DISPMG=y" >> ${WORKDIR}/defconfig
	fi

	if ${@bb.utils.contains('DISTRO_FEATURES', 'multimedia', 'true', 'false', d)}; then
		echo "CONFIG_TCC_VPU_DRV=m" 					>> ${WORKDIR}/defconfig
	fi

    if ${@bb.utils.contains('INVITE_PLATFORM', 'use-vout-vsync', 'true', 'false', d)}; then
		echo "CONFIG_VOUT_USE_VSYNC_INT=y"				>> ${WORKDIR}/defconfig
		echo "CONFIG_VOUT_DISPLAY_LASTFRAME=y"			>> ${WORKDIR}/defconfig
    fi

	if ${@bb.utils.contains('DISTRO_FEATURES', 'nfs', 'true', 'false', d)}; then
		echo "CONFIG_NFSD=y" 							>> ${WORKDIR}/defconfig
		echo "CONFIG_NFSD_V3_ACL=y" 					>> ${WORKDIR}/defconfig
		echo "CONFIG_NFSD_V4=y" 						>> ${WORKDIR}/defconfig
	fi

	if ${@bb.utils.contains('DISTRO_FEATURES', 'systemd', 'true', 'false', d)}; then
		echo "CONFIG_FHANDLE=y" 						>> ${WORKDIR}/defconfig
	fi

# network configuration for ssh server
	if ${@bb.utils.contains('INVITE_PLATFORM', 'network', 'true', 'false', d)}; then
		echo "CONFIG_INET=y"						>> ${WORKDIR}/defconfig
		echo "CONFIG_IPV6=y"						>> ${WORKDIR}/defconfig
		echo "CONFIG_NETDEVICES=y"					>> ${WORKDIR}/defconfig
		echo "CONFIG_ETHERNET=y"					>> ${WORKDIR}/defconfig
		echo "CONFIG_PHYLIB=y"						>> ${WORKDIR}/defconfig
		echo "CONFIG_TCC_REALTEK_PHY=y"				>> ${WORKDIR}/defconfig
		echo "CONFIG_STMMAC_ETH=y"					>> ${WORKDIR}/defconfig

		if ${@bb.utils.contains('USE_USB_TO_ETHERNET', '1', 'true', 'false', d)}; then
			echo "CONFIG_USB_USBNET=y"				>>  ${WORKDIR}/defconfig
		fi

		if ${@bb.utils.contains('USE_RNDIS_HOST', '1', 'true', 'false', d)}; then
			echo "CONFIG_USB_NET_RNDIS_HOST=y"		>> ${WORKDIR}/defconfig
		fi

		if ${@bb.utils.contains('USE_IP_NETFILTER', '1', 'true', 'false', d)}; then
			echo "CONFIG_NETFILTER=y"					>>  ${WORKDIR}/defconfig
			echo "CONFIG_NF_CONNTRACK=y"				>>  ${WORKDIR}/defconfig
			echo "CONFIG_NF_TABLES=y"					>>  ${WORKDIR}/defconfig
			echo "CONFIG_NF_TABLES_NETDEV=y"			>>  ${WORKDIR}/defconfig
			echo "CONFIG_NFT_CT=y"						>>  ${WORKDIR}/defconfig
			echo "CONFIG_NFT_MASQ=y"					>>  ${WORKDIR}/defconfig
			echo "CONFIG_NFT_NAT=y"						>>  ${WORKDIR}/defconfig
			echo "CONFIG_NF_CONNTRACK_IPV4=y"			>>  ${WORKDIR}/defconfig
			echo "CONFIG_IP_NF_IPTABLES=y"				>>  ${WORKDIR}/defconfig
			echo "CONFIG_IP_NF_FILTER=y"				>>  ${WORKDIR}/defconfig
			echo "CONFIG_IP_NF_NAT=y"					>>  ${WORKDIR}/defconfig
			echo "CONFIG_IP_NF_TARGET_MASQUERADE=y"		>>  ${WORKDIR}/defconfig
		fi
	fi

	if ${@bb.utils.contains_any('INVITE_PLATFORM', 'with-subcore', 'true', 'false', d)}; then
		echo "CONFIG_MAILBOX=y"							>> ${WORKDIR}/defconfig
		echo "CONFIG_TCC_MULTI_MAILBOX=y"				>> ${WORKDIR}/defconfig
		echo "CONFIG_PROC_MBOX=y"           			>> ${WORKDIR}/defconfig
	fi

	if ${@bb.utils.contains('INVITE_PLATFORM', 'gpu-vz', 'true', 'false', d)}; then
		echo "CONFIG_POWERVR_VZ=y"						>> ${WORKDIR}/defconfig
	fi

# GPU/Graphic Configuration
    if ${@bb.utils.contains('INVITE_PLATFORM', 'drm', 'true', 'false', d)}; then
		echo "CONFIG_FB=n"							>> ${WORKDIR}/defconfig

		echo "CONFIG_DRM_TCC=y"             	    >> ${WORKDIR}/defconfig
		echo "CONFIG_DRM_TCC_LCD=y"         	    >> ${WORKDIR}/defconfig
		echo "CONFIG_DRM_TCC_VIC_MAX=1026"			>> ${WORKDIR}/defconfig
		echo "CONFIG_DRM_TCC_LCD_VIC=1024"			>> ${WORKDIR}/defconfig
		echo "CONFIG_DRM_TCC_KEEP_LOGO=y"				>> ${WORKDIR}/defconfig

    	if ${@bb.utils.contains_any('INVITE_PLATFORM', 'hud-display', 'true', 'false', d)}; then
			echo "CONFIG_DRM_TCC_EXT=y"				>> ${WORKDIR}/defconfig
			echo "CONFIG_DRM_TCC_EXT_VIC=1024"		>> ${WORKDIR}/defconfig
		fi

		echo "CONFIG_DRM_TCC_DPI=y"             	>> ${WORKDIR}/defconfig

		if ${@bb.utils.contains_any('TCC_MACHINE_FAMILY', 'tcc805x-main tcc805x-cluster', 'true', 'false', d)}; then
			echo "CONFIG_DRM_PANEL_LVDS_TCC=y"			>> ${WORKDIR}/defconfig
			echo "CONFIG_DRM_PANEL_DPV14_TCC=y"			>> ${WORKDIR}/defconfig
			echo "CONFIG_DRM_PANEL_MAX968XX=y"			>> ${WORKDIR}/defconfig
		fi
	else
		echo "CONFIG_DRM_TCC=n"						>> ${WORKDIR}/defconfig

		echo "CONFIG_FB=y"							>> ${WORKDIR}/defconfig
		echo "CONFIG_FB_NEW=y"						>> ${WORKDIR}/defconfig
		echo "CONFIG_FB_PANEL_LVDS_TCC=y"			>> ${WORKDIR}/defconfig

		if ${@bb.utils.contains_any('TCC_MACHINE_FAMILY', 'tcc805x-main tcc805x-cluster', 'true', 'false', d)}; then
			echo "CONFIG_POWERVR_ROGUE=y"				>> ${WORKDIR}/defconfig
			echo "CONFIG_POWERVR_DC_FBDEV=y"			>> ${WORKDIR}/defconfig
		fi
	fi

# for disable 4k
	if ${@bb.utils.contains('INVITE_PLATFORM', 'support-4k-video', 'false', 'true', d)}; then
		echo "# CONFIG_SUPPORT_TCC_HEVC_4K is not set"				>> ${WORKDIR}/defconfig
		echo "# CONFIG_SUPPORT_TCC_VP9_4K is not set"				>> ${WORKDIR}/defconfig
	fi

# for TOST board
	echo "CONFIG_I2C_TCC_V3=y"					>> ${WORKDIR}/defconfig
	echo "CONFIG_REGULATOR_DA9121=y"				>> ${WORKDIR}/defconfig
	echo "CONFIG_REGULATOR_DA9210=y"				>> ${WORKDIR}/defconfig
	echo "CONFIG_PM_TCC805X_DA9131_SW_WORKAROUND=y"				>> ${WORKDIR}/defconfig
	echo "CONFIG_REGULATOR_DA9062=y"				>> ${WORKDIR}/defconfig
	echo "# CONFIG_TOUCHSCREEN_INIT_SERDES is not set"		>> ${WORKDIR}/defconfig
}

do_change_defconfig_append_tcc803x() {
	if ${@bb.utils.contains('DISTRO_FEATURES', 'opengl', 'true', 'false', d)}; then
		echo "CONFIG_MALI_MIDGARD=y"			>> ${WORKDIR}/defconfig
	fi
}

addtask change_defconfig before do_configure after do_kernel_metadata
addtask tc_make_image before do_deploy after do_install

do_configure_prepend() {
	cp ${WORKDIR}/defconfig ${B}/.config
}

kernel_do_install_append() {
	if ${@bb.utils.contains("TUNE_FEATURES", "aarch64", "false", "true", d)}; then
		install -m 0644 ${KERNEL_OUTPUT_DIR}/${KERNEL_IMAGETYPE_UNCOMPRESSED} ${D}/${KERNEL_IMAGEDEST}/${KERNEL_IMAGETYPE_UNCOMPRESSED}-${KERNEL_VERSION}
	fi
	rm -f $kerneldir/mkbootimg
}

do_deploy_append() {
	install -m 0644 ${D}/${KERNEL_IMAGEDEST}/${KERNEL_IMAGETYPE_UNCOMPRESSED}-${KERNEL_VERSION} ${DEPLOYDIR}/${KERNEL_IMAGE_BASE_NAME_UNCOMPRESSED}.bin
	install -m 0644 ${D}/${KERNEL_IMAGEDEST}/${BOOT_IMAGE} ${DEPLOYDIR}/
	install -m 0644 ${D}/${KERNEL_IMAGEDEST}/${BOOT_IMAGE_UNCOMPRESSED} ${DEPLOYDIR}/

	cd ${DEPLOYDIR}

	ln -sf ${KERNEL_IMAGE_BASE_NAME_UNCOMPRESSED}.bin ${DEPLOYDIR}/${KERNEL_IMAGE_SYMLINK_NAME_UNCOMPRESSED}.bin
	ln -sf ${KERNEL_IMAGE_BASE_NAME_UNCOMPRESSED}.bin ${DEPLOYDIR}/${KERNEL_IMAGETYPE_UNCOMPRESSED}

    rm -f ${BOOT_IMAGE_BINARY} ${BOOT_IMAGE_SYMLINK}
    ln -sf ${BOOT_IMAGE} ${BOOT_IMAGE_SYMLINK}
    ln -sf ${BOOT_IMAGE} ${BOOT_IMAGE_BINARY}

	rm -f ${BOOT_IMAGE_BINARY_UNCOMPRESSED} ${BOOT_IMAGE_SYMLINK_UNCOMPRESSED}
	ln -sf ${BOOT_IMAGE_UNCOMPRESSED} ${BOOT_IMAGE_SYMLINK_UNCOMPRESSED}
	ln -sf ${BOOT_IMAGE_UNCOMPRESSED} ${BOOT_IMAGE_BINARY_UNCOMPRESSED}

	cd -
}

PACKAGE_KERNEL_BOOT_IMAGE = " \
	/${KERNEL_IMAGEDEST}/${BOOT_IMAGE} \
	/${KERNEL_IMAGEDEST}/${BOOT_IMAGE_BINARY} \
"
PACKAGE_KERNEL_BOOT_IMAGE_UNCOMPRESSED = " \
	/${KERNEL_IMAGEDEST}/${BOOT_IMAGE_UNCOMPRESSED} \
	/${KERNEL_IMAGEDEST}/${BOOT_IMAGE_BINARY_UNCOMPRESSED} \
"
FILES_${KERNEL_PACKAGE_NAME}-image += " \
	/${KERNEL_IMAGEDEST}/${KERNEL_IMAGETYPE_UNCOMPRESSED}* \
	${PACKAGE_KERNEL_BOOT_IMAGE} \
	${PACKAGE_KERNEL_BOOT_IMAGE_UNCOMPRESSED} \
"

FILES_${KERNEL_PACKAGE_NAME}-modules += " \
	/lib/modules/${KERNEL_VERSION}/modules.builtin \
	/lib/modules/${KERNEL_VERSION}/modules.order \
"

RDEPENDS_kernel-modules += "${@bb.utils.contains('DISTRO_FEATURES', 'multimedia', 'kernel-modules-vpu', '', d)}"

# disable unneeded tasks
do_uboot_mkimage[noexec] = "1"

pkg_postinst_kernel-image_append () {
	update-alternatives --install /${KERNEL_IMAGEDEST}/${KERNEL_IMAGETYPE_UNCOMPRESSED} ${KERNEL_IMAGETYPE_UNCOMPRESSED} /${KERNEL_IMAGEDEST}/${KERNEL_IMAGETYPE_UNCOMPRESSED}-${KERNEL_VERSION} ${KERNEL_PRIORITY} || true
}
