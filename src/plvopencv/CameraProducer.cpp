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

#include "CameraProducer.h"

#include <QMutexLocker>
#include <QDebug>
#include <QStringBuilder>
#include <QVariant>

#include <plvcore/OpenCVImagePin.h>
#include "OpenCVCamera.h"


using namespace plv;
using namespace plvopencv;

CameraProducer::CameraProducer() :
        m_camera( new OpenCVCamera() ),
        m_cameraId( 0 ),
        m_width( 640 ),
        m_height( 480 ),
        m_lastProcessedId( 0 )
{
    // we have one output pin
    m_outputPin = createOpenCVImageOutputPin( "output", this );

    // supports all types of images
    m_outputPin->addAllChannels();
    m_outputPin->addAllDepths();

    // connect the camera to this camera producer using Qt's signals
    // and slots mechanism.
    connect( m_camera.getPtr(),
             SIGNAL( newFrame( plv::RefPtr<plv::Data> ) ),
             this,
             SLOT( newFrame( plv::RefPtr<plv::Data> ) ) );
}

CameraProducer::~CameraProducer()
{
}

void CameraProducer::process()
{
    QMutexLocker lock(&m_frameMutex);
    assert( m_lastFrame.isNotNull() );

    m_outputPin->put( m_lastFrame );

    // clear last frame so we do not process this image twice
    m_lastFrame.set( 0 );
}

void CameraProducer::newFrame( plv::RefPtr<plv::Data> frame )
{
    QMutexLocker lock(&m_frameMutex);
    m_lastFrame = ref_ptr_static_cast<OpenCVImage>( frame );
}

void CameraProducer::init()
{
    if( !m_camera->init(m_cameraId) )
    {
        QString msg = QString("Camera with id %1 failed to initialise")
                      .arg(m_cameraId);
        throw PlvRuntimeException(msg, __FILE__, __LINE__ );
    }
    m_camera->setDimensions(m_width, m_height);
}

void CameraProducer::deinit() throw()
{
    m_camera->release();
}

void CameraProducer::start()
{
    m_camera->start();
}

void CameraProducer::stop()
{
    m_camera->pause();
}

bool CameraProducer::isReadyForProcessing() const
{
    return( m_lastFrame.isNotNull() );
}
