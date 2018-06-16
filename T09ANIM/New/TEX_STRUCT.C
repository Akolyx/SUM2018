
/***
 * Image handle
 ***/

/* Image representation type */
typedef struct tagvg4IMAGE
{
  INT W, H;    /* Image size in pixels */
  DWORD *Bits; /* Image pixels: pixel(X,Y) = Bits[Y * W + X] */

  HBITMAP hBm; /* WinAPI bitmap handle */
} vg4IMAGE;

/* Load image from BMP file function.
 * ARGUMENTS:
 *   - image structure to be load to:
 *       vg4IMAGE *Img;
 *   - image file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL VG4_RndImgLoad( vg4IMAGE *Img, CHAR *FileName );

/* Free image function.
 * ARGUMENTS:
 *   - image structure to be load to:
 *       vg4IMAGE *Img;
 * RETURNS: None.
 */
VOID VG4_RndImgFree( vg4IMAGE *Img );

/* Upload image to OpenGL texture memory function.
 * ARGUMENTS:
 *   - image structure to be load to:
 *       vg4IMAGE *Img;
 * RETURNS: None.
 */
VOID VG4_RndImgToTex( vg4IMAGE *Img );

/* Otain image pixel color function.
 * ARGUMENTS:
 *   - image structure to be load to:
 *       vg4IMAGE *Img;
 *   - pixel coordinates:
 *       INT X, Y;
 * RETURNS: None.
 */
DWORD VG4_RndImgGet( vg4IMAGE *Img, INT X, INT Y );

/***
 * Texture handle
 ***/

/* Texture representation type */
typedef struct tagvg4TEXTURE
{
  CHAR Name[VG4_STR_MAX]; /* Texture name */
  INT W, H;                          /* Texture size in pixels */
  UINT TexId;                        /* OpenGL texture Id */ 
} vg4TEXTURE;

/* Texture stock maximum size */
#define VG4_MAX_TEXTURES 30
/* Array of textures */
extern vg4TEXTURE VG4_RndTextures[VG4_MAX_TEXTURES];
/* Shadres array store size */
extern INT VG4_RndTexturesSize;

/* Texture storage initialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VG4_RndTexInit( VOID );

/* Texture storage deinitialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VG4_RndTexClose( VOID );

/* Add texture from image to stock from file function.
 * ARGUMENTS:
 *   - image name:
 *       CHAR *Name;
 *   - image size:
 *       INT W, H;
 *   - image pixels data:
 *       DWORD *Bits;
 * RETURNS:
 *   (INT) new texture stock number.
 */
INT VG4_RndTexAddImg( CHAR *Name, INT W, INT H, DWORD *Bits );

/* Add texture to stock from file function.
 * ARGUMENTS:
 *   - texture file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (INT) new texture stock number (-1 if error occured).
 */
INT VG4_RndTexAdd( CHAR *FileName );
