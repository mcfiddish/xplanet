/****************************************************************************
    gif.c - read and write gif images using libgif/libungif.
    Distributed with Xplanet.  
    Copyright (C) 2002 Hari Nair <hari@alumni.caltech.edu>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <gif_lib.h>

/*
  Based on the gif2rgb and rgb2gif codes in the libgif distribution. 
*/

int
read_gif(const char *filename, int *width, int *height, unsigned char **rgb)
{
    int	BackGround = 0;
    /* The way Interlaced image should be read - offsets and
       jumps... */
    int InterlacedOffset[] = { 0, 4, 2, 1 }; 
    int InterlacedJumps[] = { 8, 8, 4, 2 };
    ColorMapObject *ColorMap;

    int	i, j, Size, Row, Col, Width, Height, ExtCode, Count;
    GifRecordType RecordType;
    GifByteType *Extension;
    GifRowType *ScreenBuffer;

    GifRowType GifRow;
    GifColorType *ColorMapEntry;
    unsigned char *BufferP;

    GifFileType *GifFile;
#if GIFLIB_MAJOR >= 5
    GifFile = DGifOpenFileName(filename, NULL);
#else
    GifFile = DGifOpenFileName(filename);
#endif
    if (GifFile == NULL) {
	fprintf(stderr, "Can't open GIF file %s\n", filename);
	return(0);
    }
    /* (Following comment was probably written on a 286 running under DOS.)  */
    /* Allocate the screen as vector of column of rows. We can't allocate the*/
    /* whole screen at once, as this broken minded CPU can allocate up to    */
    /* 64k at a time and our image can be bigger than that:		     */
    /* Note this screen is device independent - it's the screen defined by   */
    /* the GIF file parameters.						     */
    if ((ScreenBuffer = (GifRowType *)
	 malloc(GifFile->SHeight * sizeof(GifRowType))) == NULL)
    {
	fprintf(stderr, "Failed to allocate memory required, aborted.");
	return(0);
    }

    Size = GifFile->SWidth * sizeof(GifPixelType);/* Size in bytes one row.*/
    if ((ScreenBuffer[0] = (GifRowType) malloc(Size)) == NULL) /* First row. */
    {
	fprintf(stderr, "Failed to allocate memory required, aborted.");
	return(0);
    }

    for (i = 0; i < GifFile->SWidth; i++)  /* Set its color to BackGround. */
	ScreenBuffer[0][i] = GifFile->SBackGroundColor;
    for (i = 1; i < GifFile->SHeight; i++) {
	/* Allocate the other rows, and set their color to background too: */
	if ((ScreenBuffer[i] = (GifRowType) malloc(Size)) == NULL)
	{
	    fprintf(stderr, "Failed to allocate memory required, aborted.\n");
	    return(0);
	}

	memcpy(ScreenBuffer[i], ScreenBuffer[0], Size);
    }

    /* Scan the content of the GIF file and load the image(s) in: */
    do {
	if (DGifGetRecordType(GifFile, &RecordType) == GIF_ERROR) {
	    fprintf(stderr, "Can't read GIF file\n");
	    return(0);
	}
	switch (RecordType) {
	case IMAGE_DESC_RECORD_TYPE:
	    if (DGifGetImageDesc(GifFile) == GIF_ERROR) {
		return(0);
	    }
	    Row = GifFile->Image.Top; /* Image Position relative to Screen. */
	    Col = GifFile->Image.Left;
	    Width = GifFile->Image.Width;
	    Height = GifFile->Image.Height;
	    if (GifFile->Image.Left + GifFile->Image.Width > GifFile->SWidth ||
		GifFile->Image.Top + GifFile->Image.Height > GifFile->SHeight) {
		return(0);
	    }
	    if (GifFile->Image.Interlace) {
		/* Need to perform 4 passes on the images: */
		for (Count = i = 0; i < 4; i++)
		    for (j = Row + InterlacedOffset[i]; j < Row + Height;
			 j += InterlacedJumps[i]) {
			if (DGifGetLine(GifFile, &ScreenBuffer[j][Col],
					Width) == GIF_ERROR) {
			    return(0);
			}
		    }
	    }
	    else {
		for (i = 0; i < Height; i++) {
		    if (DGifGetLine(GifFile, &ScreenBuffer[Row++][Col],
				    Width) == GIF_ERROR) {
			return(0);
		    }
		}
	    }
	    break;
	case EXTENSION_RECORD_TYPE:
	    /* Skip any extension blocks in file: */
	    if (DGifGetExtension(GifFile, &ExtCode, &Extension) == GIF_ERROR) {
		return(0);
	    }
	    while (Extension != NULL) {
		if (DGifGetExtensionNext(GifFile, &Extension) == GIF_ERROR) {
		    return(0);
		}
	    }
	    break;
	case TERMINATE_RECORD_TYPE:
	    break;
	default:		    /* Should be traps by DGifGetRecordType. */
	    break;
	}
    } while (RecordType != TERMINATE_RECORD_TYPE);

    /* Lets dump it - set the global variables required and do it: */
    BackGround = GifFile->SBackGroundColor;
    ColorMap = (GifFile->Image.ColorMap
		? GifFile->Image.ColorMap
		: GifFile->SColorMap);
    if (ColorMap == NULL) {
	fprintf(stderr, "Gif Image does not have a colormap\n");
	return(0);
    }

    *width = GifFile->SWidth;
    *height = GifFile->SHeight;

    rgb[0] = malloc(3 * *width * *height);
    if (rgb[0] == NULL)
    {
	fprintf(stderr, "Can't allocate memory for GIF file.\n");
	return(0);
    }

    BufferP = rgb[0];
    
    for (i = 0; i < *height; i++) {
	GifRow = ScreenBuffer[i];
	for (j = 0; j < *width; j++) {
	    ColorMapEntry = &ColorMap->Colors[GifRow[j]];
	    *BufferP++ = ColorMapEntry->Red;
	    *BufferP++ = ColorMapEntry->Green;
	    *BufferP++ = ColorMapEntry->Blue;
	}
    }
    
#if GIFLIB_MAJOR >= 5
    if (DGifCloseFile(GifFile, NULL) == GIF_ERROR) {
#else
    if (DGifCloseFile(GifFile) == GIF_ERROR) {
#endif
	return(0);
    }

    return(1);
}

/* Make some variables global, so we could access them faster: */
static int
    ExpNumOfColors = 8,
    ColorMapSize = 256;

/******************************************************************************
 * Code to quantize high resolution image into lower one. You may want to
 * peek into the following article this code is based on:
 * "Color Image Quantization for frame buffer Display", by Paul Heckbert
 * SIGGRAPH 1982 page 297-307.
 ******************************************************************************
 * History:
 * 5 Jan 90 - Version 1.0 by Gershon Elber.
 *****************************************************************************/

#define ABS(x)    ((x) > 0 ? (x) : (-(x)))

#define COLOR_ARRAY_SIZE 32768
#define BITS_PER_PRIM_COLOR 5
#define MAX_PRIM_COLOR      0x1f

static int SortRGBAxis;

typedef struct QuantizedColorType {
    GifByteType RGB[3];
    GifByteType NewColorIndex;
    long Count;
    struct QuantizedColorType *Pnext;
} QuantizedColorType;

typedef struct NewColorMapType {
    GifByteType RGBMin[3], RGBWidth[3];
    unsigned int NumEntries; /* # of QuantizedColorType in linked list below */
    unsigned long Count; /* Total number of pixels in all the entries */
    QuantizedColorType *QuantizedColors;
} NewColorMapType;

static int SubdivColorMap(NewColorMapType * NewColorSubdiv,
                          unsigned int ColorMapSize,
                          unsigned int *NewColorMapSize);
static int SortCmpRtn(const void *Entry1, const void *Entry2);

/******************************************************************************
 * Quantize high resolution image into lower one. Input image consists of a
 * 2D array for each of the RGB colors with size Width by Height. There is no
 * Color map for the input. Output is a quantized image with 2D array of
 * indexes into the output color map.
 *   Note input image can be 24 bits at the most (8 for red/green/blue) and
 * the output has 256 colors at the most (256 entries in the color map.).
 * ColorMapSize specifies size of color map up to 256 and will be updated to
 * real size before returning.
 *   Also non of the parameter are allocated by this routine.
 *   This function returns GIF_OK if succesfull, GIF_ERROR otherwise.
 ******************************************************************************/
int
QuantizeBuffer(unsigned int Width,
               unsigned int Height,
               int *ColorMapSize,
               GifByteType * RedInput,
               GifByteType * GreenInput,
               GifByteType * BlueInput,
               GifByteType * OutputBuffer,
               GifColorType * OutputColorMap) {

    unsigned int Index, NumOfEntries;
    int i, j, MaxRGBError[3];
    unsigned int NewColorMapSize;
    long Red, Green, Blue;
    NewColorMapType NewColorSubdiv[256];
    QuantizedColorType *ColorArrayEntries, *QuantizedColor;

    ColorArrayEntries = (QuantizedColorType *)malloc(
                           sizeof(QuantizedColorType) * COLOR_ARRAY_SIZE);
    if (ColorArrayEntries == NULL) {
        return GIF_ERROR;
    }

    for (i = 0; i < COLOR_ARRAY_SIZE; i++) {
        ColorArrayEntries[i].RGB[0] = i >> (2 * BITS_PER_PRIM_COLOR);
        ColorArrayEntries[i].RGB[1] = (i >> BITS_PER_PRIM_COLOR) &
           MAX_PRIM_COLOR;
        ColorArrayEntries[i].RGB[2] = i & MAX_PRIM_COLOR;
        ColorArrayEntries[i].Count = 0;
    }

    /* Sample the colors and their distribution: */
    for (i = 0; i < (int)(Width * Height); i++) {
        Index = ((RedInput[i] >> (8 - BITS_PER_PRIM_COLOR)) <<
                  (2 * BITS_PER_PRIM_COLOR)) +
                ((GreenInput[i] >> (8 - BITS_PER_PRIM_COLOR)) <<
                  BITS_PER_PRIM_COLOR) +
                (BlueInput[i] >> (8 - BITS_PER_PRIM_COLOR));
        ColorArrayEntries[Index].Count++;
    }

    /* Put all the colors in the first entry of the color map, and call the
     * recursive subdivision process.  */
    for (i = 0; i < 256; i++) {
        NewColorSubdiv[i].QuantizedColors = NULL;
        NewColorSubdiv[i].Count = NewColorSubdiv[i].NumEntries = 0;
        for (j = 0; j < 3; j++) {
            NewColorSubdiv[i].RGBMin[j] = 0;
            NewColorSubdiv[i].RGBWidth[j] = 255;
        }
    }

    /* Find the non empty entries in the color table and chain them: */
    for (i = 0; i < COLOR_ARRAY_SIZE; i++)
        if (ColorArrayEntries[i].Count > 0)
            break;
    QuantizedColor = NewColorSubdiv[0].QuantizedColors = &ColorArrayEntries[i];
    NumOfEntries = 1;
    while (++i < COLOR_ARRAY_SIZE)
        if (ColorArrayEntries[i].Count > 0) {
            QuantizedColor->Pnext = &ColorArrayEntries[i];
            QuantizedColor = &ColorArrayEntries[i];
            NumOfEntries++;
        }
    QuantizedColor->Pnext = NULL;

    NewColorSubdiv[0].NumEntries = NumOfEntries; /* Different sampled colors */
    NewColorSubdiv[0].Count = ((long)Width) * Height; /* Pixels */
    NewColorMapSize = 1;
    if (SubdivColorMap(NewColorSubdiv, *ColorMapSize, &NewColorMapSize) !=
       GIF_OK) {
        free((char *)ColorArrayEntries);
        return GIF_ERROR;
    }
    if (NewColorMapSize < *ColorMapSize) {
        /* And clear rest of color map: */
        for (i = NewColorMapSize; i < *ColorMapSize; i++)
            OutputColorMap[i].Red = OutputColorMap[i].Green =
                OutputColorMap[i].Blue = 0;
    }

    /* Average the colors in each entry to be the color to be used in the
     * output color map, and plug it into the output color map itself. */
    for (i = 0; i < NewColorMapSize; i++) {
        if ((j = NewColorSubdiv[i].NumEntries) > 0) {
            QuantizedColor = NewColorSubdiv[i].QuantizedColors;
            Red = Green = Blue = 0;
            while (QuantizedColor) {
                QuantizedColor->NewColorIndex = i;
                Red += QuantizedColor->RGB[0];
                Green += QuantizedColor->RGB[1];
                Blue += QuantizedColor->RGB[2];
                QuantizedColor = QuantizedColor->Pnext;
            }
            OutputColorMap[i].Red = (Red << (8 - BITS_PER_PRIM_COLOR)) / j;
            OutputColorMap[i].Green = (Green << (8 - BITS_PER_PRIM_COLOR)) / j;
            OutputColorMap[i].Blue = (Blue << (8 - BITS_PER_PRIM_COLOR)) / j;
        } else
            fprintf(stderr,
                    "Null entry in quantized color map - that's weird.\n");
    }

    /* Finally scan the input buffer again and put the mapped index in the
     * output buffer.  */
    MaxRGBError[0] = MaxRGBError[1] = MaxRGBError[2] = 0;
    for (i = 0; i < (int)(Width * Height); i++) {
        Index = ((RedInput[i] >> (8 - BITS_PER_PRIM_COLOR)) <<
                 (2 * BITS_PER_PRIM_COLOR)) +
                ((GreenInput[i] >> (8 - BITS_PER_PRIM_COLOR)) <<
                 BITS_PER_PRIM_COLOR) +
                (BlueInput[i] >> (8 - BITS_PER_PRIM_COLOR));
        Index = ColorArrayEntries[Index].NewColorIndex;
        OutputBuffer[i] = Index;
        if (MaxRGBError[0] < ABS(OutputColorMap[Index].Red - RedInput[i]))
            MaxRGBError[0] = ABS(OutputColorMap[Index].Red - RedInput[i]);
        if (MaxRGBError[1] < ABS(OutputColorMap[Index].Green - GreenInput[i]))
            MaxRGBError[1] = ABS(OutputColorMap[Index].Green - GreenInput[i]);
        if (MaxRGBError[2] < ABS(OutputColorMap[Index].Blue - BlueInput[i]))
            MaxRGBError[2] = ABS(OutputColorMap[Index].Blue - BlueInput[i]);
    }

    free((char *)ColorArrayEntries);

    *ColorMapSize = NewColorMapSize;

    return GIF_OK;
}

/******************************************************************************
 * Routine to subdivide the RGB space recursively using median cut in each
 * axes alternatingly until ColorMapSize different cubes exists.
 * The biggest cube in one dimension is subdivide unless it has only one entry.
 * Returns GIF_ERROR if failed, otherwise GIF_OK.
 ******************************************************************************/
static int
SubdivColorMap(NewColorMapType * NewColorSubdiv,
               unsigned int ColorMapSize,
               unsigned int *NewColorMapSize) {

    int MaxSize;
    unsigned int i, j, Index = 0, NumEntries, MinColor, MaxColor;
    long Sum, Count;
    QuantizedColorType *QuantizedColor, **SortArray;

    while (ColorMapSize > *NewColorMapSize) {
        /* Find candidate for subdivision: */
        MaxSize = -1;
        for (i = 0; i < *NewColorMapSize; i++) {
            for (j = 0; j < 3; j++) {
                if ((((int)NewColorSubdiv[i].RGBWidth[j]) > MaxSize) &&
                      (NewColorSubdiv[i].NumEntries > 1)) {
                    MaxSize = NewColorSubdiv[i].RGBWidth[j];
                    Index = i;
                    SortRGBAxis = j;
                }
            }
        }

        if (MaxSize == -1)
            return GIF_OK;

        /* Split the entry Index into two along the axis SortRGBAxis: */

        /* Sort all elements in that entry along the given axis and split at
         * the median.  */
        SortArray = (QuantizedColorType **)malloc(
                      sizeof(QuantizedColorType *) * 
                      NewColorSubdiv[Index].NumEntries);
        if (SortArray == NULL)
            return GIF_ERROR;
        for (j = 0, QuantizedColor = NewColorSubdiv[Index].QuantizedColors;
             j < NewColorSubdiv[Index].NumEntries && QuantizedColor != NULL;
             j++, QuantizedColor = QuantizedColor->Pnext)
            SortArray[j] = QuantizedColor;

        qsort(SortArray, NewColorSubdiv[Index].NumEntries,
              sizeof(QuantizedColorType *), SortCmpRtn);

        /* Relink the sorted list into one: */
        for (j = 0; j < NewColorSubdiv[Index].NumEntries - 1; j++)
            SortArray[j]->Pnext = SortArray[j + 1];
        SortArray[NewColorSubdiv[Index].NumEntries - 1]->Pnext = NULL;
        NewColorSubdiv[Index].QuantizedColors = QuantizedColor = SortArray[0];
        free((char *)SortArray);

        /* Now simply add the Counts until we have half of the Count: */
        Sum = NewColorSubdiv[Index].Count / 2 - QuantizedColor->Count;
        NumEntries = 1;
        Count = QuantizedColor->Count;
        while (QuantizedColor->Pnext != NULL &&
	       (Sum -= QuantizedColor->Pnext->Count) >= 0 &&
               QuantizedColor->Pnext->Pnext != NULL) {
            QuantizedColor = QuantizedColor->Pnext;
            NumEntries++;
            Count += QuantizedColor->Count;
        }
        /* Save the values of the last color of the first half, and first
         * of the second half so we can update the Bounding Boxes later.
         * Also as the colors are quantized and the BBoxes are full 0..255,
         * they need to be rescaled.
         */
        MaxColor = QuantizedColor->RGB[SortRGBAxis]; /* Max. of first half */
	/* coverity[var_deref_op] */
        MinColor = QuantizedColor->Pnext->RGB[SortRGBAxis]; /* of second */
        MaxColor <<= (8 - BITS_PER_PRIM_COLOR);
        MinColor <<= (8 - BITS_PER_PRIM_COLOR);

        /* Partition right here: */
        NewColorSubdiv[*NewColorMapSize].QuantizedColors =
           QuantizedColor->Pnext;
        QuantizedColor->Pnext = NULL;
        NewColorSubdiv[*NewColorMapSize].Count = Count;
        NewColorSubdiv[Index].Count -= Count;
        NewColorSubdiv[*NewColorMapSize].NumEntries =
           NewColorSubdiv[Index].NumEntries - NumEntries;
        NewColorSubdiv[Index].NumEntries = NumEntries;
        for (j = 0; j < 3; j++) {
            NewColorSubdiv[*NewColorMapSize].RGBMin[j] =
               NewColorSubdiv[Index].RGBMin[j];
            NewColorSubdiv[*NewColorMapSize].RGBWidth[j] =
               NewColorSubdiv[Index].RGBWidth[j];
        }
        NewColorSubdiv[*NewColorMapSize].RGBWidth[SortRGBAxis] =
           NewColorSubdiv[*NewColorMapSize].RGBMin[SortRGBAxis] +
           NewColorSubdiv[*NewColorMapSize].RGBWidth[SortRGBAxis] - MinColor;
        NewColorSubdiv[*NewColorMapSize].RGBMin[SortRGBAxis] = MinColor;

        NewColorSubdiv[Index].RGBWidth[SortRGBAxis] =
           MaxColor - NewColorSubdiv[Index].RGBMin[SortRGBAxis];

        (*NewColorMapSize)++;
    }

    return GIF_OK;
}

/****************************************************************************
 * Routine called by qsort to compare two entries.
 ****************************************************************************/
static int
SortCmpRtn(const void *Entry1,
           const void *Entry2) {

    return (*((QuantizedColorType **) Entry1))->RGB[SortRGBAxis] -
       (*((QuantizedColorType **) Entry2))->RGB[SortRGBAxis];
}

/******************************************************************************
* Close output file (if open), and exit.
******************************************************************************/
static void QuitGifError(GifFileType *GifFile)
{
    fprintf(stderr, "Error writing GIF file\n");
#if GIFLIB_MAJOR >= 5
    if (GifFile != NULL) EGifCloseFile(GifFile, NULL);
#else
    if (GifFile != NULL) EGifCloseFile(GifFile);
#endif
}

int 
write_gif(const char *filename, int width, int height, char *rgb)
{
    int i;
    GifByteType *red, *green, *blue;

    GifByteType *OutputBuffer = NULL;
    ColorMapObject *OutputColorMap = NULL;

    GifFileType *GifFile;
    GifByteType *Ptr;

    bool GifTestExistance = false;

    ColorMapSize = 1 << ExpNumOfColors;

#if GIFLIB_MAJOR >= 5
    OutputColorMap = GifMakeMapObject(ColorMapSize, NULL);
#else
    OutputColorMap = MakeMapObject(ColorMapSize, NULL);
#endif
    
    if (OutputColorMap == NULL ||
	(OutputBuffer = (GifByteType *) malloc(width * height *
					       sizeof(GifByteType))) == NULL)
    {
	fprintf(stderr,"Failed to allocate memory required, aborted.");
	return(0);
    }

    red = malloc(width * height * sizeof(GifByteType));
    green = malloc(width * height * sizeof(GifByteType));
    blue = malloc(width * height * sizeof(GifByteType));
    if (red == NULL || green == NULL || blue == NULL)
    {
	fprintf(stderr, "Can't allocate memory for GIF file.\n");
	return(0);
    }
    for (i = 0; i < width * height; i++)
    {
	red[i]   = (GifByteType) rgb[3*i  ];
	green[i] = (GifByteType) rgb[3*i+1];
	blue[i]  = (GifByteType) rgb[3*i+2];
    }

    if (QuantizeBuffer(width, height, &ColorMapSize,
		       red, green, blue,
		       OutputBuffer, OutputColorMap->Colors) == GIF_ERROR)

    {
	QuitGifError(NULL);
	return(0);
    }

    free(red);
    free(green);
    free(blue);

    Ptr = OutputBuffer;

#if GIFLIB_MAJOR >= 5
    GifFile = EGifOpenFileName(filename, GifTestExistance, NULL);
#else
    GifFile = EGifOpenFileName(filename, GifTestExistance);
#endif
    /* Open stdout for the output file: */
    if (GifFile == NULL)
    {
	QuitGifError(GifFile);
	return(0);
    }

    if (EGifPutScreenDesc(GifFile,
			  width, height, ExpNumOfColors, 0,
			  OutputColorMap) == GIF_ERROR ||
	EGifPutImageDesc(GifFile,
			 0, 0, width, height, false, NULL) == GIF_ERROR)

    {
	QuitGifError(GifFile);
	return(0);
    }

    for (i = 0; i < height; i++) {
	if (EGifPutLine(GifFile, Ptr, width) == GIF_ERROR)

	{
	    QuitGifError(GifFile);
	    return(0);
	}
	Ptr += width;
    }

#if GIFLIB_MAJOR >= 5
    if (EGifCloseFile(GifFile, NULL) == GIF_ERROR)
#else
    if (EGifCloseFile(GifFile) == GIF_ERROR)
#endif

    {
	QuitGifError(GifFile);
	return(0);
    }

    return(1);
}
