echo "Building Kernel!"
cd $STEP_BUILD

# Build the Linux kernel automatically.

if [ ! -d "linux-stable" ]; then
	# Get the kernel sources from github.

	git clone git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git
fi
cd linux-stable
if [ ! -f ".config" ]; then
	# Configure the kernel.

	make defconfig
fi

# Make the kernel.
make -j6
STATUS_0=$?
make bzImage
STATUS_1=$?

exit $STATUS_0
