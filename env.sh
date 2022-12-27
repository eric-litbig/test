ROOT=$PWD
BUILD_DIR=$ROOT/build
core=$1
audio=$2

if [ -z $core ]; then
	echo "---------------------------------------"
	echo "Note: Set default machine core as main"
	echo "---------------------------------------"
	core="main"
fi

if [ -z $audio ]; then
    echo "------------------------------------------"
    echo "Note: Set default audio codec to maincore"
    echo "------------------------------------------"
    audio="audio_main"
fi

if [ $core != "main" ] && [ $core != "sub" ]; then
	echo "* Uasge : source env.sh main (or sub)"
else
	echo "+++++ ENV SETUP : $core core +++++"
	echo "Type 'help' to show usages"

	if [ $core == "sub" ]; then
		SUPPORT_coreS="tcc8050-sub"
		KERNEL=$BUILD_DIR/$SUPPORT_coreS/tmp/work/tcc8050_sub-telechips-linux/linux-telechips/5.4.159-r0/git
		BOOTLOADER=$BUILD_DIR/tcc8050-sub/tmp/work/tcc8050_sub-telechips-linux/u-boot-tcc/1.0-r0/git
		source poky/meta-telechips/meta-subcore/subcore-build.sh tcc8050-sub 
	else
		SUPPORT_coreS="tcc8050-main"
		KERNEL=$BUILD_DIR/$SUPPORT_coreS/tmp/work/tcc8050_main-telechips-linux/linux-telechips/5.4.159-r0/git
		BOOTLOADER=$BUILD_DIR/tcc8050-main/tmp/work/tcc8050_main-telechips-linux/u-boot-tcc/1.0-r0/git
		source poky/meta-telechips/meta-ivi/ivi-build.sh tcc8050-main 
	fi

	META=poky/meta-telechips/
	META_MAIN=poky/meta-telechips/meta-ivi
	META_SUB=poky/meta-telechips/meta-subcore

	WORK=$BUILD_DIR/$SUPPORT_coreS/tmp/work/

	DEPLOY_MAIN_DIR_IMAGE=$ROOT/build/tcc8050-main/tmp/deploy/images/tcc8050-main
	DEPLOY_SUB_DIR_IMAGE=$ROOT/build/tcc8050-sub/tmp/deploy/images/tcc8050-sub
	IMAGE_OUT=$ROOT/fwdn/mkimg/out_images

