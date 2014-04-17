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
	{ 0x6a91b2bc, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x88ea4119, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0xf5769951, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x809950d5, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0xddaf8796, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0x50eedeb8, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x2f287f0d, __VMLINUX_SYMBOL_STR(copy_to_user) },
	{ 0xb4390f9a, __VMLINUX_SYMBOL_STR(mcount) },
	{ 0xe871a17f, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0xf2975f0b, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0x439a4a19, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0xcc84ea7c, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x362ef408, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0x8d7724c0, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "0E9189725DDF93ED4EB66BC");
