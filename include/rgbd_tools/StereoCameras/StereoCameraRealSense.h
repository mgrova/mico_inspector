////////////////////////////////////////////////////////////////
//															  //
//		RGB-D Slam and Active Perception Project			  //
//															  //
//				Author: Pablo R.S. (aka. Bardo91)			  //
//															  //
////////////////////////////////////////////////////////////////

#ifndef RGBDSLAM_VISION_STEREOCAMERAS_STEREOCAMERAREALSENSE_H_
#define RGBDSLAM_VISION_STEREOCAMERAS_STEREOCAMERAREALSENSE_H_

#include <rgbd_tools/StereoCamera.h>

#ifdef ENABLE_LIBREALSENSE_V1
    #include <librealsense/rs.hpp>
#endif

#ifdef ENABLE_LIBREALSENSE_V2
    #include <librealsense2/rs.hpp>
#endif

// Forward declarations
namespace rs {
	class context;
	class device;

	struct extrinsics;
	struct intrinsics;
}

namespace rgbd {
	/// Wrapper for IntelReal Sense 3D camera.
	class StereoCameraRealSense :public StereoCamera {
	public:		// Public interface
        /// \brief Initialize the camera using a config file. The coordinate system for the 3d is always on the color camera.
		/// Config file must have following structure.
		///
		/// \code
		///     {
		///			"deviceId":0	// integer
		///			"syncStreams":true|false,
		///			"depth":
		///				{
		///					"resolution":640,	// Desired resolution, if didn't exist, take the closer one
		///					"fps":30			// Desired fps, if didn't exist, take the closer one
		///				},
		///			"rgb":
		///				{
		///					"resolution":640,	// Desired resolution, if didn't exist, take the closer one
		///					"fps":30			// Desired fps, if didn't exist, take the closer one
		///				}	
		///			"useUncolorizedPoints":true|false,	// The FOV of the Depth camera is larger than the color one.
		///												// Set true to use them in the color cloud even it they dont 
		///												// color, or set it to false to disable them and take only the
		///												// colorized points.
		///			"cloudDownsampleStep":1				// During the creation of the point cloud this step is used for 
		///												// iterating over depth pixels. If step is 1, then all point cloud
		///												// is reconstructed. If larger, the a number of points equal to step-1
		///												// are skipped. This is usefull if you are considering to downsample
		///												// the cloud as it is faster than postprocessing it.
		///			"others":
		///				{
		///					"r200_lr_autoexposure":true|false;
		///					"r200_emitter_enabled":true|false;
		///					"r200_lr_exposure":1 to 164 (int, default: 164);
		///					"r200_lr_gain":100 to 6399 (int, default 400);
		///				}
		///     }
		/// \endcode
		/// \param _filePath: path to the file.
		bool init(const cjson::Json &_json = "");

		/// \brief get the rgb frame and fill only the left image.
		bool rgb(cv::Mat &_left, cv::Mat &_right);

		/// \brief get the depth frame generated by the IR sensors. 
		bool depth(cv::Mat &_depth);

		/// \brief Grab current data from camera to make sure that is synchronized.
		bool grab();

		/// \brief Get a new point cloud from the camera with only spatial information.
		/// \param _cloud: reference to a container for the point cloud.
		bool cloud(pcl::PointCloud<pcl::PointXYZ> &_cloud);

		/// \brief Get a new point cloud from the camera with spatial and RGB information.
		/// \param _cloud: reference to a container for the point cloud.
		bool cloud(pcl::PointCloud<pcl::PointXYZRGB> &_cloud);

		/// \brief Get a new point cloud from the camera with spatial, surface normals and RGB information.
		/// \param _cloud: reference to a container for the point cloud.
		bool cloud(pcl::PointCloud<pcl::PointXYZRGBNormal> &_cloud);

		/// \brief Get a new point cloud from the camera with spatial information and surface normals.
		/// \param _cloud: reference to a container for the point cloud.
		bool cloud(pcl::PointCloud<pcl::PointNormal> &_cloud);

        /// \brief templatized method to define the interface for retrieving a new point cloud from the camera with
        /// spatial, surface normals and RGB information (custom types out of PCL).
        /// \param _cloud: reference to a container for the point cloud.
        template<typename PointType_>
        bool cloud(pcl::PointCloud<PointType_> &_cloud);

