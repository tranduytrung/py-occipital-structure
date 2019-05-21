#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <thread>

#define LIB_API __declspec(dllimport)

extern "C" {
    bool LIB_API startCamera();
    void LIB_API stopCamera();
    bool LIB_API lastDepthFrame(float *out);
    bool LIB_API lastVisibleFrame(uint8_t *out);

    void LIB_API setVisibleExposure(float seconds);
    float LIB_API getVisibleExposure();
    void LIB_API setVisibleGain(float multiplier); // from 1 to 8
    float LIB_API getVisibleGain();
    void LIB_API setInfraredExposure(float seconds);
    float LIB_API getInfraredExposure();
    void LIB_API setInfraredGain(float multiplier); // from 0 to 3
    float LIB_API getInfraredGain();

    extern const int LIB_API SC_RESOLUTION_VGA;
    extern const int LIB_API SC_RESOLUTION_SXGA;
    void LIB_API setDepthResolution(int resolution);
    int LIB_API getDepthResolution();

    extern const LIB_API int SC_DEPTH_RANGE_VERY_SHORT; /** Estimated range of 0.35m to 0.92m */
    extern const LIB_API int SC_DEPTH_RANGE_SHORT; /** Estimated range of 0.41m to 1.36m */
    extern const LIB_API int SC_DEPTH_RANGE_MEDIUM; /** Estimated range of 0.52m to 5.23m */
    extern const LIB_API int SC_DEPTH_RANGE_LONG; /** Estimated range of 0.58m to 8.0m */
    extern const LIB_API int SC_DEPTH_RANGE_VERY_LONG; /** Estimated range of 0.58m to 10.0m */
    extern const LIB_API int SC_DEPTH_RANGE_HYBRID; /** Estimated range of 0.35m to 10.0m */
    extern const LIB_API int SC_DEPTH_RANGE_DEFAULT; /** Don't use a preset, use the provided configuration options. */
    void LIB_API setDepthRange(int mode);
    int LIB_API getDepthRange();

    /** No dynamic calibration is performed. If misalignment of the stereo
        infrared cameras exceeds 0.1 mm, the quality of depth data may be
        reduced. */
    extern const LIB_API int SC_CALIBRATION_OFF;
    /** A single dynamic calibration cycle is performed when depth streaming
        starts. This mode is sufficient to correct for misalignment caused by
        static mechanical stress, such as that caused by attaching the sensor to
        a fixture which is not perfectly aligned to the body of the sensor. Once
        a suitable calibration is determined, it is stored persistently on the
        sensor. If the dynamic calibration matches the exsting calibration
        stored on the sensor (if any), no update is performed. */
    extern const LIB_API int SC_CALIBRATION_ONESHOT;
    /** Dynamic calibration is performed continuously as the sensor streams
        depth data. This mode should be employed in cases where the sensor is
        subject to continuously varying mechanical stresses such as vibration,
        thermal expansion/contraction, etc. The calibration is not stored on the
        sensor in this mode. */
    extern const LIB_API int SC_CALIBRATION_CONTINUOUS;
    void LIB_API setCalibrationMode(int mode);
    int LIB_API getCalibrationMode();

    void LIB_API setDepthCorrection(bool value);
    bool LIB_API getDepthCorrection();

    void LIB_API setInfraredAutoExposure(bool value);
    bool LIB_API getInfraredAutoExposure();

    void LIB_API setGammaCorrection(bool value);
    bool LIB_API getGammaCorrection();
}

void main()
{
    setDepthResolution(2);
    bool ret = startCamera();
    if (!ret)
    {
        printf("Failed to initialize capture session!\n");
        exit(1);
    }
    printf("Successed to initialize capture session!\n");

    float * depth = new float[1280*960];
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (lastDepthFrame(depth)) {
            printf("%f", depth[0]);
        }
    }

    delete [] depth;
}