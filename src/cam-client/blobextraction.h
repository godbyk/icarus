//***********************************************************//
//* Blob analysis package  8 August 2003                    *//
//* Version 1.0                                             *//
//* Input: IplImage* binary image                           *//
//* Output: attributes of each connected region             *//
//* Author: Dave Grossman                                   *//
//* Email: dgrossman@cdr.stanford.edu                       *//
//* Acknowledgement: the algorithm has been around > 20 yrs *//
//***********************************************************//


#if !defined(_CLASSE_BLOBEXTRACTION_INCLUDED)
#define _CLASSE_BLOBEXTRACTION_INCLUDED

//! Extreu els blobs d'una imatge
bool BlobAnalysis(IplImage* inputImage, uchar threshold, IplImage* maskImage,
				    bool borderColor, bool findmoments, blob_vector &RegionData );

 
// FUNCIONS AUXILIARS

//! Fusiona dos blobs
void Subsume(blob_vector &RegionData, int, int*, int, int, bool );
//! Crea un nou blob
void NewBlob(blob_vector &buffer);
//! Reallocata el vector auxiliar de blobs subsumats
int *NewSubsume(int *SubSumedRegion, int elems_inbuffer);
//! indica si un pixel és extern en la màscara (pq val 0 o té un vei a 0)
bool VeiMascara(IplImage *Mask, int x, int y);

#endif //_CLASSE_BLOBEXTRACTION_INCLUDED


