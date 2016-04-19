//
//  mouse.c
//  GLES2Playground
//
//  Created by Michael Kwasnicki on 21.02.16.
//  Copyright (c) 2016 Michael Kwasnicki. All rights reserved.
//

#include "mouse.h"

#include "input.h"

#include <assert.h>
#include <fcntl.h>
#include <iso646.h>
#include <linux/input.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>



void mouseInit() {
    uint32_t eventTypes;
    
    char * devices[] = {
        "/dev/input/event0",
        "/dev/input/event1",
        "/dev/input/event2",
        "/dev/input/event3",
        "/dev/input/event4",
        "/dev/input/event5",
        "/dev/input/event6",
        "/dev/input/event7",
        "/dev/input/event8",
        "/dev/input/event9"
    };
    
    memset(&s_mouse, 0, sizeof(Mouse_s));
    
    for (int d = 0; d < 10; d++) {
        eventTypes = testEventDevice(devices[d]);
        
        if ((eventTypes & (1 << EV_REL)) && (eventTypes & (1 << EV_REP))) {
            puts(devices[d]);
            s_mouse.fd = open(devices[d], O_RDONLY|O_NONBLOCK);
            assert(s_mouse.fd >= 0);
            break;
        }
    }
}



void mouseFree() {
    if (s_mouse.fd) {
        int ret = close(s_mouse.fd);
        assert(ret == 0);
        
        s_mouse.fd = 0;
    }
}



void mouseUpdate() {
    struct input_event m;
    Mouse_s acc;
    memset(&acc, 0, sizeof(Mouse_s));
    
    if (s_mouse.fd) {
        ssize_t bytes = read(s_mouse.fd, &m, sizeof(m));
        
        while (bytes == sizeof(m)) {
            if (m.type == EV_SYN) {
                // nothing to do on synchronization events
            } else if (m.type == EV_KEY) {
                if (m.code == BTN_LEFT) {
                    if (m.value == 0x01) {
                        //puts("LMB down");
                        acc.lmb = true;
                        acc.chg.lmbDn = true;
                    } else if (m.value == 0x02) {
                        //puts("LMB repeat");
                        acc.lmb = true;
                    } else {
                        //puts("LMB up");
                        acc.lmb = false;
                        acc.chg.lmbUp = true;
                    }
                } else if (m.code == BTN_MIDDLE) {
                    if (m.value == 0x01) {
                        //puts("MMB down");
                        acc.mmb = true;
                        acc.chg.mmbDn = true;
                    } else if (m.value == 0x02) {
                        //puts("MMB repeat");
                        acc.mmb = true;
                    } else {
                        //puts("MMB up");
                        acc.mmb = false;
                        acc.chg.mmbUp = true;
                    }
                } else if (m.code == BTN_RIGHT) {
                    if (m.value == 0x01) {
                        //puts("RMB down");
                        acc.rmb = true;
                        acc.chg.rmbDn = true;
                    } else if (m.value == 0x02) {
                        //puts("RMB repeat");
                        acc.rmb = true;
                    } else {
                        //puts("RMB up");
                        acc.rmb = false;
                        acc.chg.rmbUp = true;
                    }
                } else if (m.code == BTN_SIDE) {
                    if (m.value == 0x01) {
                        //puts("SMB down");
                        acc.smb = true;
                        acc.chg.smbDn = true;
                    } else if (m.value == 0x02) {
                        //puts("SMB repeat");
                        acc.smb = true;
                    } else {
                        //puts("SMB up");
                        acc.smb = false;
                        acc.chg.smbUp = true;
                    }
                } else if (m.code == BTN_EXTRA) {
                    if (m.value == 0x01) {
                        //puts("EMB down");
                        acc.emb = true;
                        acc.chg.embDn = true;
                    } else if (m.value == 0x01) {
                        //puts("EMB repeat");
                        acc.emb = true;
                    } else {
                        //puts("EMB up");
                        acc.emb = false;
                        acc.chg.embUp = true;
                    }
                } else {
                    printf("%04x, %04x, %08x\n", m.type, m.code, m.value);
                }
            } else if (m.type == EV_REL) {
                if (m.code == REL_X) {
                    acc.dx += m.value;
                    acc.chg.dx = true;
                } else if (m.code == REL_Y) {
                    acc.dy += m.value;
                    acc.chg.dy = true;
                } else if (m.code == REL_HWHEEL) {
                    acc.sx += m.value;
                    acc.chg.sx = true;
                } else if (m.code == REL_WHEEL) {
                    acc.sy += m.value;
                    acc.chg.sy = true;
                } else {
                    printf("%04x, %04x, %08x\n", m.type, m.code, m.value);
                }
            } else if (m.type == EV_MSC) {
                // do nothing
            } else {
                printf("%04x, %04x, %08x\n", m.type, m.code, m.value);
            }
            
            bytes = read(s_mouse.fd, &m, sizeof(m));
        }
        
        acc.x = s_mouse.x + acc.dx;
        acc.y = s_mouse.y + acc.dy;
        acc.fd = s_mouse.fd;
    }
    
    s_mouse = acc;
}


