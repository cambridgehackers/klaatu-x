
#ifndef _FBDEVHW_H_
#define _FBDEVHW_H_

#include "xf86str.h"
#include "colormapst.h"

#define FBDEVHW_PACKED_PIXELS		0       /* Packed Pixels        */
#define FBDEVHW_PLANES			1       /* Non interleaved planes */
#define FBDEVHW_INTERLEAVED_PLANES	2       /* Interleaved planes   */
#define FBDEVHW_TEXT			3       /* Text/attributes      */
#define FBDEVHW_VGA_PLANES		4       /* EGA/VGA planes       */

extern _X_EXPORT Bool androidHWGetRec(ScrnInfoPtr pScrn);
extern _X_EXPORT void androidHWFreeRec(ScrnInfoPtr pScrn);

extern _X_EXPORT int androidHWGetFD(ScrnInfoPtr pScrn);

extern _X_EXPORT Bool androidHWProbe(struct pci_device *pPci, char *device,
                                   char **namep);
extern _X_EXPORT Bool androidHWInit(ScrnInfoPtr pScrn, struct pci_device *pPci,
                                  char *device);

extern _X_EXPORT char *androidHWGetName(ScrnInfoPtr pScrn);
extern _X_EXPORT int androidHWGetDepth(ScrnInfoPtr pScrn, int *fbbpp);
extern _X_EXPORT int androidHWGetLineLength(ScrnInfoPtr pScrn);
extern _X_EXPORT int androidHWGetType(ScrnInfoPtr pScrn);
extern _X_EXPORT int androidHWGetVidmem(ScrnInfoPtr pScrn);

extern _X_EXPORT void *androidHWMapVidmem(ScrnInfoPtr pScrn);
extern _X_EXPORT int androidHWLinearOffset(ScrnInfoPtr pScrn);
extern _X_EXPORT Bool androidHWUnmapVidmem(ScrnInfoPtr pScrn);
extern _X_EXPORT void *androidHWMapMMIO(ScrnInfoPtr pScrn);
extern _X_EXPORT Bool androidHWUnmapMMIO(ScrnInfoPtr pScrn);

extern _X_EXPORT void androidHWSetVideoModes(ScrnInfoPtr pScrn);
extern _X_EXPORT DisplayModePtr androidHWGetBuildinMode(ScrnInfoPtr pScrn);
extern _X_EXPORT void androidHWUseBuildinMode(ScrnInfoPtr pScrn);
extern _X_EXPORT Bool androidHWModeInit(ScrnInfoPtr pScrn, DisplayModePtr mode);
extern _X_EXPORT void androidHWSave(ScrnInfoPtr pScrn);
extern _X_EXPORT void androidHWRestore(ScrnInfoPtr pScrn);

extern _X_EXPORT void androidHWLoadPalette(ScrnInfoPtr pScrn, int numColors,
                                         int *indices, LOCO * colors,
                                         VisualPtr pVisual);

extern _X_EXPORT ModeStatus androidHWValidMode(ScrnInfoPtr pScrn, DisplayModePtr mode,
                                             Bool verbose, int flags);
extern _X_EXPORT Bool androidHWSwitchMode(ScrnInfoPtr pScrn, DisplayModePtr mode);
extern _X_EXPORT void androidHWAdjustFrame(ScrnInfoPtr pScrn, int x, int y);
extern _X_EXPORT Bool androidHWEnterVT(ScrnInfoPtr pScrn);
extern _X_EXPORT void androidHWLeaveVT(ScrnInfoPtr pScrn);
extern _X_EXPORT void androidHWDPMSSet(ScrnInfoPtr pScrn, int mode, int flags);

extern _X_EXPORT Bool androidHWSaveScreen(ScreenPtr pScreen, int mode);

extern _X_EXPORT xf86SwitchModeProc *androidHWSwitchModeWeak(void);
extern _X_EXPORT xf86AdjustFrameProc *androidHWAdjustFrameWeak(void);
extern _X_EXPORT xf86EnterVTProc *androidHWEnterVTWeak(void);
extern _X_EXPORT xf86LeaveVTProc *androidHWLeaveVTWeak(void);
extern _X_EXPORT xf86ValidModeProc *androidHWValidModeWeak(void);
extern _X_EXPORT xf86DPMSSetProc *androidHWDPMSSetWeak(void);
extern _X_EXPORT xf86LoadPaletteProc *androidHWLoadPaletteWeak(void);
extern _X_EXPORT SaveScreenProcPtr androidHWSaveScreenWeak(void);

#endif
