
#include "FFTConvolver.h"

#include <cassert>
#include <cmath>

#if defined (FFTCONVOLVER_USE_SSE)
#include <xmmintrin.h>
#endif


//http://pcfarina.eng.unipr.it/Aida/aida-old/sviluppo_algo_en.php?id=5

FFTConvolver::FFTConvolver()
{
    reset();
    

    
}

  
FFTConvolver::~FFTConvolver()
{
    reset();
    for (unsigned long i=0; i<segCount; ++i)
    {
        delete segments[i];
        delete segmentsIR[i];
    }
    
}

  
void FFTConvolver::reset()
{  

  blockSize = 0;
  segSize = 0;
  segCount = 0;
  fftComplexSize = 0;
  segments.clear();
  segmentsIR.clear();
  fftBuffer.clear();
  fft.init(0);
  preMultiplied.clear();
  conv.clear();
  overlap.clear();
  current = 0;
  inputBuffer.clear();
  //inputBufferFill = 0;
}

  
void FFTConvolver::init(unsigned long bufferSize, const float* ir, unsigned long irLen)
{
  reset();

  /*if (blockSize == 0)
  {
    return false;
  }
  
  // Ignore zeros at the end of the impulse response because they only waste computation time
  while (irLen > 0 && ::fabs(ir[irLen-1]) < 0.000001f)
  {
    --irLen;
  }

  if (irLen == 0)
  {
    return true;
  }*/ // cambiar por assert
  
  blockSize = NextPowerOf2(bufferSize);
  segSize = 2 * blockSize;
  segCount = static_cast<unsigned long>(::ceil(static_cast<float>(irLen) / static_cast<float>(blockSize)));
  
  //http://dsp.stackexchange.com/questions/12972/discrete-fourier-transform-what-is-the-dc-term-really
  //http://www.gaussianwaves.com/2015/11/interpreting-fft-results-complex-dft-frequency-bins-and-fftshift/
  
  fftComplexSize = audiofft::AudioFFT::ComplexSize(segSize);// Tamño igual a segSize/2 por frecuencia DC 0hz
  
  // FFT
  fft.init(segSize);
  fftBuffer.resize(segSize);
  
  // Prepare segments
  for (unsigned long i=0; i<segCount; ++i)
  {
    segments.push_back(new SplitComplex(fftComplexSize));
  }
  
  // Prepare IR
  

  /*for (unsigned long i=0; i<segCount; ++i)
  {
    SplitComplex* segment = new SplitComplex(fftComplexSize);
    const unsigned long remaining = irLen - (i * blockSize);
    const unsigned long sizeCopy = (remaining >= blockSize) ? blockSize : remaining;
    CopyAndPad(fftBuffer, &ir[i*blockSize], sizeCopy);
    fft.fft(fftBuffer.data(), segment->re(), segment->im());
    segmentsIR.push_back(segment);
  }*/
    
   // Prepare IR
    
    unsigned long remainingIR = 0; //Remaining IR size
    //unsigned long sizeCopy = 0;
    
         segment = new SplitComplex(fftComplexSize);// Revisar porqe no funciona sin reiniciar
    
    fftBuffer.setZero();
    
    for (unsigned long i=0; i<segCount; ++i)
    {
        
   
        remainingIR = irLen - (i * blockSize);
        //sizeCopy = (remaining >= blockSize) ? blockSize : remaining;
        
        if(remainingIR>=blockSize)
            CopyAndPad(fftBuffer, &ir[i*blockSize], blockSize);
        else
            CopyAndPad(fftBuffer, &ir[i*blockSize], remainingIR);
        
        
        
        
        fft.fft(fftBuffer.data(), segment->re(), segment->im());
        segmentsIR.push_back(segment);
    }
  
  // Prepare convolution buffers  
  preMultiplied.resize(fftComplexSize);
  conv.resize(fftComplexSize);
  overlap.resize(blockSize);
  
  // Prepare input buffer
  inputBuffer.resize(blockSize);
  //inputBufferFill = 0;

  // Reset current position
  current = 0;
    
    inputBufferFill=0;
  
  //return true;
}


void FFTConvolver::process(const float* input, float* output, size_t len)
{
    if (segCount == 0)
    {
        ::memset(output, 0, len * sizeof(float));
        return;
    }
    
    size_t processed = 0;
    while (processed < len)
    {
        const bool inputBufferWasEmpty = (inputBufferFill == 0);
        const size_t processing = std::min(len-processed, (size_t)blockSize-inputBufferFill);
        const size_t inputBufferPos = inputBufferFill;
        ::memcpy(inputBuffer.data()+inputBufferPos, input+processed, processing * sizeof(float));
        
        // Forward FFT
        CopyAndPad(fftBuffer, &inputBuffer[0], blockSize);
        fft.fft(fftBuffer.data(), segments[current]->re(), segments[current]->im());
        
        // Complex multiplication
        if (inputBufferWasEmpty)
        {
            preMultiplied.setZero();
            for (size_t i=1; i<segCount; ++i)
            {
                const size_t indexIr = i;
                const size_t indexAudio = (current + i) % segCount;
                ComplexMultiplyAccumulate(preMultiplied, *segmentsIR[indexIr], *segments[indexAudio]);
            }
        }
        conv.copyFrom(preMultiplied);
        ComplexMultiplyAccumulate(conv, *segments[current], *segmentsIR[0]);
        
        // Backward FFT
        fft.ifft(fftBuffer.data(), conv.re(), conv.im());
        
        // Add overlap
        Sum(output+processed, fftBuffer.data()+inputBufferPos, overlap.data()+inputBufferPos, processing);
        
        // Input buffer full => Next block
        inputBufferFill += processing;
        if (inputBufferFill == blockSize)
        {
            // Input buffer is empty again now
            inputBuffer.setZero();
            inputBufferFill = 0;
            
            // Save the overlap
            ::memcpy(overlap.data(), fftBuffer.data()+blockSize, blockSize * sizeof(float));
            
            // Update current segment
            current = (current > 0) ? (current - 1) : (segCount - 1);
        }
        
        processed += processing;
    }


}

void FFTConvolver::setIR(const float* ir)
{
    
    // Prepare IR
    for (size_t i=0; i<segCount; ++i)
    {
        const size_t remaining = 128 - (i * blockSize);
        const size_t sizeCopy = (remaining >= blockSize) ? blockSize : remaining;
        CopyAndPad(fftBuffer, &ir[i*blockSize], sizeCopy);
        fft.fft(fftBuffer.data(), segment->re(), segment->im());
        segmentsIR[i]=segment;
    }

     current = 0;
    //overlap.setZero();

}



