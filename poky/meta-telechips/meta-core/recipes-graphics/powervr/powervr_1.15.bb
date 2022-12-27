require ${BPN}.inc

SRC_URI = "${TELECHIPS_AUTOMOTIVE_BSP_GIT}/gpu.git;protocol=${ALS_GIT_PROTOCOL};branch=${ALS_BRANCH};"
SRC_URI += "file://pkgconfig"

SRCREV = "819c34bd1c0852fb900a92d7d0f343e338d550e9"

PATCHTOOL = "git"
COMPATIBLE_MACHINE = "tcc805x"
