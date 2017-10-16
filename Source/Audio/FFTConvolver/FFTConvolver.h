
#ifndef FFTCONVOLVERFFTCONVOLVERH
#define FFTCONVOLVERFFTCONVOLVERH

#include "AudioFFT.h"
#include "Utilities.h"
#include "../JuceLibraryCode/JuceHeader.h"

#include <vector>



/**
* @class FFTConvolver
* @brief Implementation of a partitioned FFT convolution algorithm with uniform block size
*
* Some notes on how to use it:
*
* - After initialization with an impulse response, subsequent data portions of
*   arbitrary length can be convolved. The convolver internally can handle
*   this by using appropriate buffering.
*
* - The convolver works without "latency" (except for the required
*   processing time, of course), i.e. the output always is the convolved
*   input for each processing call.
*
* - The convolver is suitable for real-time processing which means that no
*   "unpredictable" operations like allocations, locking, API calls, etc. are
*   performed during processing (all necessary allocations and preparations take
*   place during initialization).
*/
class FFTConvolver
{  
public:
  FFTConvolver();  
  virtual ~FFTConvolver();
  
  /**
  * @brief Initializes the convolver
  * @param blockSize Block size internally used by the convolver (partition size)
  * @param ir The impulse response
  * @param irLen Length of the impulse response
  * @return true: Success - false: Failed
  */
  void init(unsigned long blockSize, const float* ir, unsigned long irLen);

  /**
  * @brief Convolves the the given input floats and immediately outputs the result
  * @param input The input floats
  * @param output The convolution result
  * @param len Number of input/output floats
  */
  void process(const float* input, float* output, size_t len);

  /**
  * @brief Resets the convolver and discards the set impulse response
  */
  void reset();
  void setIR(const float* ir);

  
private:
  unsigned long blockSize;
  unsigned long segSize;
  unsigned long segCount;
  unsigned long fftComplexSize;
  std::vector<SplitComplex*> segments;
  std::vector<SplitComplex*> segmentsIR;
  floatBuffer fftBuffer;
  audiofft::AudioFFT fft;
  SplitComplex preMultiplied;
  SplitComplex conv;
  floatBuffer overlap;
  unsigned long current;
  floatBuffer inputBuffer;
  //unsigned long inputBufferFill;
  SplitComplex* segment;
    
  size_t inputBufferFill;
  

};
  
 

#endif // Header guard
