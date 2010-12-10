/**
  * Copyright (C)2010 by Michel Jansen and Richard Loos
  * All rights reserved.
  *
  * This file is part of the plvcore module of ParleVision.
  *
  * ParleVision is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * ParleVision is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * A copy of the GNU General Public License can be found in the root
  * of this software package directory in the file LICENSE.LGPL.
  * If not, see <http://www.gnu.org/licenses/>.
  */

#ifndef EDGEDETECTORSOBEL_H
#define EDGEDETECTORSOBEL_H

#include <plvcore/PipelineProcessor.h>

namespace plv
{
    class OpenCVImage;
    class CvMatDataInputPin;
    class CvMatDataOutputPin;
}

namespace plvopencv
{
    class EdgeDetectorSobel : public plv::PipelineProcessor
    {
        Q_OBJECT
        Q_DISABLE_COPY( EdgeDetectorSobel )
        Q_CLASSINFO("author", "Richard, Wim, Dennis")
        Q_CLASSINFO("name", "Edge detector Sobel")
        Q_CLASSINFO("description", "Edge detection using the Sobel method.");

        Q_PROPERTY( bool xOrder READ getXOrder WRITE setXOrder NOTIFY xOrderChanged )
        Q_PROPERTY( bool yOrder READ getYOrder WRITE setYOrder NOTIFY yOrderChanged )
        Q_PROPERTY( int apertureSize READ getApertureSize WRITE setApertureSize NOTIFY apertureSizeChanged )
        Q_PROPERTY( bool Scharr READ getScharr WRITE setScharr NOTIFY ScharrChanged )

        /** required standard method declaration for plv::PipelineElement */
        PLV_PIPELINE_ELEMENT

    public:
        EdgeDetectorSobel();
        virtual ~EdgeDetectorSobel();

        /** propery methods */
        bool getXOrder() const;
        bool getYOrder() const;
        bool getScharr() const;
        int getApertureSize() const;

    signals:
        void xOrderChanged(bool x);
        void yOrderChanged(bool y);
        void apertureSizeChanged(int newValue);
        void ScharrChanged(bool useScharr);

    public slots:
        /** ApertureSize � Size of the extended Sobel kernel, must be 1, 3, 5 or 7 */
        void setApertureSize(int a);

        /** xorder � Order of the derivative x */
        void setXOrder( bool x );

        /** yorder � Order of the derivative y */
        void setYOrder( bool y );

        /** Use special Scharr kernel. Bypasses aperture size */
        void setScharr( bool useScharr );

    private:
        plv::CvMatDataInputPin* m_inputPin;
        plv::CvMatDataOutputPin* m_outputPin;

        bool m_xOrder;
        bool m_yOrder;
        int m_apertureSize;
        bool m_Scharr;
    };
}
#endif // EDGEDETECTORSOBEL_H
