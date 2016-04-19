//
//  input.c
//  GLES2Playground
//
//  Created by Michael Kwasnicki on 21.02.16.
//  Copyright (c) 2016 Michael Kwasnicki. All rights reserved.
//

#include "input.h"

#include <assert.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <unistd.h>



uint32_t testEventDevice(const char * const in_PATH) {
    uint32_t eventTypes = 0;
    int fd = open(in_PATH, O_RDONLY);
    
    if (fd == -1) {
        return 0;
    }
    
    assert(fd >= 0);
    
    int ret = ioctl(fd, EVIOCGBIT(0, EV_MAX), &eventTypes);
    assert(ret >= 0);
    
    printf("Supported event types in %s:\n", in_PATH);
    
    for (uint32_t yalv = 0; yalv < EV_MAX; yalv++) {
        if (eventTypes & (1 << yalv)) {
            /* the bit is set in the event types list */
            printf("  Event type 0x%02x ", yalv);
            switch (yalv)
            {
                case EV_SYN :
                    printf(" (Synch Events)\n");
                    break;
                case EV_KEY :
                    printf(" (Keys or Buttons)\n");
                    break;
                case EV_REL :
                    printf(" (Relative Axes)\n");
                    break;
                case EV_ABS :
                    printf(" (Absolute Axes)\n");
                    break;
                case EV_MSC :
                    printf(" (Miscellaneous)\n");
                    break;
                case EV_LED :
                    printf(" (LEDs)\n");
                    break;
                case EV_SND :
                    printf(" (Sounds)\n");
                    break;
                case EV_REP :
                    printf(" (Repeat)\n");
                    break;
                case EV_FF :
                case EV_FF_STATUS:
                    printf(" (Force Feedback)\n");
                    break;
                case EV_PWR:
                    printf(" (Power Management)\n");
                    break;
                default:
                    printf(" (Unknown: 0x%04hx)\n",
                           yalv);
            }
        }
    }
    
    close(fd);
    return eventTypes;
}
