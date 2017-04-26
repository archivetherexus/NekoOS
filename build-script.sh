rm Build/BOOT-USB/boot.iso

# Get project dir from script location.
export PROJECT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Get the toolchain location.
export TOOLCHAIN="$PROJECT_DIR/Toolchain"

# The location of all the required steps.
export STEPS_DIR="$PROJECT_DIR/Steps"

# The directory where to put all the build outputs.
export BUILD_DIR="$PROJECT_DIR/Build"

# Add toolchain to path.
export PATH="$PATH:$TOOLCHAIN"

# Build the operating system!
cd "$PROJECT_DIR"
do-step "BOOT-USB"




