// -----------------------------------------------------------------------------
// gadget_endian_tools.cc                                                       
// Implementation of the class GadgetEndianTools                                
//                                                                              
// This class perform data IO operation according to the endianness             
//                                                                              
// Intel/Amd/Alpha are little-endian compliant                                  
//                                                                              
// PowerPC IBM/Sparc/SGI are big-endian compliant                               
// -----------------------------------------------------------------------------

#include <iostream>
#include "gadget_endian_tools.h"


// -----------------------------------------------------------------------------
// Constructor
GadgetEndianTools::GadgetEndianTools(const FILE * _fd,const  bool _swap)
{
  // get input paramters
  fd   = _fd;
  swap = _swap;
}

// -----------------------------------------------------------------------------
// Destructor
GadgetEndianTools::~GadgetEndianTools()
{
}

// -----------------------------------------------------------------------------
// swapBytes
// reverse byte order for the word 'x' 
// example : x a 4 bytes word
//    x        -> 1 2 3 4
// swapBytes x -> 4 3 2 1
void GadgetEndianTools::swapBytes( void * x,
				  const int size /* size of the word */
				  )
{
  char * p;

  p = (char *) x;
  for (int i=0;i<size/2;i++) {
     int t=*(p+i);
     *(p+i)=*(p+size-i-1);
     *(p+size-i-1)=t;
  }
}
// -----------------------------------------------------------------------------
// ioHeader:
// perform IO (Read,Write) operation on the header
int GadgetEndianTools::ioHeader(t_io_header_1 * header,const ioop op)
{
  std::cerr << "header adress ioHeader=" << header <<"\n";
  char * ptr=(char *) header;
  char *pp=(char *)ptr;
  std::cerr << "pptr adress ioHeader=" << ptr <<"\n";
  for (int i=0; i<NB_DATA_HEADER; i++) { // loop on all header member data
    // process IO  header member data
    ioData(ptr, dth[i].size_bytes, dth[i].items, op);
    // forward ptr
    pp += (dth[i].size_bytes*dth[i].items);
    ptr = pp;
  }

  return 1;
}
// -----------------------------------------------------------------------------
// ioHeader:
// perform IO (Read,Write) operation on Data
int GadgetEndianTools::ioData(char * ptr,int size_bytes, int items,const ioop op)
{

  int status;
  char * pp;
  switch (op) {

  case READ :
    // get data from file
    status = fread(ptr,size_bytes,items,const_cast<FILE*> (fd));
    if (!status) {
      std::cerr << "Error occured in READ operation, aborting\n";
      std::exit(1);
    }
    // We SWAP data
    if (swap && (size_bytes != CHAR)) { // swapping requested
      for (int i=0; i<items; i++) {
	swapBytes(ptr,size_bytes);
	ptr += size_bytes;
      }
    }
    break;

  case WRITE :
    // We must SWAP data first
    if (swap && (size_bytes != CHAR)) { // swapping requested
      pp = ptr;
      for (int i=0; i<items; i++) {
	swapBytes(ptr,size_bytes);
	ptr += size_bytes;
      }
      ptr = pp;
    }
    // Save Data to file
    status = fwrite(ptr,size_bytes,items,const_cast<FILE*> (fd));
    if (!status) {
      std::cerr << "Error occured in WRITE operation, aborting\n";
      std::exit(1);
    }
    // We have to unswap the data now
    if (swap && (size_bytes != CHAR)) { // swapping requested

      for (int i=0; i<items; i++) {
	swapBytes(ptr,size_bytes);
	ptr += size_bytes;
      }
    }
    break;
  } // end of switch (op) ....

  return 1;
}
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
