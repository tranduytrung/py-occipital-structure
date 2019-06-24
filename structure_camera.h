#include <mutex>
#include <ST/CaptureSession.h>

#pragma once

enum StructureCameraErrorCode {
    None, UnknownError
};

class SessionDelegate : public ST::CaptureSessionDelegate
{
private:
    /*
          The mutex synchronizes access to the stored CaptureSession samples and
          rate monitors, since they may be accessed from:

          - The GL rendering thread when the owning Window retrieves them to display
            in the user interface
          - The CaptureSession's internal frame delivery thread(s)
          - The CaptureSession's dedicated IMU sample delivery thread if
            CaptureSessionSettings::lowLatencyIMU is enabled (default)
        */
    std::mutex _sampleLock;

    ST::DepthFrame _lastDepthFrame;
    ST::ColorFrame _lastVisibleFrame;
    ST::InfraredFrame _lastInfraredFrame;
    ST::AccelerometerEvent _lastAccelerometerEvent;
    ST::GyroscopeEvent _lastGyroscopeEvent;

    StructureCameraErrorCode _lastError;

public:
    explicit SessionDelegate();
    ~SessionDelegate();

    // Thread-safe accessors for the stored CaptureSession samples
    ST::DepthFrame lastDepthFrame();
    ST::ColorFrame lastVisibleFrame();
    ST::InfraredFrame lastInfraredFrame();
    ST::AccelerometerEvent lastAccelerometerEvent();
    ST::GyroscopeEvent lastGyroscopeEvent();
    StructureCameraErrorCode lastError();

    // CaptureSession callbacks
    void captureSessionEventDidOccur(ST::CaptureSession *, ST::CaptureSessionEventId) override;
    void captureSessionDidOutputSample(ST::CaptureSession *, const ST::CaptureSessionSample &) override;
};

class StructureCamera
{
private:
    StructureCamera();
    ST::CaptureSession _session;
    ST::CaptureSessionSettings _settings;
    SessionDelegate _delegate;

public:
    ~StructureCamera();
    StructureCamera(StructureCamera const&) = delete;
    void operator=(StructureCamera const&) = delete;
    static StructureCamera & getInstance();

    bool start();
    void stop();
    ST::CaptureSessionSettings & settings();
    bool lastDepthFrame(float *out);
    bool lastVisibleFrame(uint8_t *out);
    bool lastInfraredFrame(uint16_t *out);

    void visibleExposure(float seconds);
    float visibleExposure();
    void visibleGain(float multiplier); // from 1 to 8
    float visibleGain();
    void infraredExposure(float seconds);
    float infraredExposure();
    void infraredGain(float multiplier); // from 0 to 3
    float infraredGain();
};

#define LIB_API __declspec(dllexport)

extern "C" {
    bool LIB_API startCamera();
    void LIB_API stopCamera();
    bool LIB_API lastDepthFrame(float *out);
    bool LIB_API lastVisibleFrame(uint8_t *out);
    bool LIB_API lastInfraredFrame(uint16_t *out);

    void LIB_API setVisibleExposure(float seconds);
    float LIB_API getVisibleExposure();
    void LIB_API setVisibleGain(float multiplier); // from 1 to 8
    float LIB_API getVisibleGain();
    void LIB_API setInfraredExposure(float seconds);
    float LIB_API getInfraredExposure();
    void LIB_API setInfraredGain(float multiplier); // from 0 to 3
    float LIB_API getInfraredGain();

    extern const int LIB_API SC_RESOLUTION_VGA = 1;
    extern const int LIB_API SC_RESOLUTION_SXGA = 2;
    void LIB_API setDepthResolution(int resolution);
    int LIB_API getDepthResolution();

    extern const LIB_API int SC_DEPTH_RANGE_VERY_SHORT = 0; /** Estimated range of 0.35m to 0.92m */
    extern const LIB_API int SC_DEPTH_RANGE_SHORT = 1; /** Estimated range of 0.41m to 1.36m */
    extern const LIB_API int SC_DEPTH_RANGE_MEDIUM = 2; /** Estimated range of 0.52m to 5.23m */
    extern const LIB_API int SC_DEPTH_RANGE_LONG = 3; /** Estimated range of 0.58m to 8.0m */
    extern const LIB_API int SC_DEPTH_RANGE_VERY_LONG = 4; /** Estimated range of 0.58m to 10.0m */
    extern const LIB_API int SC_DEPTH_RANGE_HYBRID = 5; /** Estimated range of 0.35m to 10.0m */
    extern const LIB_API int SC_DEPTH_RANGE_DEFAULT = 6; /** Don't use a preset, use the provided configuration options. */
    void LIB_API setDepthRange(int mode);
    int LIB_API getDepthRange();

    /** No dynamic calibration is performed. If misalignment of the stereo
        infrared cameras exceeds 0.1 mm, the quality of depth data may be
        reduced. */
    extern const LIB_API int SC_CALIBRATION_OFF = 0;
    /** A single dynamic calibration cycle is performed when depth streaming
        starts. This mode is sufficient to correct for misalignment caused by
        static mechanical stress, such as that caused by attaching the sensor to
        a fixture which is not perfectly aligned to the body of the sensor. Once
        a suitable calibration is determined, it is stored persistently on the
        sensor. If the dynamic calibration matches the exsting calibration
        stored on the sensor (if any), no update is performed. */
    extern const LIB_API int SC_CALIBRATION_ONESHOT = 1;
    /** Dynamic calibration is performed continuously as the sensor streams
        depth data. This mode should be employed in cases where the sensor is
        subject to continuously varying mechanical stresses such as vibration,
        thermal expansion/contraction, etc. The calibration is not stored on the
        sensor in this mode. */
    extern const LIB_API int SC_CALIBRATION_CONTINUOUS = 2;
    void LIB_API setCalibrationMode(int mode);
    int LIB_API getCalibrationMode();

    void LIB_API setDepthCorrection(bool value);
    bool LIB_API getDepthCorrection();

    void LIB_API setInfraredAutoExposure(bool value);
    bool LIB_API getInfraredAutoExposure();

    void LIB_API setGammaCorrection(bool value);
    bool LIB_API getGammaCorrection();

    void LIB_API setVisibleEnabled(bool value);
    bool LIB_API getVisibleEnabled();

    void LIB_API setInfraredEnabled(bool value);
    bool LIB_API getInfraredEnabled();

    extern const LIB_API int SC_INFRARED_MODE_LEFT = 0;
    extern const LIB_API int SC_INFRARED_MODE_RIGHT = 1;
    extern const LIB_API int SC_INFRARED_MODE_RIGHTLEFT = 2;
    void LIB_API setInfraredMode(int value);
    int LIB_API getInfraredMode();
}