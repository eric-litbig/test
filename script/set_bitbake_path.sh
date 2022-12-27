#!/bin/bash

DIR=$PWD

VAR1="main"
VAR2="sub"
VAR3="path"

if [ "$VAR1" = "$1" ]; then
    source $TOSTDIR/poky/oe-init-build-env $TOSTDIR/build/tcc8050-main
    echo "source $TOSTDIR/poky/oe-init-build-env $TOSTDIR/build/tcc8050-main"
elif [ "$VAR2" = "$1" ]; then
    source $TOSTDIR/poky/oe-init-build-env $TOSTDIR/build/tcc8050-sub
    echo "source $TOSTDIR/poky/oe-init-build-env $TOSTDIR/build/tcc8050-sub"
elif [ "$VAR3" = "$1" ]; then
	if [ -z "$2" ]; then
		export TOSTDIR=$DIR
		export TOSTPOKYDIR=$DIR/poky
		echo "export TOSTDIR=$DIR"
	else
		export TOSTDIR=$2
		export TOSTPOKYDIR=$2/poky
		echo "export TOSTDIR=$2"
	fi
else
    echo "$1 is not match !"
fi

echo "================================================================="
echo "TOSTDIR=$TOSTDIR"
echo "BUILDDIR=$BUILDDIR"

cd $DIR
