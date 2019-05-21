#include "structure_camera.h"

SessionDelegate::SessionDelegate()
{
    _lastError = StructureCameraErrorCode::None;
}

SessionDelegate::~SessionDelegate() {}

void SessionDelegate::captureSessionEventDidOccur(ST::CaptureSession *session, ST::CaptureSessionEventId event)
{
    // wrong code
    if (event == ST::CaptureSessionEventId::Ready)
    {
        session->startStreaming();
    }
    else
    {
        _lastError = StructureCameraErrorCode::UnknownError;
    }
}

void SessionDelegate::captureSessionDidOutputSample(ST::CaptureSession *, const ST::CaptureSessionSample &sample)
{
    std::unique_lock<std::mutex> u(_sampleLock);
    // Depth/visible/infrared frames can arrive individually or as part of a SynchronizedFrames sample
    if (sample.depthFrame.isValid())
    {
        _lastDepthFrame = sample.depthFrame;
    }
    if (sample.visibleFrame.isValid())
    {
        _lastVisibleFrame = sample.visibleFrame;
    }
    if (sample.infraredFrame.isValid())
    {
        _lastInfraredFrame = sample.infraredFrame;
    }
    if (sample.type == ST::CaptureSessionSample::Type::AccelerometerEvent)
    {
        _lastAccelerometerEvent = sample.accelerometerEvent;
    }
    if (sample.type == ST::CaptureSessionSample::Type::GyroscopeEvent)
    {
        _lastGyroscopeEvent = sample.gyroscopeEvent;
    }
    u.unlock();
}

ST::DepthFrame SessionDelegate::lastDepthFrame()
{
    std::unique_lock<std::mutex> u(_sampleLock);
    return _lastDepthFrame;
}

ST::ColorFrame SessionDelegate::lastVisibleFrame()
{
    std::unique_lock<std::mutex> u(_sampleLock);
    return _lastVisibleFrame;
}

ST::InfraredFrame SessionDelegate::lastInfraredFrame()
{
    std::unique_lock<std::mutex> u(_sampleLock);
    return _lastInfraredFrame;
}

ST::AccelerometerEvent SessionDelegate::lastAccelerometerEvent()
{
    std::unique_lock<std::mutex> u(_sampleLock);
    return _lastAccelerometerEvent;
}

ST::GyroscopeEvent SessionDelegate::lastGyroscopeEvent()
{
    std::unique_lock<std::mutex> u(_sampleLock);
    return _lastGyroscopeEvent;
}

StructureCameraErrorCode SessionDelegate::lastError()
{
    std::unique_lock<std::mutex> u(_sampleLock);
    return _lastError;
}

StructureCamera::StructureCamera()
{
    _settings.source = ST::CaptureSessionSourceId::StructureCore;
    _settings.structureCore.depthEnabled = true;
    _settings.structureCore.visibleEnabled = true;
    _settings.structureCore.infraredEnabled = false;
    _settings.structureCore.accelerometerEnabled = false;
    _settings.structureCore.gyroscopeEnabled = false;
    _settings.structureCore.demosaicMethod = ST::StructureCoreDemosaicMethod::EdgeAware;
    _settings.structureCore.depthResolution = ST::StructureCoreDepthResolution::SXGA;
    _settings.structureCore.imuUpdateRate = ST::StructureCoreIMUUpdateRate::AccelAndGyro_200Hz;

    _session.setDelegate(&_delegate);
}

StructureCamera &StructureCamera::getInstance()
{
    static StructureCamera camera;
    return camera;
}

StructureCamera::~StructureCamera()
{
    _session.stopStreaming();
}

bool StructureCamera::start()
{
    return _session.startMonitoring(_settings);
}

void StructureCamera::stop()
{
    _session.stopStreaming();
}

ST::CaptureSessionSettings &StructureCamera::settings()
{
    return _settings;
}

bool StructureCamera::lastVisibleFrame(uint8_t *out)
{
    auto frame = _delegate.lastVisibleFrame();
    if (!frame.isValid())
        return false;

    memcpy(out, frame.rgbData(), sizeof(uint8_t) * frame.rgbSize());
    return true;
}

bool StructureCamera::lastDepthFrame(float *out)
{
    auto frame = _delegate.lastDepthFrame();
    if (!frame.isValid())
        return false;
    
    memcpy(out, frame.depthInMillimeters(), sizeof(float) * frame.width() * frame.height());
    return true;
}

void StructureCamera::visibleExposure(float seconds)
{
    float gain = visibleGain();
    _session.setVisibleCameraExposureAndGain(seconds, gain);
}

float StructureCamera::visibleExposure()
{
    float exposure;
    float gain;
    _session.getVisibleCameraExposureAndGain(&exposure, &gain);
    return exposure;
}