#    if [ $audio == "audio_main" ]
#    then
#	    if [ $core == "sub" ]; then
#            sed -i '/SOUND_CARD_EN/d' $BUILD_DIR/tcc8050-sub/conf/local.conf
#            echo "SOUND_CARD_EN = \"main\"" >> $BUILD_DIR/tcc8050-sub/conf/local.conf
#        else
#            sed -i '/SOUND_CARD_EN/d' $BUILD_DIR/tcc8050-main/conf/local.conf
#            echo "SOUND_CARD_EN = \"main\"" >> $BUILD_DIR/tcc8050-main/conf/local.conf
#        fi
#    elif [ $audio == "audio_sub" ]
#    then
#	    if [ $core == "sub" ]; then
#            sed -i '/SOUND_CARD_EN/d' $BUILD_DIR/tcc8050-sub/conf/local.conf
#            echo "SOUND_CARD_EN = \"sub\"" >> $BUILD_DIR/tcc8050-sub/conf/local.conf
#        else
#            sed -i '/SOUND_CARD_EN/d' $BUILD_DIR/tcc8050-main/conf/local.conf
#            echo "SOUND_CARD_EN = \"sub\"" >> $BUILD_DIR/tcc8050-main/conf/local.conf
#        fi
#    fi

	function help()
	{
		echo "<< Shortcut commands >>"
		echo "1. envmain : set enviroment as main core"
		echo "2. envsub : set enviroment as sub core"
		echo "3. cdk : go to main kernel"
		echo "4. cdb : go to main/sub bootloader(uboot)"
		echo "5. cdw : go to work"
		echo "6. croot : go to root"
		echo "7. cdmeta : to to meta-meta (main or sub : depends env core setting)"
		echo "8. cdmeta-main : to to meta-meta main core"
		echo "9. cdmeta-sub : to to meta-meta sub core"
		echo "10. buildmain : build main core image"
		echo "11. buildsub : build sub core image"
		echo "12. buildall : build main & sub core image"
		echo "13. cproms : copy deploy images to out image folder"
		echo "14. mkfai : make SD_Data.fai from cprom images (main&sub)"
		echo "15. fwdn : cproms && mkfai"
	}

	function envmain()
	{
		croot
		source env.sh main
	}

	function envsub()
	{
		croot
		source env.sh sub
	}

	function croot()
	{
		\cd $ROOT
	}

	function cdk()
	{
		croot
		\cd $KERNEL
	}

	function cdb()
	{
		croot
		\cd $BOOTLOADER
	}

	function cdw()
	{
		croot
		\cd $WORK
	}
	function cddts()
	{
		croot
		\cd $KERNEL/arch/arm64/boot/dts/tcc/
	}

	function cddmain()
	{
		cd $DEPLOY_MAIN_DIR_IMAGE
	}

	function cddsub()
	{
		cd $DEPLOY_SUB_DIR_IMAGE
	}

	function cdmeta()
	{
		croot
		cd $META
	}

	function cdmeta-main()
	{
		croot
		cd $META_MAIN
	}

	function cdmeta-sub()
	{
		croot
		cd $META_SUB
	}

	function buildmain()
	{
		croot
		source env.sh main $audio
		bitbake automotive-linux-platform-image 
	}

	function buildsub()
	{
		croot
		source env.sh sub $audio
		bitbake telechips-subcore-image
	}

	function buildb()
	{
		bitbake u-boot-tcc -C compile -f
		bitbake u-boot-tcc -c deploy -f
	}

	function buildk()
	{
		bitbake linux-telechips -C compile -f
		bitbake linux-telechips -c deploy -f
	}

	function buildall()
	{
		buildmain && buildsub
	}

	function cpk()
	{
		cdk && \cd ..
		if [ $core == "main" ]; then
			rm -rfv ~/tmp/tc-boot-tcc8050-main.img
			rm -rfv ~/tmp/tcc8050-linux-ivi-tost_sv0.1-tcc8050-main.dtb
			cp -v deploy-linux-telechips/tc-boot-tcc8050-main.img ~/tmp/
			cp -v deploy-linux-telechips/tcc8050-linux-ivi-tost_sv0.1-tcc8050-main.dtb ~/tmp/
		else
			cp -v deploy-linux-telechips/tc-boot-tcc8050-sub.img ~/tmp/
			cp -v deploy-linux-telechips/tcc8050-linux-subcore_sv0.1-tcc8050-sub.dtb ~/tmp/
		fi
	}

	function mkfai()
	{
		cd $ROOT/fwdn/mkimg/
		$ROOT/fwdn/mkimg/mkfai.sh $ROOT/fwdn/mkimg/mktcimg
	}

        function cpimgs()	
	{

		IMAGE_OUT=~/build_image/fwdn/deploy-fwdn/out_images

		echo $IMAGE_OUT

		rm -rfv $IMAGE_OUT
		mkdir -p  $IMAGE_OUT
		cp -v $DEPLOY_MAIN_DIR_IMAGE/ca72_bl3.rom $IMAGE_OUT
		cp -v $DEPLOY_MAIN_DIR_IMAGE/tc-boot-tcc8050-main.img $IMAGE_OUT
		cp -fv $DEPLOY_MAIN_DIR_IMAGE/tcc8050-linux-ivi-tost_sv0.1-tcc8050-main.dtb $IMAGE_OUT
		cp -fv $DEPLOY_MAIN_DIR_IMAGE/automotive-linux-platform-image-tcc8050-main.ext4 $IMAGE_OUT

		cp -v $DEPLOY_SUB_DIR_IMAGE/ca53_bl3.rom  $IMAGE_OUT
		cp -v $DEPLOY_SUB_DIR_IMAGE/tc-boot-tcc8050-sub.img $IMAGE_OUT
		cp -v $DEPLOY_SUB_DIR_IMAGE/tcc8050-linux-subcore-ivi-tost_sv0.1-tcc8050-sub.dtb $IMAGE_OUT
		cp -v $DEPLOY_SUB_DIR_IMAGE/telechips-subcore-image-tcc8050-sub.ext4 $IMAGE_OUT
	}

	function cproms()
	{
		rm -rfv $IMAGE_OUT
		mkdir -p  $IMAGE_OUT
		cp -v $DEPLOY_MAIN_DIR_IMAGE/ca72_bl3.rom $IMAGE_OUT
		cp -v $DEPLOY_MAIN_DIR_IMAGE/tc-boot-tcc8050-main.img $IMAGE_OUT
		cp -fv $DEPLOY_MAIN_DIR_IMAGE/tcc8050-linux-ivi-tost_sv0.1-tcc8050-main.dtb $IMAGE_OUT
		cp -fv $DEPLOY_MAIN_DIR_IMAGE/automotive-linux-platform-image-tcc8050-main.ext4 $IMAGE_OUT

		cp -v $DEPLOY_SUB_DIR_IMAGE/ca53_bl3.rom  $IMAGE_OUT
		cp -v $DEPLOY_SUB_DIR_IMAGE/tc-boot-tcc8050-sub.img $IMAGE_OUT
		cp -v $DEPLOY_SUB_DIR_IMAGE/tcc8050-linux-subcore-ivi-tost_sv0.1-tcc8050-sub.dtb $IMAGE_OUT
		cp -v $DEPLOY_SUB_DIR_IMAGE/telechips-subcore-image-tcc8050-sub.ext4 $IMAGE_OUT
	}

	function fwdn()
	{
		cproms && mkfai
	}
	
fi
