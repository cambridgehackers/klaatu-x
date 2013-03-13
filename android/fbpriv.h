/*
 * copyed from from linux kernel 2.2.4
 * removed internal stuff (#ifdef __KERNEL__)
 */

#ifdef HAVE_XORG_CONFIG_H
#include <xorg-config.h>
#endif

#ifndef _LINUX_FB_H
#define _LINUX_FB_H

#include <asm/types.h>

/* Definitions of frame buffers						*/

#define FB_MAJOR	29

#define FB_MODES_SHIFT		5       /* 32 modes per framebuffer */
#define FB_NUM_MINORS		256     /* 256 Minors               */
#define FB_MAX			(FB_NUM_MINORS / (1 << FB_MODES_SHIFT))
#define GET_FB_IDX(node)	(MINOR(node) >> FB_MODES_SHIFT)

#define FB_VISUAL_MONO01		0       /* Monochr. 1=Black 0=White */
#define FB_VISUAL_MONO10		1       /* Monochr. 1=White 0=Black */
#define FB_VISUAL_TRUECOLOR		2       /* True color   */
#define FB_VISUAL_PSEUDOCOLOR		3       /* Pseudo color (like atari) */
#define FB_VISUAL_DIRECTCOLOR		4       /* Direct color */
#define FB_VISUAL_STATIC_PSEUDOCOLOR	5       /* Pseudo color readonly */

struct fb_fix_screeninfo {
    char id[16];                /* identification string eg "TT Builtin" */
    char *smem_start;           /* Start of frame buffer mem */
    /* (physical address) */
    __u32 smem_len;             /* Length of frame buffer mem */
    __u32 type;                 /* see FB_TYPE_*                */
    __u32 type_aux;             /* Interleave for interleaved Planes */
    __u32 visual;               /* see FB_VISUAL_*              */
    __u16 xpanstep;             /* zero if no hardware panning  */
    __u16 ypanstep;             /* zero if no hardware panning  */
    __u16 ywrapstep;            /* zero if no hardware ywrap    */
    __u32 line_length;          /* length of a line in bytes    */
    char *mmio_start;           /* Start of Memory Mapped I/O   */
    /* (physical address) */
    __u32 mmio_len;             /* Length of Memory Mapped I/O  */
    __u32 accel;                /* Type of acceleration available */
    __u16 reserved[3];          /* Reserved for future compatibility */
};

/* Interpretation of offset for color fields: All offsets are from the right,
 * inside a "pixel" value, which is exactly 'bits_per_pixel' wide (means: you
 * can use the offset as right argument to <<). A pixel afterwards is a bit
 * stream and is written to video memory as that unmodified. This implies
 * big-endian byte order if bits_per_pixel is greater than 8.
 */
struct fb_bitfield {
    __u32 offset;               /* beginning of bitfield        */
    __u32 length;               /* length of bitfield           */
    __u32 msb_right;            /* != 0 : Most significant bit is */
    /* right */
};

#define FB_ACTIVATE_TEST	2       /* don't set, round up impossible */

#define FB_SYNC_HOR_HIGH_ACT	1       /* horizontal sync high active  */
#define FB_SYNC_VERT_HIGH_ACT	2       /* vertical sync high active    */
#define FB_SYNC_EXT		4       /* external sync                */
#define FB_SYNC_COMP_HIGH_ACT	8       /* composite sync high active   */
#define FB_SYNC_BROADCAST	16      /* broadcast video timings      */
                                        /* vtotal = 144d/288n/576i => PAL  */
                                        /* vtotal = 121d/242n/484i => NTSC */
#define FB_SYNC_ON_GREEN	32      /* sync on green */

#define FB_VMODE_NONINTERLACED  0       /* non interlaced */
#define FB_VMODE_INTERLACED	1       /* interlaced   */
#define FB_VMODE_DOUBLE		2       /* double scan */
#define FB_VMODE_MASK		255

#define FB_VMODE_YWRAP		256     /* ywrap instead of panning     */
#define FB_VMODE_SMOOTH_XPAN	512     /* smooth xpan possible (internally used) */
#define FB_VMODE_CONUPDATE	512     /* don't update x/yoffset       */

struct fb_var_screeninfo {
    __u32 xres;                 /* visible resolution           */
    __u32 yres;
    __u32 xres_virtual;         /* virtual resolution           */
    __u32 yres_virtual;
    __u32 xoffset;              /* offset from virtual to visible */
    __u32 yoffset;              /* resolution                   */

    __u32 bits_per_pixel;       /* guess what                   */
    __u32 grayscale;            /* != 0 Graylevels instead of colors */

    struct fb_bitfield red;     /* bitfield in fb mem if true color, */
    struct fb_bitfield green;   /* else only length is significant */
    struct fb_bitfield blue;
    struct fb_bitfield transp;  /* transparency                 */

    __u32 nonstd;               /* != 0 Non standard pixel format */

    __u32 activate;             /* see FB_ACTIVATE_*            */

    __u32 height;               /* height of picture in mm    */
    __u32 width;                /* width of picture in mm     */

    __u32 accel_flags;          /* acceleration flags (hints)   */

    /* Timing: All values in pixclocks, except pixclock (of course) */
    __u32 pixclock;             /* pixel clock in ps (pico seconds) */
    __u32 left_margin;          /* time from sync to picture    */
    __u32 right_margin;         /* time from picture to sync    */
    __u32 upper_margin;         /* time from sync to picture    */
    __u32 lower_margin;
    __u32 hsync_len;            /* length of horizontal sync    */
    __u32 vsync_len;            /* length of vertical sync      */
    __u32 sync;                 /* see FB_SYNC_*                */
    __u32 vmode;                /* see FB_VMODE_*               */
    __u32 reserved[6];          /* Reserved for future compatibility */
};

struct fb_cmap {
    __u32 start;                /* First entry  */
    __u32 len;                  /* Number of entries */
    __u16 *red;                 /* Red values   */
    __u16 *green;
    __u16 *blue;
    __u16 *transp;              /* transparency, can be NULL */
};

struct fb_con2fbmap {
    __u32 console;
    __u32 framebuffer;
};

struct fb_monspecs {
    __u32 hfmin;                /* hfreq lower limit (Hz) */
    __u32 hfmax;                /* hfreq upper limit (Hz) */
    __u16 vfmin;                /* vfreq lower limit (Hz) */
    __u16 vfmax;                /* vfreq upper limit (Hz) */
    unsigned dpms:1;            /* supports DPMS */
};

#if 1

#define FBCMD_GET_CURRENTPAR	0xDEAD0005
#define FBCMD_SET_CURRENTPAR	0xDEAD8005

#endif


#endif                          /* _LINUX_FB_H */
