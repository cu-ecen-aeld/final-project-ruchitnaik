
##############################################################
#
# Temperature Sensor (TMP102) I2C Package
#
##############################################################


TMP102_VERSION = '66e464721d5274ebcc03e785d6a11ddae673ce40'
# Note: Be sure to reference the *ssh* repository URL here (not https) to work properly
# with ssh keys and the automated build/test system.
# Your site should start with git@github.com:
TMP102_SITE = 'git@github.com:cu-ecen-aeld/final-project-DivyeshShashikant.git'
TMP102_SITE_METHOD = git
TMP102_GIT_SUBMODULES = YES

define TMP102_BUILD_CMDS
	$(MAKE) $(TARGET_CONFIGURE_OPTS) -C $(@D)/TMP102test all
endef

# TODO add your writer, finder and finder-test utilities/scripts to the installation steps below
define TMP102_INSTALL_TARGET_CMDS
	$(INSTALL) -m 0755 $(@D)/TMP102test/tmp102 $(TARGET_DIR)/usr/bin
	
endef

$(eval $(generic-package))
