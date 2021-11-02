#ifndef mulle_c_string_escape_version_h__
#define mulle_c_string_escape_version_h__

/*
 *  version:  major, minor, patch
 */
#define MULLE_C_STRING_ESCAPE_VERSION  ((1 << 20) | (0 << 8) | 0)


static inline unsigned int   mulle_c_string_escape_get_version_major( void)
{
   return( MULLE_C_STRING_ESCAPE_VERSION >> 20);
}


static inline unsigned int   mulle_c_string_escape_get_version_minor( void)
{
   return( (MULLE_C_STRING_ESCAPE_VERSION >> 8) & 0xFFF);
}


static inline unsigned int   mulle_c_string_escape_get_version_patch( void)
{
   return( MULLE_C_STRING_ESCAPE_VERSION & 0xFF);
}

#endif
