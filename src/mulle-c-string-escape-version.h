#ifndef mulle_c_string_escape_version_h__
#define mulle_c_string_escape_version_h__

/*
 *  version:  major, minor, patch
 */
#define MULLE__C__STRING__ESCAPE_VERSION  ((1UL << 20) | (1 << 8) | 3)


static inline unsigned int   mulle_c_string_escape_get_version_major( void)
{
   return( MULLE__C__STRING__ESCAPE_VERSION >> 20);
}


static inline unsigned int   mulle_c_string_escape_get_version_minor( void)
{
   return( (MULLE__C__STRING__ESCAPE_VERSION >> 8) & 0xFFF);
}


static inline unsigned int   mulle_c_string_escape_get_version_patch( void)
{
   return( MULLE__C__STRING__ESCAPE_VERSION & 0xFF);
}

#endif
