#!/usr/bin/env python

import abb_librws as abb
import sys
import time
import rospy

rospy.init_node("swetree_cloud")
rws = abb.RWSInterface(sys.argv[1])
rmmp_state = abb.RMMPState()

if rws.isAutoMode().isFalse():
    print("The robot is not in Auto Mode, Requesting RMMP.")    
    rws.requestRMMP()   
    while rmmp_state.privilege != "modify":
        rws.getRMMPState(rmmp_state)
        rospy.sleep(1)           
    rospy.loginfo("Access granted")    
    for i in range(10):
        rospy.loginfo(i)
        rws.setRAPIDSymbolData("T_ROB1", "ABB_LIBRWS", "testNum", "123432")


# rospy.loginfo("{} started".format(rospy.get_name()))
rospy.spin()
