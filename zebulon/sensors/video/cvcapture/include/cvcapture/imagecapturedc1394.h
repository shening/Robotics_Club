////////////////////////////////////////////////////////////////////////////////////
///
///  Filename:  imagecapturedc1394.h
///
///  Copyright 2009 Gary Stein
///                 Daniel Barber
///                 David Adams
///                 Robotics Laboratory
///                 University of Central Florida
///                 http://robotics.ucf.edu
///
///  Program Contents:
///  -------------------------------------------------------------------------------
///  Contains a wrapper class for the libv4l image capture library.
///  -------------------------------------------------------------------------------
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the Robotics Club at UCF, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
///
///  THIS SOFTWARE IS PROVIDED BY THE ROBOTICS CLUB AT UCF ''AS IS'' AND ANY
///  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
///  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
///  DISCLAIMED. IN NO EVENT SHALL UCF BE LIABLE FOR ANY
///  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
///  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
///  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
///  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
///  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
///  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
////////////////////////////////////////////////////////////////////////////////////
#ifndef _CV_IMAGE_CAPTURE_DC1394__H
#define _CV_IMAGE_CAPTURE_DC1394__H

#ifndef WIN32

#include "cvcapture/cvimagecapture.h"

#include <fw/fwConfig.h>
#include <fw/fwDebug.h>
#include <fw/fwTime.h>
#include <fw/fw1394Camera.h>


namespace Zebulon
{
    namespace Video
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class ImageCaptureDC1394
        ///   \brief Video Capture interface that uses fw1394 to acquire video from
        ///          capture cards and other devices in Linux.
        ///
        ///   Required Libraries:
        ///   Windows: Not supported.
        ///   Linux: fw1394
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ImageCaptureDC1394 : public CvCaptureBase
        {
        public:
            ImageCaptureDC1394();
            ~ImageCaptureDC1394();
            virtual int Start(const std::string& videoSource,
                              const int width = 0,
                              const int height = 0,
                              const bool interlace = false);
            virtual int Stop();
            virtual int GetFrame(CvImageCapture::Image* dest, const bool block = false);
            virtual int GetRawFrame(CvImageCapture::Image* dest, const bool block = false) { return 0; }
            virtual bool IsReady() const;
            static unsigned int ScanForSources(std::vector<std::string>& sources);
            fw1394Camera *GetCamera() { return mCamera; };
        private:
            CvImageCapture::Image mImage;   ///<  Image data.
            fw1394Camera *mCamera;          ///<  Pointer to file descriptor.
            int mFirst;                     ///<  First time capture flag.
            unsigned int mWidth;            ///<  Capture width.
            unsigned int mHeight;           ///<  Capture height.
            volatile bool mInterlacedFlag;  ///< Used to determine if image should be de-interlaced.
        };
    }
}
#endif

#endif

/*  End of File */
