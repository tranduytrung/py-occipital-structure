import ctypes, os
import numpy as np

os.environ['PATH'] = os.path.dirname(__file__) + ';' + os.environ['PATH']
class StructureCamera:
    __SC = ctypes.cdll.LoadLibrary('structure_camera')

    __start_camera = __SC["startCamera"]
    __start_camera.restype = ctypes.c_bool

    __stop_camera = __SC["stopCamera"]
    __started = False

    __last_visible_frame = __SC["lastVisibleFrame"]
    __last_visible_frame.argtypes = [ctypes.POINTER(ctypes.c_uint8)]
    __last_visible_frame.restype = ctypes.c_bool
    VisibleVGA = ctypes.c_uint8*(640*480*3)

    __last_depth_frame = __SC["lastDepthFrame"]
    __last_depth_frame.argtypes = [ctypes.POINTER(ctypes.c_float)]
    __last_depth_frame.restype = ctypes.c_bool
    DepthVGA = ctypes.c_float*(640*480)
    DepthSXGA = ctypes.c_float*(1280*960)

    __last_infrared_frame = __SC["lastInfraredFrame"]
    __last_infrared_frame.argtypes = [ctypes.POINTER(ctypes.c_uint16)]
    __last_infrared_frame.restype = ctypes.c_bool
    InfraredSXGA = ctypes.c_uint16*(1216*912)
    InfraredSXGADual = ctypes.c_uint16*(1216*912*2)

    __get_visible_exposure = __SC["getVisibleExposure"]
    __get_visible_exposure.restype = ctypes.c_float
    __set_visible_exposure = __SC["setVisibleExposure"]
    __set_visible_exposure.argtypes = [ctypes.c_float]

    __get_visible_gain = __SC["getVisibleGain"]
    __get_visible_gain.restype = ctypes.c_float
    __set_visible_gain = __SC["setVisibleGain"]
    __set_visible_gain.argtypes = [ctypes.c_float]

    __get_infrared_exposure = __SC["getInfraredExposure"]
    __get_infrared_exposure.restype = ctypes.c_float
    __set_infrared_exposure = __SC["setInfraredExposure"]
    __set_infrared_exposure.argtypes = [ctypes.c_float]

    __get_infrared_gain = __SC["getInfraredGain"]
    __get_infrared_gain.restype = ctypes.c_float
    __set_infrared_gain = __SC["setInfraredGain"]
    __set_infrared_gain.argtypes = [ctypes.c_float]

    SC_RESOLUTION_VGA = ctypes.c_int.in_dll(__SC, 'SC_RESOLUTION_VGA').value
    SC_RESOLUTION_SXGA = ctypes.c_int.in_dll(__SC, 'SC_RESOLUTION_SXGA').value
    __get_depth_resolution = __SC["getDepthResolution"]
    __get_depth_resolution.restype = ctypes.c_int
    __set_depth_resolution = __SC["setDepthResolution"]
    __set_depth_resolution.argtypes = [ctypes.c_int]

    """ Estimated range of 0.35m to 0.92m """
    SC_DEPTH_RANGE_VERY_SHORT = ctypes.c_int.in_dll(
        __SC, 'SC_DEPTH_RANGE_VERY_SHORT').value

    """ Estimated range of 0.41m to 1.36m """
    SC_DEPTH_RANGE_SHORT = ctypes.c_int.in_dll(
        __SC, 'SC_DEPTH_RANGE_SHORT').value
    """ Estimated range of 0.52m to 5.23m  """
    SC_DEPTH_RANGE_MEDIUM = ctypes.c_int.in_dll(
        __SC, 'SC_DEPTH_RANGE_MEDIUM').value
    """ Estimated range of 0.58m to 8.0m """
    SC_DEPTH_RANGE_LONG = ctypes.c_int.in_dll(
        __SC, 'SC_DEPTH_RANGE_LONG').value
    """ Estimated range of 0.58m to 10.0m """
    SC_DEPTH_RANGE_VERY_LONG = ctypes.c_int.in_dll(
        __SC, 'SC_DEPTH_RANGE_VERY_LONG').value
    """ Estimated range of 0.35m to 10.0m """
    SC_DEPTH_RANGE_HYBRID = ctypes.c_int.in_dll(
        __SC, 'SC_DEPTH_RANGE_HYBRID').value
    """ Don't use a preset, use the provided configuration options. """
    SC_DEPTH_RANGE_DEFAULT = ctypes.c_int.in_dll(
        __SC, 'SC_DEPTH_RANGE_DEFAULT').value
    __get_depth_range = __SC["getDepthRange"]
    __get_depth_range.restype = ctypes.c_int
    __set_depth_range = __SC["setDepthRange"]
    __set_depth_range.argtypes = [ctypes.c_int]

    SC_CALIBRATION_OFF = ctypes.c_int.in_dll(__SC, 'SC_CALIBRATION_OFF').value
    SC_CALIBRATION_ONESHOT = ctypes.c_int.in_dll(
        __SC, 'SC_CALIBRATION_ONESHOT').value
    SC_CALIBRATION_CONTINUOUS = ctypes.c_int.in_dll(
        __SC, 'SC_CALIBRATION_CONTINUOUS').value
    __get_calibration_mode = __SC["getCalibrationMode"]
    __get_calibration_mode.restype = ctypes.c_int
    __set_calibration_mode = __SC["setCalibrationMode"]
    __set_calibration_mode.argtypes = [ctypes.c_int]

    __get_depth_correction = __SC["getDepthCorrection"]
    __get_depth_correction.restype = ctypes.c_bool
    __set_depth_correction = __SC["setDepthCorrection"]
    __set_depth_correction.argtypes = [ctypes.c_bool]

    __get_infrared_auto_exposure = __SC["getInfraredAutoExposure"]
    __get_infrared_auto_exposure.restype = ctypes.c_bool
    __set_infrared_auto_exposure = __SC["setInfraredAutoExposure"]
    __set_infrared_auto_exposure.argtypes = [ctypes.c_bool]

    __get_gamma_correction = __SC["getGammaCorrection"]
    __get_gamma_correction.restype = ctypes.c_bool
    __set_gamma_correction = __SC["setGammaCorrection"]
    __set_gamma_correction.argtypes = [ctypes.c_bool]

    __get_visible_enabled = __SC["getVisibleEnabled"]
    __get_visible_enabled.restype = ctypes.c_bool
    __set_visible_enabled = __SC["setVisibleEnabled"]
    __set_visible_enabled.argtypes = [ctypes.c_bool]

    __get_infrared_enabled = __SC["getInfraredEnabled"]
    __get_infrared_enabled.restype = ctypes.c_bool
    __set_infrared_enabled = __SC["setInfraredEnabled"]
    __set_infrared_enabled.argtypes = [ctypes.c_bool]

    SC_INFRARED_MODE_LEFT = ctypes.c_int.in_dll(__SC, 'SC_INFRARED_MODE_LEFT').value
    SC_INFRARED_MODE_RIGHT = ctypes.c_int.in_dll(__SC, 'SC_INFRARED_MODE_RIGHT').value
    SC_INFRARED_MODE_RIGHTLEFT = ctypes.c_int.in_dll(__SC, 'SC_INFRARED_MODE_RIGHTLEFT').value

    __get_infrared_mode = __SC["getInfraredMode"]
    __get_infrared_mode.restype = ctypes.c_int
    __set_infrared_mode = __SC["setInfraredMode"]
    __set_infrared_mode.argtypes = [ctypes.c_int]

    def __init__(self):
        pass

    @property
    def started(self):
        return StructureCamera.__started

    def __assert_started(self):
        assert self.started, "Cannot execute this operation when the camera has not been streaming"

    def __assert_not_started(self):
        assert not self.started, "Cannot execute this operation when the camera has been streaming"

    def start(self):
        self.__assert_not_started()
        StructureCamera.__started = StructureCamera.__start_camera()
        return StructureCamera.__started

    def stop(self):
        # self.__assert_started()
        StructureCamera.__started = StructureCamera.__stop_camera()

    def __enter__(self):
        self.start()
        return self

    def __exit__(self, type, value, tb):
        self.stop()

    def last_visible_frame(self):
        self.__assert_started()
        visible_data = StructureCamera.VisibleVGA()
        StructureCamera.__last_visible_frame(ctypes.cast(
            visible_data, ctypes.POINTER(ctypes.c_uint8)))
        np_visible = np.array(visible_data).reshape((480, 640, 3))
        return np_visible

    def last_depth_frame(self):
        if self.depth_resolution == StructureCamera.SC_RESOLUTION_SXGA:
            depth_data = StructureCamera.DepthSXGA()
            data_shape = (960, 1280)
        elif self.depth_resolution == StructureCamera.SC_RESOLUTION_VGA:
            depth_data = StructureCamera.DepthVGA()
            data_shape = (480, 640)
        else:
            raise Exception('not supported resolution')

        StructureCamera.__last_depth_frame(ctypes.cast(
            depth_data, ctypes.POINTER(ctypes.c_float)))
        np_depth = np.array(depth_data).reshape(data_shape)
        return np_depth

    def last_infrared_frame(self):
        if self.infrared_mode == StructureCamera.SC_INFRARED_MODE_RIGHTLEFT:
            depth_data = StructureCamera.InfraredSXGADual()
            data_shape = (912, 1216*2)
        else:
            depth_data = StructureCamera.InfraredSXGA()
            data_shape = (912, 1216)

        StructureCamera.__last_infrared_frame(ctypes.cast(
            depth_data, ctypes.POINTER(ctypes.c_uint16)))
        np_depth = np.array(depth_data).reshape(data_shape)
        return np_depth

    @property
    def visible_exposure(self):
        return StructureCamera.__get_visible_exposure()

    @visible_exposure.setter
    def visible_exposure(self, value):
        StructureCamera.__set_visible_exposure(value)

    @property
    def visible_gain(self):
        return StructureCamera.__get_visible_gain()

    @visible_gain.setter
    def visible_gain(self, value):
        StructureCamera.__set_visible_gain(value)

    @property
    def infrared_exposure(self):
        return StructureCamera.__get_infrared_exposure()

    @infrared_exposure.setter
    def infrared_exposure(self, value):
        StructureCamera.__set_infrared_exposure(value)

    @property
    def infrared_gain(self):
        return StructureCamera.__get_infrared_gain()

    @infrared_gain.setter
    def infrared_gain(self, value):
        StructureCamera.__set_infrared_gain(value)

    @property
    def depth_resolution(self):
        return StructureCamera.__get_depth_resolution()

    @depth_resolution.setter
    def depth_resolution(self, value):
        self.__assert_not_started()
        StructureCamera.__set_depth_resolution(value)

    @property
    def depth_range(self):
        return StructureCamera.__get_depth_range()

    @depth_range.setter
    def depth_range(self, value):
        self.__assert_not_started()
        StructureCamera.__set_depth_range(value)

    @property
    def calibration_mode(self):
        return StructureCamera.__get_calibration_mode()

    @calibration_mode.setter
    def calibration_mode(self, value):
        self.__assert_not_started()
        StructureCamera.__set_calibration_mode(value)

    @property
    def depth_correction(self):
        return StructureCamera.__get_depth_correction()

    @depth_correction.setter
    def depth_correction(self, value):
        self.__assert_not_started()
        StructureCamera.__set_depth_correction(value)

    @property
    def gamma_correction(self):
        return StructureCamera.__get_gamma_correction()

    @gamma_correction.setter
    def gamma_correction(self, value):
        self.__assert_not_started()
        StructureCamera.__set_gamma_correction(value)

    @property
    def infrared_auto_exposure(self):
        return StructureCamera.__get_infrared_auto_exposure()

    @infrared_auto_exposure.setter
    def infrared_auto_exposure(self, value):
        self.__assert_not_started()
        StructureCamera.__set_infrared_auto_exposure(value)

    @property
    def visible_enabled(self):
        return StructureCamera.__get_visible_enabled()

    @visible_enabled.setter
    def visible_enabled(self, value):
        self.__assert_not_started()
        StructureCamera.__set_visible_enabled(value)

    @property
    def infrared_enabled(self):
        return StructureCamera.__get_infrared_enabled()

    @infrared_enabled.setter
    def infrared_enabled(self, value):
        self.__assert_not_started()
        StructureCamera.__set_infrared_enabled(value)

    @property
    def infrared_mode(self):
        return StructureCamera.__get_infrared_mode()

    @infrared_mode.setter
    def infrared_mode(self, value):
        self.__assert_not_started()
        StructureCamera.__set_infrared_mode(value)

