#include <fcntl.h>
#include <linux/uinput.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>

struct Output {
    int width, height;
};
std::vector<Output> outputs = {
    {1600, 900},
    {1920, 1080},
    {1360, 768}
};
int uinput_fd = -1;

void setup_uinput() {
    if (uinput_fd != -1) return;

    uinput_fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (uinput_fd < 0) {
        std::cerr << "Failed to open /dev/uinput" << std::endl;
        return;
    }
    ioctl(uinput_fd, UI_SET_EVBIT, EV_KEY);
    ioctl(uinput_fd, UI_SET_KEYBIT, BTN_LEFT);

    ioctl(uinput_fd, UI_SET_EVBIT, EV_REL);
    ioctl(uinput_fd, UI_SET_RELBIT, REL_X);
    ioctl(uinput_fd, UI_SET_RELBIT, REL_Y);

    struct uinput_setup usetup;
    memset(&usetup, 0, sizeof(usetup));
    usetup.id.bustype = BUS_USB;
    usetup.id.vendor = 0x1234;
    usetup.id.product = 0x5678;
    strcpy(usetup.name, "WCenterMouse");

    ioctl(uinput_fd, UI_DEV_SETUP, &usetup);
    ioctl(uinput_fd, UI_DEV_CREATE);

    sleep(1);
}

// Function to move the pointer to a specified position using uinput
void move_pointer(int x) {
    setup_uinput();

    struct input_event ev;
    memset(&ev, 0, sizeof(ev));

    ev.type = EV_REL;
    ev.code = REL_X;
    ev.value = x;
    if (write(uinput_fd, &ev, sizeof(ev)) < 0) {
        std::cerr << "Failed to write REL_X event" << std::endl;
    }

    ev.type = EV_SYN;
    ev.code = SYN_REPORT;
    ev.value = 0;
    if (write(uinput_fd, &ev, sizeof(ev)) < 0) {
        std::cerr << "Failed to write SYN_REPORT event" << std::endl;
    }

    std::cout << "Moved pointer to position: " << x << std::endl;
    sleep(1);
}

// Main function to connect to Wayland, gather screen data, and move the pointer
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <monitor_number>" << std::endl;
        return -1;
    }

    int monitor_index = std::atoi(argv[1]);

    const char* xdg_runtime_dir = std::getenv("XDG_RUNTIME_DIR");
    if (xdg_runtime_dir == nullptr) {
        std::cerr << "Error: XDG_RUNTIME_DIR is invalid or not set in the environment." << std::endl;
        return 1;
    } else {
        std::cout << "XDG_RUNTIME_DIR is set to: " << xdg_runtime_dir << std::endl;
    }

    move_pointer(-5000);

    // Get the selected monitor
    int x_position = 0;
    for (int i = 0; i < monitor_index - 1; ++i) {
        x_position += outputs[i].width;
    }

    if (monitor_index > 1) {
        x_position -= outputs[monitor_index].width / 3;
        move_pointer(x_position);
    }

    return 0;
}
