#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xa4bb88f0, "module_layout" },
	{ 0x3085120, "sock_release" },
	{ 0x2bd52085, "__netlink_kernel_create" },
	{ 0xefaceb4b, "init_net" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0xeec1ebbf, "kfree_skb" },
	{ 0x39042ddb, "__nlmsg_put" },
	{ 0x4e2856a1, "netlink_unicast" },
	{ 0x8588defd, "__alloc_skb" },
	{ 0xe914e41e, "strcpy" },
	{ 0x50eedeb8, "printk" },
	{ 0xb4390f9a, "mcount" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "6757E0D11DE216E0B27FE94");
