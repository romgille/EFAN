//=================================================================================
//  efan_mex.cpp
//
//
//  AUTORIGHTS
//  Copyright (C) 2016 Ecole Polytechnique Federale de Lausanne (EPFL), Switzerland.
//
//  Created by Radhakrishna Achanta on 08/08/16.
//=================================================================================
/*Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met
 
 * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 * Neither the name of EPFL nor the
 names of its contributors may be used to endorse or promote products
 derived from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include<mex.h>
#include <cmath>
#include <cfloat>


//===============================================================================
// This version takes in as parameters: the input image, and the mask of 0's and 1's
//===============================================================================
void mexFunction(int nlhs, mxArray *plhs[],
                 int nrhs, const mxArray *prhs[])
{
    //    if (nrhs < 1) {
    //        mexErrMsgTxt("At least one argument - the input image array.") ;
    //    } else if(nrhs > 2) {
    //        mexErrMsgTxt("Too many input arguments.");
    //    }
    //    if(nlhs!=2) {
    //        mexErrMsgIdAndTxt("LHS: Two outputs required, the completed image and the mask.");
    //    }
    //---------------------------
    const int numelements   = mxGetNumberOfElements(prhs[0]) ;
    const mwSize numdims = mxGetNumberOfDimensions(prhs[0]);
    const mwSize* dims  = mxGetDimensions(prhs[0]) ;
    unsigned char* imgbytes = (unsigned char*)mxGetData(prhs[0]) ;//mxGetData returns a void pointer, so cast it
    //double* imgbytes = (double*)mxGetData(prhs[0]) ;//mxGetData returns a void pointer, so cast it
    double* mask     = (double*)mxGetData(prhs[1]);
    const int width = dims[1];
    const int height = dims[0];//Note: first dimension provided is height and second is width
    const int sz = width*height;
    const int szsz = sz+sz;
    
    //---------------------------
    // Allocate memory
    //---------------------------
    unsigned char* rin  = (unsigned char*)mxMalloc( sizeof(unsigned char)      * sz );
    unsigned char* gin  = (unsigned char*)mxMalloc( sizeof(unsigned char)      * sz );
    unsigned char* bin  = (unsigned char*)mxMalloc( sizeof(unsigned char)      * sz );
    unsigned char* rout = (unsigned char*)mxMalloc( sizeof(unsigned char)      * sz );
    unsigned char* gout = (unsigned char*)mxMalloc( sizeof(unsigned char)      * sz );
    unsigned char* bout = (unsigned char*)mxMalloc( sizeof(unsigned char)      * sz );
//    double* rsum        = (double*)mxMalloc( sizeof(double)      * sz );
//    double* gsum        = (double*)mxMalloc( sizeof(double)      * sz );
//    double* bsum        = (double*)mxMalloc( sizeof(double)      * sz );
//    double* distancesum = (double*)mxMalloc( sizeof(double)      * sz );
    double* rsum        = 0;
    double* gsum        = 0;
    double* bsum        = 0;
    double* distancesum = 0;
    int* randindx       = (int*)mxMalloc( sizeof(int)      * sz );
    int* randindy       = (int*)mxMalloc( sizeof(int)      * sz );

    //---------------------------
    // Read the pixel values
    //---------------------------
    //if(2 == numdims)
    if(numelements/sz == 1)//if it is a grayscale image, copy the values directly into the lab vectors
    {
        for(int x = 0, ii = 0; x < width; x++)//reading data from column-major MATLAB matrics to row-major C matrices (i.e perform transpose)
        {
            int ind = 0;
            for(int y = 0; y < height; y++)
            {
                //int i = y*width+x;
                int i = ind + x;
                rin[i] = imgbytes[ii];
                gin[i] = imgbytes[ii];
                bin[i] = imgbytes[ii];
                ii++;
                ind += width;
            }
        }
    }
    else//else convert from rgb to lab
    {
        for(int x = 0, ii = 0; x < width; x++)//reading data from column-major MATLAB matrics to row-major C matrices (i.e perform transpose)
        {
            int ind = 0;
            for(int y = 0; y < height; y++)
            {
                //int i = y*width+x;
                int i = ind + x;
                rin[i] = imgbytes[ii];
                gin[i] = imgbytes[ii+sz];
                bin[i] = imgbytes[ii+szsz];
                ii++;
                ind += width;
            }
        }
    }

    int numrandpixels = 0;
    for(int x = 0, ii = 0; x < width; x++)//reading data from column-major MATLAB matrics to row-major C matrices
    {
        for(int y = 0; y < height; y++, ii++)
        {
            if(mask[ii] > 0)
            {
                //randinds[numrandpixels] = y*width+x;
                randindx[numrandpixels] = x;
                randindy[numrandpixels] = y;
                numrandpixels++;
            }
        }
    }

    //===============================================================================
    if(0)//does not use zero padding
    {
        rsum        = (double*)mxMalloc( sizeof(double)      * sz );
        gsum        = (double*)mxMalloc( sizeof(double)      * sz );
        bsum        = (double*)mxMalloc( sizeof(double)      * sz );
        distancesum = (double*)mxMalloc( sizeof(double)      * sz );
        for(int i = 0; i < sz; i++)
        {
            distancesum[i] = 0;
            rsum[i] = gsum[i] = bsum[i] = 0;
        }
        
        //double sigma = sqrt((double)sz/(double)numrandpixels)/1.0;
        //const int off = int(sigma*2.0 + 0.5);
        const double PI	= 3.14159265358979323846264338327950288419716939937510;
        const double sigma = sqrt((double)sz/(numrandpixels*PI));
        const double kwd = 6.0*sigma;        const int off = int(0.5 + kwd/2);
        const int kernelw = off+off+1;
        const int kernelh = off+off+1;
        
        //FILE* pf = fopen("/Users/radhakrishnaachanta/rktemp/windowsize.txt","w");
        //fprintf(pf,"%d\n",kernelw);
        //fclose(pf);
        
        //double initkernel[kernelh][kernelw];
        double** filter = (double**)mxMalloc( sizeof(double*) * kernelh);
        for(int y = 0; y < kernelh; y++)
        {
            filter[y] = (double*)mxMalloc( sizeof(double) * kernelw);
            for(int x = 0; x < kernelw; x++)
            {
                double dist = ((x-off)*(x-off)+(y-off)*(y-off));
                filter[y][x] = exp(-0.5*dist/(sigma*sigma));
            }
        }
        
        for(int n = 0; n < numrandpixels; n++)
        {
            int xx = randindx[n];
            int yy = randindy[n];
            int ii = yy*width+xx;
            
            int x1 = xx - off; if(x1 < 0) x1 = 0;
            int x2 = xx + off; if(x2 >= width) x2 = width-1;
            int y1 = yy - off; if(y1 < 0) y1 = 0;
            int y2 = yy + off; if(y2 >= height) y2 = height-1;
            for(int y = y1; y <= y2; y++)
            {
                int ind = y*width;
                for(int x = x1; x <= x2; x++)
                {
                    int i = ind+x;
                    double dist = filter[y-yy+off][x-xx+off];
                    distancesum[i] += dist;
                    rsum[i] += rin[ii]*dist;
                    gsum[i] += gin[ii]*dist;
                    bsum[i] += bin[ii]*dist;
                }
            }
        }
        for(int i = 0; i < sz; i++)
        {
            double inv = 1.0/distancesum[i];
            rsum[i] *= inv;
            gsum[i] *= inv;
            bsum[i] *= inv;
        }
        
        for(int i = 0; i < sz; i++)
        {
            rout[i] = (unsigned char)(rsum[i] + 0.5);
            gout[i] = (unsigned char)(gsum[i] + 0.5);
            bout[i] = (unsigned char)(bsum[i] + 0.5);
        }
        // Deallocate the filter matrix
        for(int y = 0; y < kernelh; y++)
        {
            mxFree(filter[y]);
        }
        mxFree(filter);
    }
    if(1)//Use zero-padding (this is slightly faster)
    {
        //double sigma = sqrt((double)sz/(double)numrandpixels)/1.0;
        //const int off = int(sigma*2.0 + 0.5);
        const double PI	= 3.14159265358979323846264338327950288419716939937510;
        const double sigma = sqrt((double)sz/(numrandpixels*PI));
        const int margin = 2;//because it may not be a 'truely uniform' random distribution...
        const double kwd = 6.0*sigma;        const int off = int(0.5 + kwd/2) + margin;
        const int kernelw = off+off+1;
        const int kernelh = off+off+1;
        
        const int pad = off;
        const int pwd = width + pad + pad;
        const int pht = height+ pad + pad;
        const int padsz = pwd*pht;
        
        rsum        = (double*)mxMalloc( sizeof(double)      * padsz );
        gsum        = (double*)mxMalloc( sizeof(double)      * padsz );
        bsum        = (double*)mxMalloc( sizeof(double)      * padsz );
        distancesum = (double*)mxMalloc( sizeof(double)      * padsz );
        for(int i = 0; i < padsz; i++)
        {
            distancesum[i] = 0;
            rsum[i] = gsum[i] = bsum[i] = 0;
        }
        
        double** filter = (double**)mxMalloc( sizeof(double*) * kernelh);
        for(int y = 0; y < kernelh; y++)
        {
            filter[y] = (double*)mxMalloc( sizeof(double) * kernelw);
            for(int x = 0; x < kernelw; x++)
            {
                double dist = ((x-off)*(x-off)+(y-off)*(y-off));
                filter[y][x] = exp(-0.5*dist/(sigma*sigma));
            }
        }
        
        for(int n = 0; n < numrandpixels; n++)
        {
            int xx = randindx[n];
            int yy = randindy[n];
            int ii = yy*width+xx;
            
            int x1 = xx - off; 
            int x2 = xx + off; 
            int y1 = yy - off; 
            int y2 = yy + off;
            int ind = (y1+pad)*pwd;
            for(int y = y1; y <= y2; y++)
            {
                for(int x = x1; x <= x2; x++)
                {
                    int i = ind+x+pad;
                    double dist = filter[y-yy+off][x-xx+off];
                    distancesum[i] += dist;
                    rsum[i] += rin[ii]*dist;
                    gsum[i] += gin[ii]*dist;
                    bsum[i] += bin[ii]*dist;
                }
                ind += pwd;
            }
        }
        for(int i = 0; i < padsz; i++)
        {
            double inv = 1.0/distancesum[i];
            rsum[i] *= inv;
            gsum[i] *= inv;
            bsum[i] *= inv;
        }
        int ind=pad*pwd;
        for(int y = pad, i = 0; y < pht-pad; y++)
        {
            for(int x = pad; x < pwd-pad; x++, i++)
            {
                int ii = ind+x;
                rout[i] = (unsigned char)(rsum[ii] + 0.5);
                gout[i] = (unsigned char)(gsum[ii] + 0.5);
                bout[i] = (unsigned char)(bsum[ii] + 0.5);
            }
            ind += pwd;
        }
        // Deallocate the filter matrix
        for(int y = 0; y < kernelh; y++)
        {
            mxFree(filter[y]);
        }
        mxFree(filter);
    }
    //===============================================================================
    
    
    //---------------------------
    // Assign output image
    //---------------------------
    mwSize outdims[3] = {height,width,3};
    plhs[0] = mxCreateNumericArray(3,outdims,mxUINT8_CLASS,mxREAL);
    unsigned char* outbytes = (unsigned char*)mxGetData(plhs[0]);

    for(int x = 0, ii = 0; x < width; x++)//copying data from row-major C matrix to column-major MATLAB matrix (i.e. perform transpose)
    {
        int ind = 0;
        for(int y = 0; y < height; y++)
        {
            //int i = y*width+x;
            int i = ind + x;
            outbytes[ii] = rout[i];
            outbytes[ii+sz] = gout[i];
            outbytes[ii+szsz] = bout[i];
            ii++;
            ind += width;
        }
    }
    
    //---------------------------
    // Deallocate memory
    //---------------------------
    mxFree(rin);
    mxFree(gin);
    mxFree(bin);
    mxFree(rout);
    mxFree(gout);
    mxFree(bout);
    mxFree(rsum);
    mxFree(gsum);
    mxFree(bsum);
    mxFree(randindx);
    mxFree(randindy);
    //mxFree(randinds);
    mxFree(distancesum);
}
