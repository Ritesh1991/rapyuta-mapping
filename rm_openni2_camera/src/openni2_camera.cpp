/*
 * openni2_camera.cpp
 *
 *  Created on: Apr 24, 2013
 *      Author: vsu
 */

#include <openni2_camera.h>

OpenNI2Camera::OpenNI2Camera(ros::NodeHandle & nh) {

	Status rc = OpenNI::initialize();
	if (rc != STATUS_OK) {
		printf("Initialize failed\n%s\n", OpenNI::getExtendedError());
		exit(1);
	}

	rc = device.open(ANY_DEVICE);
	if (rc != STATUS_OK) {
		printf("Couldn't open device\n%s\n", OpenNI::getExtendedError());
		exit(2);
	}

	rc = device.setDepthColorSyncEnabled(true);
	if (rc != STATUS_OK) {
		printf("Couldn't enable depth and color images synchronization\n%s\n",
				OpenNI::getExtendedError());
		exit(2);
	}

	if (device.getSensorInfo(SENSOR_DEPTH) != NULL) {
		rc = depth.create(device, SENSOR_DEPTH);
		if (rc != STATUS_OK) {
			printf("Couldn't create depth stream\n%s\n",
					OpenNI::getExtendedError());
		}
	}

	if (device.getSensorInfo(SENSOR_COLOR) != NULL) {
		rc = color.create(device, SENSOR_COLOR);
		if (rc != STATUS_OK) {
			printf("Couldn't create color stream\n%s\n",
					OpenNI::getExtendedError());
		}
	}

	rc = depth.setMirroringEnabled(false);
	if (rc != STATUS_OK) {
		printf("Couldn't disable mirroring for depth stream\n%s\n",
				OpenNI::getExtendedError());
	}

	rc = color.setMirroringEnabled(false);
	if (rc != STATUS_OK) {
		printf("Couldn't disable mirroring for color stream\n%s\n",
				OpenNI::getExtendedError());
	}

	VideoMode depth_video_mode, color_video_mode;

	depth_video_mode.setFps(30);
	depth_video_mode.setPixelFormat(PIXEL_FORMAT_DEPTH_1_MM);
	depth_video_mode.setResolution(640, 480);

	color_video_mode.setFps(30);
	color_video_mode.setPixelFormat(PIXEL_FORMAT_YUV422);
	color_video_mode.setResolution(640, 480);

	rc = depth.setVideoMode(depth_video_mode);
	if (rc != STATUS_OK) {
		printf("Couldn't set depth video mode\n%s\n",
				OpenNI::getExtendedError());
	}

	rc = color.setVideoMode(color_video_mode);
	if (rc != STATUS_OK) {
		printf("Couldn't set color video mode\n%s\n",
				OpenNI::getExtendedError());
	}

	rc = depth.start();
	if (rc != STATUS_OK) {
		printf("Couldn't start the depth stream\n%s\n",
				OpenNI::getExtendedError());
	}

	rc = color.start();
	if (rc != STATUS_OK) {
		printf("Couldn't start the color stream\n%s\n",
				OpenNI::getExtendedError());
	}

	dc.reset(new FrameCallback(nh, "depth"));
	rgbc.reset(new FrameCallback(nh, "rgb"));

	// Register to new frame
	depth.addNewFrameListener(dc.get());
	color.addNewFrameListener(rgbc.get());

}

OpenNI2Camera::~OpenNI2Camera() {
	depth.stop();
	color.stop();
	depth.destroy();
	color.destroy();
	device.close();
	OpenNI::shutdown();
}