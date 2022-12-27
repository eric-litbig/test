#!/usr/bin/python
import os

# Supported MACHINEs
#Machines = ['tcc8050-main', 'tcc8050-sub']
Machines = ['tcc8050-main', 'tcc8050-sub', 'tcc8050-cr5']

# Manifests versions
Manifests = [['tcc8050_linux_ivi_tost_0.1.0.xml','2022/04/08']]

# Manifest URLs
def get_url():

    stream = os.popen('git remote -v | grep mirror')
    output = stream.read()

    if output:
        url = 'https://git.huconn.com/linux_ivi_mirror/manifest.git'
    else:
        url = 'ssh://git@git.huconn.com/linux_ivi/manifest.git'

    return url

ManifestsURL = get_url()

MainImages = ['automotive-linux-platform-image']
# MainImages = [
#              'telechips-ivi-image-minimal',
# 		     'telechips-ivi-image-multimedia',
# 			 'telechips-ivi-image',
# 			 'automotive-linux-platform-image']

#telechips-ivi-image-minimal
#telechips-ivi-image-gstreamer(minimal + GStreamer)
#telechips-ivi-image-qt(minimal + Qt)
#telechips-ivi-image(minimal + GStreamer + Qt)
#   meta-toolchain-telechips(Application Development Toolkit)
#   meta-toolchain-telechips-ivi(Application Development Toolkit include GStreamer)
#   meta-toolchain-telechips-qt5(Application Development Toolkit include GStreamer and Qt5)
#Telechips Automotive Linux Platform targets are:
#    automotive-linux-platform-image(telechips-ivi-image + demo applications)

SubImages = ['telechips-subcore-image']

MainBuildScript = 'poky/meta-telechips/meta-ivi/ivi-build.sh'

SubBuildScript = 'poky/meta-telechips/meta-subcore/subcore-build.sh'

MainFeatures = []
SubFeatures = []
# Default Features Configuration
#MainFeatures = [
#	['with-subcore', True],
#	['gpu-vz', False],
#	['cluster-display', False],
#	['hud-display', False],
#	['support-4k-video', True],
#	['ufs', False],
#	['optee', False],
#	['bootchart', False],
#	['network', False],
#	['ssh-server-openssh', False],
#	['early-camera', True],
#	['secure-boot', False],
#	['current', False],
#	['kernel-4.14', False],
#	['meta-micom', True],
#	['meta-update', True],
#	['meta-hdmi', False],
#	['meta-str', False],
#	['meta-apple', False],
#	['meta-bluetooth', False],
#	['meta-hdradio', False],
#	['meta-drmradio', False],
#	['meta-dabradio', False],
#	['meta-commercial', False]
#]
#SubFeatures = [
#	['rvc', True],
#	['gpu-vz', False],
#	['cluster', False],
#	['ufs', False],
#	['secure-boot', False],
#	['current', False],
#	['kernel-4.14', False],
#	['early-camera', True],
#	['meta-micom', True],
#	['meta-update', True],
#	['meta-str', False]
#]