void StructureCamera::visibleGain(float multiplier) // from 1 to 8
{
    float exposure = visibleExposure();
    _session.setVisibleCameraExposureAndGain(exposure, multiplier);
}

float StructureCamera::visibleGain()
{
    float exposure;
    float gain;
    _session.getVisibleCameraExposureAndGain(&exposure, &gain);
    return gain;
}

void StructureCamera::infraredExposure(float seconds)
{
    float gain = infraredGain();
    _session.setInfraredCamerasExposureAndGain(seconds, gain);
}

float StructureCamera::infraredExposure()
{
    float exposure;
    float gain;
    _session.getInfraredCamerasExposureAndGain(&exposure, &gain);
    return exposure;
}

void StructureCamera::infraredGain(float multiplier) // from 0 to 3
{
    float exposure = infraredExposure();
    _session.setInfraredCamerasExposureAndGain(exposure, multiplier);
}

float StructureCamera::infraredGain()
{
    float exposure;
    float gain;
    _session.getInfraredCamerasExposureAndGain(&exposure, &gain);
    return gain;
}

// ================ API =================

bool startCamera()
{
    auto camera = &StructureCamera::getInstance();
    return camera->start();
}

void stopCamera()
{
    auto camera = &StructureCamera::getInstance();
    return camera->stop();
}

bool lastDepthFrame(float *out)
{
    auto camera = &StructureCamera::getInstance();
    return camera->lastDepthFrame(out);
}

bool lastVisibleFrame(uint8_t *out)
{
    auto camera = &StructureCamera::getInstance();
    return camera->lastVisibleFrame(out);
}

void setVisibleExposure(float seconds)
{
    auto camera = &StructureCamera::getInstance();
    camera->visibleExposure(seconds);
}

float getVisibleExposure()
{
    auto camera = &StructureCamera::getInstance();
    return camera->visibleExposure();
}

void setVisibleGain(float multiplier) // from 1 to 8
{
    auto camera = &StructureCamera::getInstance();
    camera->visibleGain(multiplier);
}

float getVisibleGain()
{
    auto camera = &StructureCamera::getInstance();
    return camera->visibleGain();
}

void setInfraredExposure(float seconds)
{
    auto camera = &StructureCamera::getInstance();
    camera->infraredExposure(seconds);
}

float getInfraredExposure()
{
    auto camera = &StructureCamera::getInstance();
    return camera->infraredExposure();
}

void setInfraredGain(float multiplier) // from 0 to 3
{
    auto camera = &StructureCamera::getInstance();
    camera->infraredGain(multiplier);
}

float getInfraredGain()
{
    auto camera = &StructureCamera::getInstance();
    return camera->infraredGain();
}

void LIB_API setDepthResolution(int resolution)
{
    auto camera = &StructureCamera::getInstance();
    camera->settings().structureCore.depthResolution = (ST::StructureCoreDepthResolution)resolution;
}

int LIB_API getDepthResolution()
{
    auto camera = &StructureCamera::getInstance();
    return (int)camera->settings().structureCore.depthResolution;
}

void LIB_API setDepthRange(int mode)
{
    auto camera = &StructureCamera::getInstance();
    camera->settings().structureCore.depthRangeMode = (ST::StructureCoreDepthRangeMode)mode;
}

int LIB_API getDepthRange()
{
    auto camera = &StructureCamera::getInstance();
    return (int)camera->settings().structureCore.depthRangeMode;
}

void LIB_API setCalibrationMode(int mode)
{
    auto camera = &StructureCamera::getInstance();
    camera->settings().structureCore.dynamicCalibrationMode = (ST::StructureCoreDynamicCalibrationMode)mode;
}

int LIB_API getCalibrationMode()
{
    auto camera = &StructureCamera::getInstance();
    return (int)camera->settings().structureCore.dynamicCalibrationMode;
}

void LIB_API setDepthCorrection(bool value)
{
    auto camera = &StructureCamera::getInstance();
    camera->settings().applyExpensiveCorrection = value;
}

bool LIB_API getDepthCorrection()
{
    auto camera = &StructureCamera::getInstance();
    return camera->settings().applyExpensiveCorrection;
}

void LIB_API setInfraredAutoExposure(bool value)
{
    auto camera = &StructureCamera::getInstance();
    camera->settings().structureCore.infraredAutoExposureEnabled = value;
}

bool LIB_API getInfraredAutoExposure()
{
    auto camera = &StructureCamera::getInstance();
    return camera->settings().structureCore.infraredAutoExposureEnabled;
}

void LIB_API setGammaCorrection(bool value)
{
    auto camera = &StructureCamera::getInstance();
    camera->settings().structureCore.visibleApplyGammaCorrection = value;
}

bool LIB_API getGammaCorrection()
{
    auto camera = &StructureCamera::getInstance();
    return camera->settings().structureCore.visibleApplyGammaCorrection;
}