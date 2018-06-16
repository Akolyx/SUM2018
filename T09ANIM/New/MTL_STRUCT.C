/*** Materials handle ***/

/* Material store type */
typedef struct tagvg4MATERIAL
{
  CHAR Name[VG4_STR_MAX]; /* Material name */

  /* Illumination coefficients */    
  VEC Ka, Kd, Ks;         /* Ambient, diffuse, specular coefficients */
  FLT Ph;                 /* Phong power coefficient */
  FLT Trans;              /* Transparency factor */
  INT Tex[8];             /* Texture references from texture table */

  /* Shader string */
  CHAR ShaderStr[VG4_STR_MAX];

  INT ShdNo;              /* Shader number in shader array */
} vg4MATERIAL;

