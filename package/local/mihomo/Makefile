include $(TOPDIR)/rules.mk
PKG_NAME:=mihomo
PKG_VERSION:=1.19.32
PKG_RELEASE:=3
PKG_LICENSE:=GPL-3.0
include $(INCLUDE_DIR)/package.mk
# Keep upstream's verified static ELF byte-for-byte.
STRIP:=:
RSTRIP:=:
define Package/mihomo
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Mihomo pinned ARM64 core for OpenClash
  DEPENDS:=@aarch64
endef
define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef
define Build/Configure
endef
define Build/Compile
endef
define Package/mihomo/install
	$(INSTALL_DIR) $(1)/etc/openclash/core
	$(INSTALL_BIN) ./files/clash_meta $(1)/etc/openclash/core/clash_meta
endef
$(eval $(call BuildPackage,mihomo))
