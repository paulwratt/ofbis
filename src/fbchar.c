#include <stdio.h>

#include "libfb.h"
#include "fballoc.h"
#include "fbchar.h"

#define	MIN(a,b)	( (a) < (b) ? (a) : (b) )
#define MAX(a,b)	( (a) > (b) ? (a) : (b) )

#define setonei2(data,bpl,c) base[bpl] &= ~((data) << skew); \
			     if((c>>bpl)&1) base[bpl] |=  ((data) << skew);

#define settwoi2(data,bpl,off,c) base[bpl]     &= ~((data) >> (-skew)); \
				 base[bpl+off] &= ~((data) << (16+skew)); \
				 if((c>>bpl)&1) { base[bpl]     |=  ((data) >> (-skew)); \
						  base[bpl+off] |=  ((data) << (16+skew)); }

void
genpc( FB *f, unsigned short x, unsigned short y, unsigned long fgcol, unsigned long bgcol, unsigned char ch )
{
  int ox, oy;
  register unsigned char *chardata = &f->font->data [f->font->height * ch];
  register unsigned char data;

  for (oy = 0; oy < f->font->height; oy++) {
    data = *chardata++;

    for (ox = 0; ox < f->font->width; ox++) {
      if (data & 0x80) {
	(*f->putpixel)(f, x+ox, y+oy, fgcol);
      }

      data <<= 1;
    }

  }
}

void
i2pc1( FB *f, unsigned short x, unsigned short y, unsigned long fgcol, unsigned long bgcol, unsigned char ch )
{
	int skew = f->font->width-(x%16);
	int row;
	int linewords = (f->vinf.xres_virtual>>4)*1;
	register unsigned short *base = f->sbuf+(((y*f->vinf.xres_virtual)+x)>>4)*1;
	register unsigned char *chardata = &f->font->data[f->font->height*ch];

	if (skew >= 0) { /* only one short to put */
		for (row = 0 ; row < f->font->height ; row++,chardata++,base+=linewords) {
			setonei2(*chardata, 0, fgcol);
		}
	} else { /* two shorts are needed */
		for (row = 0 ; row < f->font->height ; row++,chardata++,base+=linewords) {
			settwoi2(*chardata, 0, 1, fgcol);
		}
	}
}

void
i2pc2( FB *f, unsigned short x, unsigned short y, unsigned long fgcol, unsigned long bgcol, unsigned char ch )
{
	int skew = f->font->width-(x%16);
	int row;
	int linewords = (f->vinf.xres_virtual>>4)*2;
	register unsigned short *base = f->sbuf+(((y*f->vinf.xres_virtual)+x)>>4)*2;
	register unsigned char *chardata = &f->font->data[f->font->height*ch];

	if (skew >= 0) { /* only one short to put */
		for (row = 0 ; row < f->font->height ; row++,chardata++,base+=linewords) {
			setonei2(*chardata, 0, fgcol);
			setonei2(*chardata, 1, fgcol);
		}
	} else { /* two shorts are needed */
		for (row = 0 ; row < f->font->height ; row++,chardata++,base+=linewords) {
			settwoi2(*chardata, 0, 2, fgcol);
			settwoi2(*chardata, 1, 2, fgcol);
		}
	}
}

void
i2pc4( FB *f, unsigned short x, unsigned short y, unsigned long fgcol, unsigned long bgcol, unsigned char ch )
{
	int skew = f->font->width-(x%16);
	int row;
	int linewords = (f->vinf.xres_virtual>>4)*4;
	register unsigned short *base = f->sbuf+(((y*f->vinf.xres_virtual)+x)>>4)*4;
	register unsigned char *chardata = &f->font->data[f->font->height*ch];

	if (skew >= 0) { /* only one short to put */
		for (row = 0 ; row < f->font->height ; row++,chardata++,base+=linewords) {
			setonei2(*chardata, 0, fgcol);
			setonei2(*chardata, 1, fgcol);
			setonei2(*chardata, 2, fgcol);
			setonei2(*chardata, 3, fgcol);
		}
	} else { /* two shorts are needed */
		for (row = 0 ; row < f->font->height ; row++,chardata++,base+=linewords) {
			settwoi2(*chardata, 0, 4, fgcol);
			settwoi2(*chardata, 1, 4, fgcol);
			settwoi2(*chardata, 2, 4, fgcol);
			settwoi2(*chardata, 3, 4, fgcol);
		}
	}
}

