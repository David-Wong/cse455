///////////////////////////////////////////////////////////////////////////
//
// NAME
//  Image.h -- a simple reference-counted image structure
//
// DESCRIPTION
//  The CImage class is used to create and access weakly typed
//  two-dimensional images.  
//
//  The templated CImageOf<T> classes are used to create strongly typed images.
//  The currently supported pixel types are:
//      unsigned char, int, and float.
//
//  The images can have an arbitrary width, height, and also an arbitrary
//  number of bands (channels) per pixel.  For example, traditional RGBA
//  images can be represented using a 4-channel unsigned_8 image.
//
//  Images are normally allocated on the stack (NOT on the heap, i.e.,
//  "new Image" should not be used).  They can be freely returned from
//  functions and put into other data structures.  Assignment and copy
//  construction share memory (to copy pixel values from one image to
//  another one, use CopyPixels()).
//
// SEE ALSO
//  Image.cpp           implementation
//  RefCntMem.h         reference-counted memory object used by CImage
//
// Copyright 