include $(TOPDIR)/rules.mk
PKG_NAME:=luci-app-openclash
PKG_VERSION:=0.48.002
PKG_RELEASE:=3
PKG_LICENSE:=GPL-3.0
include $(INCLUDE_DIR)/package.mk

define Package/luci-app-openclash
  SECTION:=luci
  CATEGORY:=LuCI
  SUBMENU:=3. Applications
  TITLE:=OpenClash official payload with N30 offline defaults
  PKGARCH:=all
  DEPENDS:=+bash +ca-bundle +curl +dnsmasq-full +ip-full +kmod-tun +kmod-inet-diag +kmod-nft-tproxy +luci-compat +ruby +ruby-yaml +unzip
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef
define Build/Configure
endef
define Build/Compile
endef
define Package/luci-app-openclash/conffiles
/etc/config/openclash
endef
define Package/luci-app-openclash/install
	$(CP) ./payload/. $(1)/
endef
$(eval $(call BuildPackage,luci-app-openclash))