        /// \brief get the calibration matrices of the left camera in opencv format. Matrices are CV_32F.
        virtual bool leftCalibration(cv::Mat &_intrinsic, cv::Mat &_coefficients);

        /// \brief get the calibration matrices of the depth (IR) camera in opencv format. Matrices are CV_32F.
        virtual bool rightCalibration(cv::Mat &_intrinsic, cv::Mat &_coefficients);

        /// \brief get the extrinsic matrices, i.e., transformation from left to depth (IR) camera. Matrices are CV_32F.
        virtual bool extrinsic(cv::Mat &_rotation, cv::Mat &_translation);

        /// \brief get the extrinsic matrices, i.e., transformation from left to depth (IR) camera.
        virtual bool extrinsic(Eigen::Matrix3f &_rotation, Eigen::Vector3f &_translation);

        /// \brief get disparty-to-depth parameter typical from RGB-D devices.
        virtual bool disparityToDepthParam(double &_dispToDepth);

        bool colorPixelToPoint(const cv::Point2f &_pixel, cv::Point3f &_point);

		/// \brief Changes the IR laser power level, 0 to turn off
		/// \param power_level: laser power level, min is 0 (off) and max is 16
		bool laserPower(double power_level);

	private:	// Private members
		cjson::Json mConfig;

		int mDownsampleStep = 1;

        #if defined(ENABLE_LIBREALSENSE_V1)
            rs::context *mRsContext;
            rs::device *mRsDevice;

            rs::intrinsics mRsColorIntrinsic, mRsDepthIntrinsic;
            rs::extrinsics mRsDepthToColor, mRsColorToDepth;
        #elif defined(ENABLE_LIBREALSENSE_V2)
            rs2::context mRsContext;
            rs2::device mRsDevice;

            rs2::pipeline mRsPipe;
            rs2::pipeline_profile mRsPipeProfile;
            rs2::align *mRsAlign;


            rs2_intrinsics mRsColorIntrinsic, mRsDepthIntrinsic;
            rs2_extrinsics mRsDepthToColor, mRsColorToDepth;
        #endif // ENABLE_LIBREALSENSE
        cv::Mat mCvColorIntrinsic;
        cv::Mat mCvDepthIntrinsic;

		float mRsDepthScale;

        cv::Mat mExtrinsicColorToDepth; // 4x4 transformation between color and depth camera

		bool mUseUncolorizedPoints = false;
		bool mHasRGB = false, mComputedDepth = false;
        cv::Mat mLastRGB, mLastDepthInColor;
		int mDeviceId = 0;

    private:	//	Private interface
        template<typename PointType_>
        bool setOrganizedAndDense(pcl::PointCloud<PointType_> &_cloud);

        #if defined(ENABLE_LIBREALSENSE_V1)
            cv::Point distortPixel(const cv::Point &_point, const rs::intrinsics &_intrinsics) const;
            cv::Point undistortPixel(const cv::Point &_point,  const rs::intrinsics &_intrinsics) const;
        #elif defined(ENABLE_LIBREALSENSE_V2)
            cv::Point distortPixel(const cv::Point &_point, const rs2_intrinsics &_intrinsics) const;
            cv::Point undistortPixel(const cv::Point &_point,  const rs2_intrinsics &_intrinsics) const;
        #endif
        cv::Point3f deproject(const cv::Point &_point, const float _depth) const;
	};	//	class StereoCameraRealSense


	template<typename PointType_>
	inline bool StereoCameraRealSense::setOrganizedAndDense(pcl::PointCloud<PointType_> &_cloud) {
		if (mHasRGB && mComputedDepth) {
            _cloud.is_dense = false; // 666 TODO: cant set to true if wrong points are set to NaN.
            _cloud.width = mLastRGB.cols/ mDownsampleStep;
            _cloud.height = mLastRGB.rows/ mDownsampleStep;
			return true;
		}
		else {
			return false;
		}
	}

}	//	namespace rgbd

#include <rgbd_tools/StereoCameras/StereoCameraRealSense.inl>

#endif  // RGBDSLAM_VISION_STEREOCAMERAS_STEREOCAMERAREALSENSE_H_
