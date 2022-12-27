DESCRIPTION = "Telechips Make Image tools"
LICENSE = "Telechips"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta-telechips/meta-core/licenses/Telechips;md5=e23a23ed6facb2366525db53060c05a4"
SECTION = "bsp"

inherit native

SRC_URI = "${TELECHIPS_AUTOMOTIVE_GIT}/tools/mktcimg.git;protocol=${ALS_GIT_PROTOCOL};branch=${ALS_BRANCH}"
SRCREV = "0210ccad2f882f5ba5b7cbf80faed59fb76b7ed1"

S = "${WORKDIR}/git"

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${S}/mktcimg		${D}${bindir}
}

do_configure[noexec] = "1"
do_compile[noexec] = "1"

BBCLASSEXTEND = "native"
