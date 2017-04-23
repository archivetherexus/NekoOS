echo "Creating a bootable USB!"
ISO_IMAGE="$STEP_BUILD/boot.iso"
ISO_FS="$STEP_BUILD/iso"
mkdir -p "$ISO_FS/boot"

# Fixme \/ This is ugly
cp -r "$BUILD_DIR/Kernel/linux-stable/arch/x86/boot/bzImage" "$ISO_FS/boot/vmlinuz"
cp -r core.gz "$ISO_FS/boot"
cp -r isolinux "$ISO_FS/"

genisoimage -b isolinux/isolinux.bin -c isolinux/boot.cat -no-emul-boot -boot-load-size 4 -boot-info-table -l -input-charset default -V MyLinuxBoot -A "NekoOSv0" -o ${ISO_IMAGE} ${ISO_FS}

exit 0
