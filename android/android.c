/* all driver need this */
#define DEBUG
#ifdef HAVE_XORG_CONFIG_H
#include <xorg-config.h>
#endif
#include <stdio.h>
#include <unistd.h>
#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86Priv.h"
#include "android.h"
#include "fbpriv.h"
#include "globals.h"
#include <X11/extensions/dpmsconst.h>
#define PAGE_MASK               (~(getpagesize() - 1))
//#define DOUT printf
#define DOUT ErrorF
static XF86ModuleVersionInfo androidHWVersRec = {
    "android",
    MODULEVENDORSTRING,
    MODINFOSTRING1,
    MODINFOSTRING2,
    XORG_VERSION_CURRENT,
    0, 0, 2,
    ABI_CLASS_VIDEODRV,
    ABI_VIDEODRV_VERSION,
    MOD_CLASS_NONE,
    {0, 0, 0, 0}
};
static MODULESETUPPROTO(androidsetup);

_X_EXPORT XF86ModuleData androidModuleData = {
    &androidHWVersRec,
    androidsetup,
    NULL
};
/* -------------------------------------------------------------------- */
/* our private data, and two functions to allocate/free this            */
#define FBDEVHWPTRLVAL(p) (p)->privates[androidHWPrivateIndex].ptr
#define FBDEVHWPTR(p) ((androidHWPtr)(FBDEVHWPTRLVAL(p)))
static int androidHWPrivateIndex = -1;
typedef struct {
    /* framebuffer device: filename (/dev/fb*), handle, more */
    char *device;
    int fd;
    void *fbmem;
    unsigned int fbmem_len;
    unsigned int fboff;
    char *mmio;
    unsigned int mmio_len;
    /* current hardware state */
    struct fb_fix_screeninfo fix;
    struct fb_var_screeninfo var;
    /* saved video mode */
    struct fb_var_screeninfo saved_var;
    /* buildin video mode */
    DisplayModeRec buildin;
} androidHWRec, *androidHWPtr;
static void androidIdentify (int flags)
{
ErrorF("[%s:%d] %d\n", __FUNCTION__, __LINE__, flags);
}
//static Bool androidoProbe 
//{
//ErrorF("[%s:%d]\n", __FUNCTION__, __LINE__);
//return 1;
//}
static DisplayModeRec buildin;
static Bool androidinitp(//SCREEN_INIT_ARGS_DECL)
ScreenPtr a, int b, char ** c)
{
ErrorF("[%s:%d]\n", __FUNCTION__, __LINE__);
return 1;
}
static MonRec monglob;
static Bool androidoProbe //(DriverPtr           driver,
                            //int                 entity_num,
                            //struct pci_device   *device,
                            //intptr_t            match_data)
(struct _DriverRec * driver, int flags)
{
ErrorF("[%s:%d] %p %x\n", __FUNCTION__, __LINE__, driver, flags);
int i;
screenLayoutPtr layout;
GDevPtr mydevice = NULL;
	ScrnInfoPtr scrn;

	scrn = xf86AllocateScreen(driver, 0);
	if (scrn == NULL)
		return FALSE;

	scrn->driverVersion = 1;
	scrn->driverName = "android";
	scrn->name = "android";
	scrn->driverPrivate = (void *)(1);
	scrn->Probe = NULL;
	scrn->modes = &buildin;
        scrn->ScreenInit = androidinitp;
        scrn->monitor = &monglob;
          //xf86MonPtr DDC = (xf86MonPtr) (xf86Screens[i]->monitor->DDC);

ErrorF("[%s:%d]\n", __FUNCTION__, __LINE__);
#if 1
    for (i = 0; i < xf86NumScreens; i++) {
        for (layout = xf86ConfigLayout.screens; layout->screen; layout++) {
ErrorF("[%s:%d] layout %p screen %p dev %p\n", __FUNCTION__, __LINE__, layout, layout->screen, layout->screen->device);
            mydevice = layout->screen->device;
ErrorF("[%s:%d] screen %p\n", __FUNCTION__, __LINE__, mydevice);
            break;
        }
    }
    int entity_num = xf86ClaimNoSlot(driver, 0, mydevice, 1);
	if (xf86IsEntitySharable(entity_num))
		xf86SetEntityShared(entity_num);
ErrorF("[%s:%d] scrn %x %x\n", __FUNCTION__, __LINE__, scrn, entity_num);
	xf86AddEntityToScreen(scrn, entity_num);
#endif
//ErrorF("[%s:%d] before IC\n", __FUNCTION__, __LINE__);
	//xf86InitialConfiguration(scrn, TRUE);
ErrorF("[%s:%d] return\n", __FUNCTION__, __LINE__);
return 1;
}
static const OptionInfoRec *androidAvailableOptions (int chipid, int bustype)
{
ErrorF("[%s:%d] %x %x\n", __FUNCTION__, __LINE__, chipid, bustype);
return NULL;
}
static Bool androiddriverfunc(ScrnInfoPtr pScrn,
                              xorgDriverFuncOp op,
                              pointer ptr)
{
ErrorF("[%s:%d] return\n", __FUNCTION__, __LINE__);
return 1;
}
struct pci_id_match {
    uint32_t    vendor_id;
    uint32_t    device_id;
    uint32_t    subvendor_id;
    uint32_t    subdevice_id;
    uint32_t    device_class;
    uint32_t    device_class_mask;
    intptr_t    match_data;
};
static struct pci_id_match pcimatch[2]; // = { 0};