void
i2pc8( FB *f, unsigned short x, unsigned short y, unsigned long fgcol, unsigned long bgcol, unsigned char ch )
{
	int skew = f->font->width-(x%16);
	int row;
	int linewords = (f->vinf.xres_virtual>>4)*8;
	register unsigned short *base = f->sbuf+(((y*f->vinf.xres_virtual)+x)>>4)*8;
	register unsigned char *chardata = &f->font->data[f->font->height*ch];

	if (skew >= 0) { /* only one short to put */
		for (row = 0 ; row < f->font->height ; row++,chardata++,base+=linewords) {
			setonei2(*chardata, 0, fgcol);
			setonei2(*chardata, 1, fgcol);
			setonei2(*chardata, 2, fgcol);
			setonei2(*chardata, 3, fgcol);
			setonei2(*chardata, 4, fgcol);
			setonei2(*chardata, 5, fgcol);
			setonei2(*chardata, 6, fgcol);
			setonei2(*chardata, 7, fgcol);
		}
	} else { /* two shorts are needed */
		for (row = 0 ; row < f->font->height ; row++,chardata++,base+=linewords) {
			settwoi2(*chardata, 0, 8, fgcol);
			settwoi2(*chardata, 1, 8, fgcol);
			settwoi2(*chardata, 2, 8, fgcol);
			settwoi2(*chardata, 3, 8, fgcol);
			settwoi2(*chardata, 4, 8, fgcol);
			settwoi2(*chardata, 5, 8, fgcol);
			settwoi2(*chardata, 6, 8, fgcol);
			settwoi2(*chardata, 7, 8, fgcol);
		}
	}
}

/*
** Description
** Put a character to a 16-bit truecolour fb
**
*/
void
pctc(FB *f,
     unsigned short x,
     unsigned short y,
     unsigned long fgcol,
     unsigned long bgcol,
     unsigned char ch )
{
  int row;
  register unsigned short *base = ((unsigned short *)f->sbuf) + 
				   (x + y * f->vinf.xres_virtual);
  register unsigned char *chardata = &f->font->data[f->font->height * ch];
  unsigned char *cp= (char *)&fgcol;
  unsigned short tc_col;
  
  tc_col = (( *cp++ << 8 ) & 0xF800 ) |
	   (( *++cp << 3 ) & 0x07E0 ) |
	   (( *++cp >> 3 ) & 0x001F );

  for (row = 0; row < f->font->height; row++, chardata++) {
    register unsigned char data = *chardata;
    int p;

    for (p = 0; p < f->font->width; p++) {
      if (data & 0x80) {
	*base = tc_col;
      }

      base++;
      data <<= 1;
    }

    base += f->vinf.xres_virtual - f->font->width;
  }
}


/*
** Description
** Put a character to an 8-bit static pseudocolour fb
**
*/
void
pcsp8 (FB *f,
       unsigned short x,
       unsigned short y,
       unsigned long fgcol,
       unsigned long bgcol,
       unsigned char ch )
{
  int row;
  register unsigned char *base = ((unsigned char *)f->sbuf) + 
				  (x + y * f->vinf.xres_virtual);
  register unsigned char *chardata = &f->font->data[f->font->height * ch];

  for (row = 0; row < f->font->height; row++, chardata++) {
    register unsigned char data = *chardata;
    int p;

    for (p = 0; p < f->font->width; p++) {
      if (data & 0x80) {
	*base = fgcol;
      }

      base++;
      data <<= 1;
    }

    base += f->vinf.xres_virtual - f->font->width;
  }
}

/* Yet to be optimized */
void
plpc1( FB *f, unsigned short x, unsigned short y, unsigned long fgcol, unsigned long bgcol, unsigned char ch )
{
}

void
plpc2( FB *f, unsigned short x, unsigned short y, unsigned long fgcol, unsigned long bgcol, unsigned char ch )
{
}

void
plpc4( FB *f, unsigned short x, unsigned short y, unsigned long fgcol, unsigned long bgcol, unsigned char ch )
{
}

void
plpc8( FB *f, unsigned short x, unsigned short y, unsigned long fgcol, unsigned long bgcol, unsigned char ch )
{
}


void
FBfontopen( FB *f )
{
	f->font = (FBFONT *)FBalloc(sizeof(FBFONT));

	/* Default font 8x8 */
	f->font->data = fontdata_8x8;
	f->font->width = fontwidth_8x8;
	f->font->height = fontheight_8x8;
}

void
FBfontclose( FB *f )
{
	FBfree(f->font);
}


void
FBsetfont( FB *f, FBFONT *newfont )
{
	f->font->data = newfont->data;
	f->font->width = newfont->width;
	f->font->height = newfont->height;
}
