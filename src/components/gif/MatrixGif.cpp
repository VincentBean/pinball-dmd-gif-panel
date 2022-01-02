#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <AnimatedGIF.h>
#include <SdFat.h>
#include "Globals.h"

int x_offset, y_offset, lastResult;
AnimatedGIF gif;
FsFile f;
bool interruptGif = false,
     gifPlaying = false;

// Draw a line of image directly on the LED Matrix
void GIFDraw(GIFDRAW *pDraw)
{
    uint8_t *s;
    uint16_t *d, *usPalette, usTemp[320];
    int x, y, iWidth;

  iWidth = pDraw->iWidth;
  if (iWidth > MATRIX_WIDTH)
      iWidth = MATRIX_WIDTH;

    usPalette = pDraw->pPalette;
    y = pDraw->iY + pDraw->y; // current line
    
    s = pDraw->pPixels;
    if (pDraw->ucDisposalMethod == 2) // restore to background color
    {
      for (x=0; x<iWidth; x++)
      {
        if (s[x] == pDraw->ucTransparent)
           s[x] = pDraw->ucBackground;
      }
      pDraw->ucHasTransparency = 0;
    }
    // Apply the new pixels to the main image
    if (pDraw->ucHasTransparency) // if transparency used
    {
      uint8_t *pEnd, c, ucTransparent = pDraw->ucTransparent;
      int x, iCount;
      pEnd = s + pDraw->iWidth;
      x = 0;
      iCount = 0; // count non-transparent pixels
      while(x < pDraw->iWidth)
      {
        c = ucTransparent-1;
        d = usTemp;
        while (c != ucTransparent && s < pEnd)
        {
          c = *s++;
          if (c == ucTransparent) // done, stop
          {
            s--; // back up to treat it like transparent
          }
          else // opaque
          {
             *d++ = usPalette[c];
             iCount++;
          }
        } // while looking for opaque pixels
        if (iCount) // any opaque pixels?
        {
          for(int xOffset = 0; xOffset < iCount; xOffset++ ){
            dma_display->drawPixel(x + xOffset, y, usTemp[xOffset]); // 565 Color Format
          }
          x += iCount;
          iCount = 0;
        }
        // no, look for a run of transparent pixels
        c = ucTransparent;
        while (c == ucTransparent && s < pEnd)
        {
          c = *s++;
          if (c == ucTransparent)
             iCount++;
          else
             s--; 
        }
        if (iCount)
        {
          x += iCount; // skip these
          iCount = 0;
        }
      }
    }
    else // does not have transparency
    {
      s = pDraw->pPixels;
      // Translate the 8-bit pixels through the RGB565 palette (already byte reversed)
      for (x=0; x<pDraw->iWidth; x++)
      {
        dma_display->drawPixel(x, y, usPalette[*s++]); // color 565
      }
    }
} /* GIFDraw() */


void * GIFOpenFile(const char *fname, int32_t *pSize)
{
  Serial.print("Playing gif: ");
  Serial.println(fname);
  f = sd.open(fname);

  Serial.print("Size: ");
  f.printName(&Serial);
  f.printFileSize(&Serial);
  f.printModifyDateTime(&Serial);
  f.printCreateDateTime(&Serial);
  
  if (f)
  {
    *pSize = f.size();
    return (void *)&f;
  }
  Serial.println("FAILED to open");
  return NULL;
} /* GIFOpenFile() */

void GIFCloseFile(void *pHandle)
{
  FsFile *f = static_cast<FsFile *>(pHandle);
  if (f != NULL)
     f->close();
} /* GIFCloseFile() */

int32_t GIFReadFile(GIFFILE *pFile, uint8_t *pBuf, int32_t iLen)
{
    int32_t iBytesRead;
    iBytesRead = iLen;
    FsFile *f = static_cast<FsFile *>(pFile->fHandle);
    // Note: If you read a file all the way to the last byte, seek() stops working
    if ((pFile->iSize - pFile->iPos) < iLen)
       iBytesRead = pFile->iSize - pFile->iPos - 1; // <-- ugly work-around
    if (iBytesRead <= 0)
       return 0;
    iBytesRead = (int32_t)f->read(pBuf, iBytesRead);
    pFile->iPos = f->position();
    return iBytesRead;
} /* GIFReadFile() */

int32_t GIFSeekFile(GIFFILE *pFile, int32_t iPosition)
{ 
  int i = micros();
  FsFile *f = static_cast<FsFile *>(pFile->fHandle);
  f->seek(iPosition);
  pFile->iPos = (int32_t)f->position();
  i = micros() - i;
//  Serial.printf("Seek time = %d us\n", i);
  return pFile->iPos;
} /* GIFSeekFile() */

unsigned long start_tick = 0;

// void ShowGIF(char *name)
// {
//   start_tick = millis();
   
//   if (gif.open(name, GIFOpenFile, GIFCloseFile, GIFReadFile, GIFSeekFile, GIFDraw))
//   {
//     x_offset = (MATRIX_WIDTH - gif.getCanvasWidth())/2;
//     if (x_offset < 0) x_offset = 0;
//     y_offset = (MATRIX_HEIGHT - gif.getCanvasHeight())/2;
//     if (y_offset < 0) y_offset = 0;
//     Serial.printf("Successfully opened GIF; Canvas size = %d x %d\n", gif.getCanvasWidth(), gif.getCanvasHeight());
//     Serial.flush();

// while (gif.playFrame(true, NULL))
//     {      
//       if ( (millis() - start_tick) > 8000) { // we'll get bored after about 8 seconds of the same looping gif
//         break;
//       }
//     }

//     gif.close();
//   }

// } /* ShowGIF() */

int LoadGIF(char *name)
{
  int result = gif.open(name, GIFOpenFile, GIFCloseFile, GIFReadFile, GIFSeekFile, GIFDraw);

  x_offset = (MATRIX_WIDTH - gif.getCanvasWidth()) / 2;
  if (x_offset < 0)
    x_offset = 0;

  y_offset = (MATRIX_HEIGHT - gif.getCanvasHeight()) / 2;
  if (y_offset < 0)
    y_offset = 0;

  return result;
}

void ShowGIF(char *name)
{
  if (!gifPlaying)
  {

    // isBusy

    LoadGIF(name);
    gifPlaying = true;
    interruptGif = false;
  }

  if (interruptGif && gifPlaying)
  {
    gifPlaying = interruptGif = false;
    gif.close();
    return;
  }

  lastResult = gif.playFrame(true, NULL);

  // Some GIFS return -1 even when there are no errors?
  // TODO: Find out why

  if (lastResult < 1 && allowNextGif)
  {
    gif.close();
    gifPlaying = false;
  }

} /* ShowGIF() */


void InitMatrixGif()
{
  gif.begin(LITTLE_ENDIAN_PIXELS);
}