static DriverRec andriver = {
    1, //VERSION
    "android", //name/
    androidIdentify,
    androidoProbe,
    androidAvailableOptions,
    NULL, //module
    0, //refcount
    //androiddriverfunc,
    //pcimatch,
    //androidProbe,
    };

static pointer androidsetup(pointer module, pointer opts, int *errmaj, int *errmin)
{
DOUT("[%s:%d] %p %p %p %p\n", __FUNCTION__, __LINE__, module, opts, errmaj, errmin);
static int initialized;
if (!initialized)
xf86AddDriver(&andriver, module, HaveDriverFuncs);
initialized = 1;
    return (pointer) 1;
}
Bool
androidHWGetRec(ScrnInfoPtr pScrn)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    androidHWPtr fPtr;
    if (androidHWPrivateIndex < 0)
        androidHWPrivateIndex = xf86AllocateScrnInfoPrivateIndex();
    if (FBDEVHWPTR(pScrn) != NULL)
        return TRUE;
    fPtr = FBDEVHWPTRLVAL(pScrn) = xnfcalloc(sizeof(androidHWRec), 1);
    return TRUE;
}
void
androidHWFreeRec(ScrnInfoPtr pScrn)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    if (androidHWPrivateIndex < 0)
        return;
    free(FBDEVHWPTR(pScrn));
    FBDEVHWPTRLVAL(pScrn) = NULL;
}
int
androidHWGetFD(ScrnInfoPtr pScrn)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    androidHWPtr fPtr;
    androidHWGetRec(pScrn);
    fPtr = FBDEVHWPTR(pScrn);
    return fPtr->fd;
}
/* -------------------------------------------------------------------- */
/* some helpers for printing debug informations                         */
#if DEBUG
static void
print_android_mode(const char *txt, struct fb_var_screeninfo *var)
{
    ErrorF("android %s mode:\t%d   %d %d %d %d   %d %d %d %d   %d %d:%d:%d\n",
           txt, var->pixclock,
           var->xres, var->right_margin, var->hsync_len, var->left_margin,
           var->yres, var->lower_margin, var->vsync_len, var->upper_margin,
           var->bits_per_pixel,
           var->red.length, var->green.length, var->blue.length);
}
static void
print_xfree_mode(const char *txt, DisplayModePtr mode)
{
    ErrorF("xfree %s mode:\t%d   %d %d %d %d   %d %d %d %d\n",
           txt, mode->Clock,
           mode->HDisplay, mode->HSyncStart, mode->HSyncEnd, mode->HTotal,
           mode->VDisplay, mode->VSyncStart, mode->VSyncEnd, mode->VTotal);
}
#endif
/* -------------------------------------------------------------------- */
/* Convert timings between the XFree and the Frame Buffer Device        */
static void
xfree2android_fblayout(ScrnInfoPtr pScrn, struct fb_var_screeninfo *var)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    var->xres_virtual = pScrn->displayWidth ? pScrn->displayWidth :
        pScrn->virtualX;
    var->yres_virtual = pScrn->virtualY;
    var->bits_per_pixel = pScrn->bitsPerPixel;
    if (pScrn->defaultVisual == TrueColor ||
        pScrn->defaultVisual == DirectColor) {
        var->red.length = pScrn->weight.red;
        var->green.length = pScrn->weight.green;
        var->blue.length = pScrn->weight.blue;
    }
    else {
        var->red.length = 8;
        var->green.length = 8;
        var->blue.length = 8;
    }
}
static void
xfree2android_timing(DisplayModePtr mode, struct fb_var_screeninfo *var)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    var->xres = mode->HDisplay;
    var->yres = mode->VDisplay;
    if (var->xres_virtual < var->xres)
        var->xres_virtual = var->xres;
    if (var->yres_virtual < var->yres)
        var->yres_virtual = var->yres;
    var->xoffset = var->yoffset = 0;
    var->pixclock = mode->Clock ? 1000000000 / mode->Clock : 0;
    var->right_margin = mode->HSyncStart - mode->HDisplay;
    var->hsync_len = mode->HSyncEnd - mode->HSyncStart;
    var->left_margin = mode->HTotal - mode->HSyncEnd;
    var->lower_margin = mode->VSyncStart - mode->VDisplay;
    var->vsync_len = mode->VSyncEnd - mode->VSyncStart;
    var->upper_margin = mode->VTotal - mode->VSyncEnd;
    var->sync = 0;
    if (mode->Flags & V_PHSYNC)
        var->sync |= FB_SYNC_HOR_HIGH_ACT;
    if (mode->Flags & V_PVSYNC)
        var->sync |= FB_SYNC_VERT_HIGH_ACT;
    if (mode->Flags & V_PCSYNC)
        var->sync |= FB_SYNC_COMP_HIGH_ACT;
    if (mode->Flags & V_BCAST)
        var->sync |= FB_SYNC_BROADCAST;
    if (mode->Flags & V_INTERLACE)
        var->vmode = FB_VMODE_INTERLACED;
    else if (mode->Flags & V_DBLSCAN)
        var->vmode = FB_VMODE_DOUBLE;
    else
        var->vmode = FB_VMODE_NONINTERLACED;
}
static Bool
android_modes_equal(struct fb_var_screeninfo *set, struct fb_var_screeninfo *req)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    return (set->xres_virtual >= req->xres_virtual &&
            set->yres_virtual >= req->yres_virtual &&
            set->bits_per_pixel == req->bits_per_pixel &&
            set->red.length == req->red.length &&
            set->green.length == req->green.length &&
            set->blue.length == req->blue.length &&
            set->xres == req->xres && set->yres == req->yres &&
            set->right_margin == req->right_margin &&
            set->hsync_len == req->hsync_len &&
            set->left_margin == req->left_margin &&
            set->lower_margin == req->lower_margin &&
            set->vsync_len == req->vsync_len &&
            set->upper_margin == req->upper_margin &&
            set->sync == req->sync && set->vmode == req->vmode);
}
static void
android2xfree_timing(struct fb_var_screeninfo *var, DisplayModePtr mode)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    mode->Clock = var->pixclock ? 1000000000 / var->pixclock : 0;
    mode->HDisplay = var->xres;
    mode->HSyncStart = mode->HDisplay + var->right_margin;
    mode->HSyncEnd = mode->HSyncStart + var->hsync_len;
    mode->HTotal = mode->HSyncEnd + var->left_margin;
    mode->VDisplay = var->yres;
    mode->VSyncStart = mode->VDisplay + var->lower_margin;
    mode->VSyncEnd = mode->VSyncStart + var->vsync_len;
    mode->VTotal = mode->VSyncEnd + var->upper_margin;
    mode->Flags = 0;
    mode->Flags |= var->sync & FB_SYNC_HOR_HIGH_ACT ? V_PHSYNC : V_NHSYNC;
    mode->Flags |= var->sync & FB_SYNC_VERT_HIGH_ACT ? V_PVSYNC : V_NVSYNC;
    mode->Flags |= var->sync & FB_SYNC_COMP_HIGH_ACT ? V_PCSYNC : V_NCSYNC;
    if (var->sync & FB_SYNC_BROADCAST)
        mode->Flags |= V_BCAST;
    if ((var->vmode & FB_VMODE_MASK) == FB_VMODE_INTERLACED)
        mode->Flags |= V_INTERLACE;
    else if ((var->vmode & FB_VMODE_MASK) == FB_VMODE_DOUBLE)
        mode->Flags |= V_DBLSCAN;
    mode->SynthClock = mode->Clock;
    mode->CrtcHDisplay = mode->HDisplay;
    mode->CrtcHSyncStart = mode->HSyncStart;
    mode->CrtcHSyncEnd = mode->HSyncEnd;
    mode->CrtcHTotal = mode->HTotal;
    mode->CrtcVDisplay = mode->VDisplay;
    mode->CrtcVSyncStart = mode->VSyncStart;
    mode->CrtcVSyncEnd = mode->VSyncEnd;
    mode->CrtcVTotal = mode->VTotal;
    mode->CrtcHAdjusted = FALSE;
    mode->CrtcVAdjusted = FALSE;
}
/* -------------------------------------------------------------------- */
Bool
androidHWProbe(struct pci_device *pPci, char *device, char **namep)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    return TRUE;
}
Bool
androidHWInit(ScrnInfoPtr pScrn, struct pci_device *pPci, char *device)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    androidHWPtr fPtr;
    androidHWGetRec(pScrn);
    fPtr = FBDEVHWPTR(pScrn);
    /* get current fb device settings */
    //if (-1 == ioctl(fPtr->fd, FBIOGET_FSCREENINFO, (void *) (&fPtr->fix))) { }
    //if (-1 == ioctl(fPtr->fd, FBIOGET_VSCREENINFO, (void *) (&fPtr->var))) {}
    /* we can use the current settings as "buildin mode" */
    android2xfree_timing(&fPtr->var, &fPtr->buildin);
    fPtr->buildin.name = (char *)"current";
    fPtr->buildin.next = &fPtr->buildin;
    fPtr->buildin.prev = &fPtr->buildin;
    fPtr->buildin.type |= M_T_BUILTIN;
    return TRUE;
}
char *
androidHWGetName(ScrnInfoPtr pScrn)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    androidHWPtr fPtr = FBDEVHWPTR(pScrn);
    return fPtr->fix.id;
}
int
androidHWGetDepth(ScrnInfoPtr pScrn, int *fbbpp)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    androidHWPtr fPtr = FBDEVHWPTR(pScrn);
    if (fbbpp)
        *fbbpp = fPtr->var.bits_per_pixel;
    if (fPtr->fix.visual == FB_VISUAL_TRUECOLOR ||
        fPtr->fix.visual == FB_VISUAL_DIRECTCOLOR)
        return fPtr->var.red.length + fPtr->var.green.length +
            fPtr->var.blue.length;
    else
        return fPtr->var.bits_per_pixel;
}
int
androidHWGetLineLength(ScrnInfoPtr pScrn)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    androidHWPtr fPtr = FBDEVHWPTR(pScrn);
    if (fPtr->fix.line_length)
        return fPtr->fix.line_length;
    else
        return fPtr->var.xres_virtual * fPtr->var.bits_per_pixel / 8;
}
int
androidHWGetType(ScrnInfoPtr pScrn)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    androidHWPtr fPtr = FBDEVHWPTR(pScrn);
    return fPtr->fix.type;
}
int
androidHWGetVidmem(ScrnInfoPtr pScrn)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    androidHWPtr fPtr = FBDEVHWPTR(pScrn);
    return fPtr->fix.smem_len;
}
static Bool
androidHWSetMode(ScrnInfoPtr pScrn, DisplayModePtr mode, Bool check)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    androidHWPtr fPtr = FBDEVHWPTR(pScrn);
    struct fb_var_screeninfo req_var = fPtr->var, set_var;
    xfree2android_fblayout(pScrn, &req_var);
    xfree2android_timing(mode, &req_var);