void mouseGet(void (* const in_MOUSE_MOVE)(const int8_t in_DX, const int8_t in_DY), void (* const in_MOUSE_DOWN)(const uint8_t in_BUTTON), void (* const in_MOUSE_UP)(const uint8_t in_BUTTON), void (* const in_MOUSE_SCROLL)(const int8_t in_DX, const int8_t in_DY)) {
    //printf("%d %d %d %d %d %d %d %d %d\n", mouse.chg.dx, mouse.chg.dy, mouse.chg.sx, mouse.chg.sy, mouse.chg.lmb, mouse.chg.mmb, mouse.chg.rmb, mouse.chg.smb, mouse.chg.emb);
    
    if (s_mouse.chg.dx or s_mouse.chg.dy) {
        in_MOUSE_MOVE(s_mouse.dx, s_mouse.dy);
    }
    
    if (s_mouse.chg.sx or s_mouse.chg.sy) {
        in_MOUSE_SCROLL(s_mouse.sx, s_mouse.sy);
    }
    
    if (s_mouse.chg.lmbDn and s_mouse.chg.lmbUp) {
        if (s_mouse.lmb) {
            in_MOUSE_UP(0);
            in_MOUSE_DOWN(0);
        } else {
            in_MOUSE_DOWN(0);
            in_MOUSE_UP(0);
        }
    } else if (s_mouse.chg.lmbDn) {
        in_MOUSE_DOWN(0);
    } else if (s_mouse.chg.lmbUp) {
        in_MOUSE_UP(0);
    }
    
    if (s_mouse.chg.mmbDn and s_mouse.chg.mmbUp) {
        if (s_mouse.mmb) {
            in_MOUSE_UP(1);
            in_MOUSE_DOWN(1);
        } else {
            in_MOUSE_DOWN(1);
            in_MOUSE_UP(1);
        }
    } else if (s_mouse.chg.mmbDn) {
        in_MOUSE_DOWN(1);
    } else if (s_mouse.chg.mmbUp) {
        in_MOUSE_UP(1);
    }
    
    if (s_mouse.chg.rmbDn and s_mouse.chg.rmbUp) {
        if (s_mouse.rmb) {
            in_MOUSE_UP(2);
            in_MOUSE_DOWN(2);
        } else {
            in_MOUSE_DOWN(2);
            in_MOUSE_UP(2);
        }
    } else if (s_mouse.chg.rmbDn) {
        in_MOUSE_DOWN(2);
    } else if (s_mouse.chg.rmbUp) {
        in_MOUSE_UP(2);
    }
    
    if (s_mouse.chg.smbDn and s_mouse.chg.smbUp) {
        if (s_mouse.smb) {
            in_MOUSE_UP(3);
            in_MOUSE_DOWN(3);
        } else {
            in_MOUSE_DOWN(3);
            in_MOUSE_UP(3);
        }
    } else if (s_mouse.chg.smbDn) {
        in_MOUSE_DOWN(3);
    } else if (s_mouse.chg.smbUp) {
        in_MOUSE_UP(3);
    }
    
    if (s_mouse.chg.embDn and s_mouse.chg.embUp) {
        if (s_mouse.emb) {
            in_MOUSE_UP(4);
            in_MOUSE_DOWN(4);
        } else {
            in_MOUSE_DOWN(4);
            in_MOUSE_UP(4);
        }
    } else if (s_mouse.chg.embDn) {
        in_MOUSE_DOWN(4);
    } else if (s_mouse.chg.embUp) {
        in_MOUSE_UP(4);
    }
}


void mouseReset() {
    //    s_mouse.x = 0;
    //    s_mouse.y = 0;
    //    s_mouse.dx = 0;
    //    s_mouse.dy = 0;
}


static int get_mouse(int *state, int *outx, int *outy)
{
    static int fd = -1;
    //const int width=state->screen_width, height=state->screen_height;
    const int width=1920, height=1200;
    static int x=800, y=400;
    const int XSIGN = 1<<4, YSIGN = 1<<5;
    if (fd<0) {
        fd = open("/dev/input/mouse0",O_RDONLY|O_NONBLOCK);
    }
    if (fd>=0) {
        struct {char buttons, dx, dy; } m;
        while (1) {
            size_t bytes = read(fd, &m, sizeof m);
            if (bytes < sizeof(m)) goto _exit;
            if (m.buttons&8) {
                break; // This bit should always be set
            }
            read(fd, &m, 1); // Try to sync up again
        }
        if (m.buttons&3)
            return m.buttons&3;
        x+=m.dx;
        y+=m.dy;
        if (m.buttons&XSIGN)
            x-=256;
        if (m.buttons&YSIGN)
            y-=256;
        if (x<0) x=0;
        if (y<0) y=0;
        if (x>width) x=width;
        if (y>height) y=height;
    }
_exit:
    printf("%i %i\n", x, y);
    if (outx) *outx = x;
    if (outy) *outy = y;
    return 0;
}
