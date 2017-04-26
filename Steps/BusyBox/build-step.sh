echo "Building BusyBox!"
cd $STEP_BUILD
if [ ! -d "busybox" ]; then
	git clone git://git.busybox.net/busybox
	cp $STEP_DIR/config $STEP_BUILD/busybox/.config
fi
cd busybox
make
exit 0