#if DEBUG
    print_xfree_mode("init", mode);
    print_android_mode("init", &req_var);
#endif
    set_var = req_var;
    if (check)
        set_var.activate = FB_ACTIVATE_TEST;
    //if (0 != ioctl(fPtr->fd, FBIOPUT_VSCREENINFO, (void *) (&set_var))) {}
    if (!android_modes_equal(&set_var, &req_var)) {
        if (!check)
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "FBIOPUT_VSCREENINFO succeeded but modified " "mode\n");
#if DEBUG
        print_android_mode("returned", &set_var);
#endif
        return FALSE;
    }
    if (!check)
        fPtr->var = set_var;
    return TRUE;
}
void
androidHWSetVideoModes(ScrnInfoPtr pScrn)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    char **modename;
    DisplayModePtr mode, this, last = pScrn->modes;
    if (NULL == pScrn->display->modes)
        return;
    pScrn->virtualX = pScrn->display->virtualX;
    pScrn->virtualY = pScrn->display->virtualY;
    for (modename = pScrn->display->modes; *modename != NULL; modename++) {
        for (mode = pScrn->monitor->Modes; mode != NULL; mode = mode->next) {
            if (0 == strcmp(mode->name, *modename)) {
                if (androidHWSetMode(pScrn, mode, TRUE))
                    break;
                xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                           "\tmode \"%s\" test failed\n", *modename);
            }
        }
        if (NULL == mode) {
            xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                       "\tmode \"%s\" not found\n", *modename);
            continue;
        }
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "\tmode \"%s\" ok\n", *modename);
        if (pScrn->virtualX < mode->HDisplay)
            pScrn->virtualX = mode->HDisplay;
        if (pScrn->virtualY < mode->VDisplay)
            pScrn->virtualY = mode->VDisplay;
        if (NULL == pScrn->modes) {
            this = pScrn->modes = xf86DuplicateMode(mode);
            this->next = this;
            this->prev = this;
        }
        else {
            this = xf86DuplicateMode(mode);
            this->next = pScrn->modes;
            this->prev = last;
            last->next = this;
            pScrn->modes->prev = this;
        }
        last = this;
    }
}
DisplayModePtr
androidHWGetBuildinMode(ScrnInfoPtr pScrn)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    androidHWPtr fPtr = FBDEVHWPTR(pScrn);
    return &fPtr->buildin;
}
void
androidHWUseBuildinMode(ScrnInfoPtr pScrn)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    androidHWPtr fPtr = FBDEVHWPTR(pScrn);
    pScrn->modes = &fPtr->buildin;
    pScrn->virtualX = pScrn->display->virtualX;
    pScrn->virtualY = pScrn->display->virtualY;
    if (pScrn->virtualX < fPtr->buildin.HDisplay)
        pScrn->virtualX = fPtr->buildin.HDisplay;
    if (pScrn->virtualY < fPtr->buildin.VDisplay)
        pScrn->virtualY = fPtr->buildin.VDisplay;
}
/* -------------------------------------------------------------------- */
static void
calculateFbmem_len(androidHWPtr fPtr)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    fPtr->fboff = (unsigned long) fPtr->fix.smem_start & ~PAGE_MASK;
    fPtr->fbmem_len = (fPtr->fboff + fPtr->fix.smem_len + ~PAGE_MASK) &
        PAGE_MASK;
}
void *
androidHWMapVidmem(ScrnInfoPtr pScrn)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    androidHWPtr fPtr = FBDEVHWPTR(pScrn);
    if (NULL == fPtr->fbmem) {
        calculateFbmem_len(fPtr);
        //fPtr->fbmem = mmap(NULL, fPtr->fbmem_len, PROT_READ | PROT_WRITE,
                           //MAP_SHARED, fPtr->fd, 0);
    }
    //pScrn->memPhysBase =
        //(unsigned long) fPtr->fix.smem_start & (unsigned long) (PAGE_MASK);
    //pScrn->fbOffset =
        //(unsigned long) fPtr->fix.smem_start & (unsigned long) (~PAGE_MASK);
    return fPtr->fbmem;
}
int
androidHWLinearOffset(ScrnInfoPtr pScrn)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    androidHWPtr fPtr = FBDEVHWPTR(pScrn);
    return fPtr->fboff;
}
Bool
androidHWUnmapVidmem(ScrnInfoPtr pScrn)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    //androidHWPtr fPtr = FBDEVHWPTR(pScrn);
    //if (NULL != fPtr->fbmem) {
        //if (-1 == munmap(fPtr->fbmem, fPtr->fbmem_len)) {}
        //fPtr->fbmem = NULL;
    //}
    return TRUE;
}
void *
androidHWMapMMIO(ScrnInfoPtr pScrn)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    unsigned int mmio_off;
    androidHWPtr fPtr = FBDEVHWPTR(pScrn);
    if (NULL == fPtr->mmio) {
        /* tell the kernel not to use accels to speed up console scrolling */
        fPtr->var.accel_flags = 0;
        //if (0 != ioctl(fPtr->fd, FBIOPUT_VSCREENINFO, (void *) (&fPtr->var))) {}
        mmio_off = (unsigned long) fPtr->fix.mmio_start & ~PAGE_MASK;
        fPtr->mmio_len = (mmio_off + fPtr->fix.mmio_len + ~PAGE_MASK) &
            PAGE_MASK;
        if (NULL == fPtr->fbmem)
            calculateFbmem_len(fPtr);
        //fPtr->mmio = mmap(NULL, fPtr->mmio_len, PROT_READ | PROT_WRITE, MAP_SHARED, fPtr->fd, fPtr->fbmem_len);
            //fPtr->mmio += mmio_off;
    }
    return fPtr->mmio;
}
Bool
androidHWUnmapMMIO(ScrnInfoPtr pScrn)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    //androidHWPtr fPtr = FBDEVHWPTR(pScrn);
    //if (NULL != fPtr->mmio) {
        //if (-1 == munmap((void *) ((unsigned long) fPtr->mmio & PAGE_MASK), fPtr->mmio_len)) {}
        //fPtr->mmio = NULL;
        ///* FIXME: restore var.accel_flags [geert] */
    //}
    return TRUE;
}
/* -------------------------------------------------------------------- */
Bool
androidHWModeInit(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    androidHWPtr fPtr = FBDEVHWPTR(pScrn);
    pScrn->vtSema = TRUE;
    /* set */
    if (!androidHWSetMode(pScrn, mode, FALSE))
        return FALSE;
    /* read back */
    //if (0 != ioctl(fPtr->fd, FBIOGET_FSCREENINFO, (void *) (&fPtr->fix))) {}
    //if (0 != ioctl(fPtr->fd, FBIOGET_VSCREENINFO, (void *) (&fPtr->var))) {}
    if (pScrn->defaultVisual == TrueColor ||
        pScrn->defaultVisual == DirectColor) {
        /* XXX: This is a hack, but it should be a NOP for all the setups that
         * worked before and actually seems to fix some others...
         */
        pScrn->offset.red = fPtr->var.red.offset;
        pScrn->offset.green = fPtr->var.green.offset;
        pScrn->offset.blue = fPtr->var.blue.offset;
        pScrn->mask.red =
            ((1 << fPtr->var.red.length) - 1) << fPtr->var.red.offset;
        pScrn->mask.green =
            ((1 << fPtr->var.green.length) - 1) << fPtr->var.green.offset;
        pScrn->mask.blue =
            ((1 << fPtr->var.blue.length) - 1) << fPtr->var.blue.offset;
    }
    return TRUE;
}
/* -------------------------------------------------------------------- */
/* video mode save/restore                                              */
void
androidHWSave(ScrnInfoPtr pScrn)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    //androidHWPtr fPtr = FBDEVHWPTR(pScrn);
    //if (0 != ioctl(fPtr->fd, FBIOGET_VSCREENINFO, (void *) (&fPtr->saved_var))) {}
}
void
androidHWRestore(ScrnInfoPtr pScrn)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    //androidHWPtr fPtr = FBDEVHWPTR(pScrn);
    //if (0 != ioctl(fPtr->fd, FBIOPUT_VSCREENINFO, (void *) (&fPtr->saved_var))) {}
}
/* -------------------------------------------------------------------- */
/* callback for xf86HandleColormaps                                     */
void
androidHWLoadPalette(ScrnInfoPtr pScrn, int numColors, int *indices,
                   LOCO * colors, VisualPtr pVisual)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    //androidHWPtr fPtr = FBDEVHWPTR(pScrn);
    struct fb_cmap cmap;
    unsigned short red, green, blue;
    int i;
    cmap.len = 1;
    cmap.red = &red;
    cmap.green = &green;
    cmap.blue = &blue;
    cmap.transp = NULL;
    for (i = 0; i < numColors; i++) {
        cmap.start = indices[i];
        red = (colors[indices[i]].red << 8) | colors[indices[i]].red;
        green = (colors[indices[i]].green << 8) | colors[indices[i]].green;
        blue = (colors[indices[i]].blue << 8) | colors[indices[i]].blue;
        //if (-1 == ioctl(fPtr->fd, FBIOPUTCMAP, (void *) &cmap)) {}
    }
}
/* -------------------------------------------------------------------- */
/* these can be hooked directly into ScrnInfoRec                        */
ModeStatus
androidHWValidMode(ScrnInfoPtr pScrn, DisplayModePtr mode, Bool verbose, int flags)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    if (!androidHWSetMode(pScrn, mode, TRUE))
        return MODE_BAD;
    return MODE_OK;
}
Bool
androidHWSwitchMode(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    if (!androidHWSetMode(pScrn, mode, FALSE))
        return FALSE;
    return TRUE;
}
void
androidHWAdjustFrame(ScrnInfoPtr pScrn, int x, int y)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    androidHWPtr fPtr = FBDEVHWPTR(pScrn);
    if (x < 0 || x + fPtr->var.xres > fPtr->var.xres_virtual ||
        y < 0 || y + fPtr->var.yres > fPtr->var.yres_virtual)
        return;
    fPtr->var.xoffset = x;
    fPtr->var.yoffset = y;
    //if (-1 == ioctl(fPtr->fd, FBIOPAN_DISPLAY, (void *) &fPtr->var)) {}
}
Bool
androidHWEnterVT(ScrnInfoPtr pScrn)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    if (!androidHWModeInit(pScrn, pScrn->currentMode))
        return FALSE;
    androidHWAdjustFrame(pScrn, pScrn->frameX0, pScrn->frameY0);
    return TRUE;
}
void
androidHWLeaveVT(ScrnInfoPtr pScrn)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    androidHWRestore(pScrn);
}
void
androidHWDPMSSet(ScrnInfoPtr pScrn, int mode, int flags)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    //androidHWPtr fPtr = FBDEVHWPTR(pScrn);
    unsigned long fbmode;
    if (!pScrn->vtSema)
        return;
    switch (mode) {
    case DPMSModeOn:
        fbmode = 0;
        break;
    case DPMSModeStandby:
        fbmode = 2;
        break;
    case DPMSModeSuspend:
        fbmode = 3;
        break;
    case DPMSModeOff:
        fbmode = 4;
        break;
    default:
        return;
    }
    //if (-1 == ioctl(fPtr->fd, FBIOBLANK, (void *) fbmode)) {}
}
Bool
androidHWSaveScreen(ScreenPtr pScreen, int mode)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    //androidHWPtr fPtr = FBDEVHWPTR(pScrn);
    unsigned long unblank;
    if (!pScrn->vtSema)
        return TRUE;
    unblank = xf86IsUnblank(mode);
    //if (-1 == ioctl(fPtr->fd, FBIOBLANK, (void *) (1 - unblank))) {}
    return TRUE;
}
xf86SwitchModeProc *
androidHWSwitchModeWeak(void)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    return androidHWSwitchMode;
}
xf86AdjustFrameProc *
androidHWAdjustFrameWeak(void)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    return androidHWAdjustFrame;
}
xf86EnterVTProc *
androidHWEnterVTWeak(void)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    return androidHWEnterVT;
}
xf86LeaveVTProc *
androidHWLeaveVTWeak(void)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    return androidHWLeaveVT;
}
xf86ValidModeProc *
androidHWValidModeWeak(void)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    return androidHWValidMode;
}
xf86DPMSSetProc *
androidHWDPMSSetWeak(void)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    return androidHWDPMSSet;
}
xf86LoadPaletteProc *
androidHWLoadPaletteWeak(void)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    return androidHWLoadPalette;
}
SaveScreenProcPtr
androidHWSaveScreenWeak(void)
{
DOUT("[%s:%d]\n", __FUNCTION__, __LINE__);
    return androidHWSaveScreen;
}
