#include <iostream>
#if GEKKO
#include <gccore.h>
#if HW_RVL
#include <wiiuse/wpad.h>
#endif

static void *xfb = nullptr;
static GXRModeObj* rmode = nullptr;

#endif

#include <athena/MemoryReader.hpp>

int main()
{
#if GEKKO
    VIDEO_Init();
#if HW_RVL
    WPAD_Init();
#endif
    PAD_Init();

    rmode = VIDEO_GetPreferredMode(nullptr);
    xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

    CON_Init(xfb, 20, 20, rmode->fbWidth, rmode->xfbHeight, rmode->fbWidth * VI_DISPLAY_PIX_SZ);
    CON_EnableGecko(CARD_SLOTB, true);

    VIDEO_Configure(rmode);
    VIDEO_SetBlack(false);

    VIDEO_Flush();

    VIDEO_WaitVSync();
    if (rmode->viTVMode & VI_NON_INTERLACE)
        VIDEO_WaitVSync();

    athena::io::MemoryCopyReader test("sd:/test.dat");
    while(true)
    {
#if HW_RVL
        WPAD_ScanPads();
#endif
        PAD_ScanPads();
        if (PAD_ButtonsDown(0) & PAD_BUTTON_START)
            break;

#if HW_RVL
        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME)
            break;
#endif
        VIDEO_WaitVSync();
    }
#endif
    return 0;
}

