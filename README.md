# abb_librws

## ROS Distro Support

|         | Indigo | Jade | Kinetic |
|:-------:|:------:|:----:|:-------:|
| Branch  | [`master`](https://github.com/ros-industrial/abb_librws/tree/master) | [`master`](https://github.com/ros-industrial/abb_librws/tree/master) | [`master`](https://github.com/ros-industrial/abb_librws/tree/master) |
| Status  |  supported | supported |  supported |
| Version | [version](http://repositories.ros.org/status_page/ros_indigo_default.html?q=abb_librws) | [version](http://repositories.ros.org/status_page/ros_jade_default.html?q=abb_librws) | [version](http://repositories.ros.org/status_page/ros_kinetic_default.html?q=abb_librws) |

## Travis - Continuous Integration

Status: [![Build Status](https://travis-ci.org/ros-industrial/abb_librws.svg?branch=master)](https://travis-ci.org/ros-industrial/abb_librws)

## ROS Buildfarm

|         | Indigo Source | Indigo Debian | Jade Source | Jade Debian |  Kinetic Source  |  Kinetic Debian |
|:-------:|:-------------------:|:-------------------:|:-------------------:|:-------------------:|:-------------------:|:-------------------:|
| abb_librws | [![not released](http://build.ros.org/buildStatus/icon?job=Isrc_uT__abb_librws__ubuntu_trusty__source)](http://build.ros.org/view/Isrc_uT/job/Isrc_uT__abb_librws__ubuntu_trusty__source/) | [![not released](http://build.ros.org/buildStatus/icon?job=Ibin_uT64__abb_librws__ubuntu_trusty_amd64__binary)](http://build.ros.org/view/Ibin_uT64/job/Ibin_uT64__abb_librws__ubuntu_trusty_amd64__binary/) | [![not released](http://build.ros.org/buildStatus/icon?job=Jsrc_uT__abb_librws__ubuntu_trusty__source)](http://build.ros.org/view/Jsrc_uT/job/Jsrc_uT__abb_librws__ubuntu_trusty__source/) | [![not released](http://build.ros.org/buildStatus/icon?job=Jbin_uT64__abb_librws__ubuntu_trusty_amd64__binary)](http://build.ros.org/view/Jbin_uT64/job/Jbin_uT64__abb_librws__ubuntu_trusty_amd64__binary/) | [![not released](http://build.ros.org/buildStatus/icon?job=Ksrc_uX__abb_librws__ubuntu_xenial__source)](http://build.ros.org/view/Ksrc_uX/job/Ksrc_uX__abb_librws__ubuntu_xenial__source/) | [![not released](http://build.ros.org/buildStatus/icon?job=Kbin_uX64__abb_librws__ubuntu_xenial_amd64__binary)](http://build.ros.org/view/Kbin_uX64/job/Kbin_uX64__abb_librws__ubuntu_xenial_amd64__binary/) |

[![support level: vendor](https://img.shields.io/badge/support%20level-vendor-brightgreen.png)](http://rosindustrial.org/news/2016/10/7/better-supporting-a-growing-ros-industrial-software-platform)

## Overview

A C++ library for interfacing with ABB robot controllers supporting Robot Web Services (RWS). See the online [documentation](http://developercenter.robotstudio.com/webservice/api_reference) for a detailed description of RWS.

See [abb_librws](https://github.com/ros-industrial/abb_librws) for a companion library that interfaces with *Externally Guided Motion* (EGM).

### Sketch

The following is a conceptual sketch of how this RWS library can be viewed, in relation to an ABB robot controller as well as the EGM companion library mentioned above. 

![RWS sketch](docs/images/rws_sketch.svg)

### Requirements

* RobotWare version `6.0` or higher.

### Dependencies

* [POCO C++ Libraries](https://pocoproject.org) (`>= 1.4.3` due to WebSocket support)

### Limitations

RWS provides access to several services and resources in the robot controller, and this library currently support the following:

 * Reading/writing of IO-signals.
 * Reading/writing of RAPID data.
 * Reading of RAPID data properties.
 * Starting/stopping/resetting the RAPID program.
 * Subscriptions (i.e. receiving notifications when resources are updated).
 * Uploading/downloading/removing files.
 * Checking controller state (e.g. motors on/off, auto/manual mode and RAPID execution running/stopped).
 * Reading the joint/Cartesian values of a mechanical unit.
 * Register as a local/remote user (e.g. for interaction during manual mode).
 * Turning the motors on/off.
 * Reading of current RobotWare version and available tasks in the robot system.

### Recommendations

 * This library has been verified to work with RobotWare `6.06.01`. Other version are expected to work, but this cannot be guaranteed at the moment.
 * It is a good idea to perform RobotStudio simulations before working with a real robot.
 * It is prudent to familiarize oneself with general safety regulations (e.g. described in ABB manuals).
 
