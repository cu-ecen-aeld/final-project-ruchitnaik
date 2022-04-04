#!/bin/sh
# Shared definitions for buildroot scripts

# The defconfig from the buildroot directory we use for qemu builds
QEMU_DEFCONFIG=configs/qemu_aarch64_virt_defconfig
# The default hardware platform, when building for hardware
HARDWARE_DEFCONFIG=configs/beaglebone_defconfig
# The place we store customizations to the qemu configuration
MODIFIED_QEMU_DEFCONFIG=base_external/configs/aesd_qemu_defconfig
# The place we store customizations for the project current target
MODIFIED_HARDWARE_DEFCONFIG=base_external/configs/aesd_beaglebone_defconfig
# The defconfig from the buildroot directory we use for the project
AESD_DEFAULT_DEFCONFIG=${HARDWARE_DEFCONFIG}
AESD_MODIFIED_DEFCONFIG=${MODIFIED_HARDWARE_DEFCONFIG}
AESD_MODIFIED_DEFCONFIG_REL_BUILDROOT=../${AESD_MODIFIED_DEFCONFIG}