def normalize_minmax(frame, min_value, max_value):
    return (frame - min_value) / (max_value - min_value)


def clip_and_fill(frame, min_value, max_value, fill_value="uniform"):
    nan_mask = np.isnan(frame)
    if fill_value == "uniform":
        fill_value = np.random.uniform(
            min_value, max_value, size=np.sum(nan_mask))
    elif fill_value == "normal":
        mean = (min_value + max_value) / 2
        std = (max_value - mean) / 4 # since 2 std = 98% of coverage
        fill_value = np.random.normal(
            mean, std, size=np.sum(nan_mask))

    frame[nan_mask] = fill_value
    clipped = np.clip(frame, min_value, max_value)
    return clipped


def crop_frame(frame, ratio=0.75):
    h = frame.shape[0]
    w = frame.shape[1]
    dw = w - int(w * ratio)
    dh = h - int(h * ratio)
    cropped = frame[dh//2:-dh//2, dw//2:-dw//2]
    return cropped

if __name__ == "__main__":
    import cv2
    import time
    sc = StructureCamera()
    sc.depth_correction = False
    sc.infrared_auto_exposure = False
    sc.gamma_correction = True
    sc.calibration_mode = sc.SC_CALIBRATION_ONESHOT
    sc.depth_range = sc.SC_DEPTH_RANGE_VERY_SHORT
    sc.depth_resolution = sc.SC_RESOLUTION_VGA
    sc.start()

    while True:
        rgb_frame = sc.last_visible_frame()
        bgr_frame = cv2.cvtColor(rgb_frame, cv2.COLOR_RGB2BGR)
        cv2.imshow('rgb_frame', bgr_frame)
        depth_frame = sc.last_depth_frame()
        depth_frame = normalize_minmax(
            clip_and_fill(depth_frame, 3e2, 15e2, 'normal'), 3e2, 15e2)
        cv2.imshow('depth_frame', depth_frame)

        key = cv2.waitKey(50)
        if key == ord('0'):
            sc.infrared_gain = 0
        if key == ord('1'):
            sc.infrared_gain = 1
        if key == ord('2'):
            sc.infrared_gain = 2
        if key == ord('3'):
            sc.infrared_gain = 3
        if key == ord('q'):
            sc.infrared_exposure = sc.infrared_exposure - 0.001
        if key == ord('w'):
            sc.infrared_exposure = sc.infrared_exposure + 0.001
        if key == 27:  # Esc
            break

    sc.stop()
    cv2.destroyAllWindows()
