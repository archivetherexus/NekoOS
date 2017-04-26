echo "Building $STEP_NAME!"

cd "$STEP_BUILD"

# Compile dash
if [ ! -d "dash" ]; then
	git clone https://git.kernel.org/pub/scm/utils/dash/dash.git
	cd dash
	./autogen.sh
	./configure --enable-static
else
	cd dash
fi
PATH_DASH=$(pwd)
make
strip --strip-all "src/dash"
cd ..

# Compile mount & other stuff.


if [ ! -d "util-linux" ]; then
	git clone https://git.kernel.org/pub/scm/utils/util-linux/util-linux.git
	cd util-linux
	./autogen.sh
	./configure --enable-static
else
	cd util-linux
fi
PATH_UTIL_LINUX=$(pwd)
make
cd ..


# Create the initramfs directory
if [ ! -d "initramfs" ]; then
	mkdir initramfs
fi
cd initramfs
cp "$STEP_DIR/init" init
chmod +x init
cp "$PATH_UTIL_LINUX/mount" mount
cp "$PATH_DASH/src/dash" dash
chmod +x dash
cd ..

cd initramfs
#find . -print0 | cpio --null -ov --format=newc | gzip -9 > ../initrd.gz
#find . | cpio -oHnewc | gzip > ../initramfs.gz
find . -print0 | cpio --null -ov --format=newc > ../initramfs.cpio
cd ..

exit 0
