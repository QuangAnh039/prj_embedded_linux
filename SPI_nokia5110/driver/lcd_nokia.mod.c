#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(.gnu.linkonce.this_module) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section(__versions) = {
	{ 0x441199c1, "module_layout" },
	{ 0x71eadc8, "cdev_del" },
	{ 0xda203910, "kmalloc_caches" },
	{ 0x5f53d92f, "cdev_init" },
	{ 0xf9a482f9, "msleep" },
	{ 0x47229b5c, "gpio_request" },
	{ 0x597168ea, "gpio_to_desc" },
	{ 0xa1c0b261, "device_destroy" },
	{ 0xf4b43dd0, "__spi_register_driver" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
	{ 0xae353d77, "arm_copy_from_user" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x28118cb6, "__get_user_1" },
	{ 0x5f754e5a, "memset" },
	{ 0xc5850110, "printk" },
	{ 0x56940520, "driver_unregister" },
	{ 0x3d3e385, "device_create" },
	{ 0x696abdc5, "gpiod_direction_output_raw" },
	{ 0x68f31cbd, "__list_add_valid" },
	{ 0x16033049, "spi_sync" },
	{ 0xc663131e, "cdev_add" },
	{ 0xdecd0b29, "__stack_chk_fail" },
	{ 0x8bc8bc00, "of_get_named_gpio_flags" },
	{ 0x4e920be5, "kmem_cache_alloc_trace" },
	{ 0xfe990052, "gpio_free" },
	{ 0x37a0cba, "kfree" },
	{ 0x47880fa3, "gpiod_set_raw_value" },
	{ 0xebc96d1c, "class_destroy" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0xdcfb973, "__class_create" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Cnokia5110-LCD");
MODULE_ALIAS("of:N*T*Cnokia5110-LCDC*");
