#!/usr/bin/env python

import abb_librws as abb
import sys
import time


class TRecord(abb.PyRAPIDRecord):
    def __init__(self):
        abb.PyRAPIDRecord.__init__(self, "TRecord")
        self.Value = abb.RAPIDNum()
        self.StringValue = abb.RAPIDString()
        self.addComponents(self.Value)
        self.addComponents(self.StringValue)


rws = abb.RWSInterface(sys.argv[1])
start = time.time()
ri = rws.collectRuntimeInfo()
print("Runtime info")
print("------------")
print("Auto mode: {}\nMotors on {}\nRapid running {}\nRws connected {}".format(ri.auto_mode, ri.motors_on, ri.rapid_running, ri.rws_connected))

si = rws.collectStaticInfo()
print("")
print("System Info")
print("-----------")
print("System name: {}".format(si.system_info.system_name))
print("System type: {}".format(si.system_info.system_type))
print("Robotware version: {}".format(si.system_info.robot_ware_version))
print("System options: {}".format(list(si.system_info.system_options)))
for rapid_task in si.rapid_tasks:
    print("Task: name {}".format(rapid_task.name))
print("")

arms = rws.getCFGArms()
for arm in arms:
    print("name: {}\tlower bound: {}\tupper_bound {}".format(arm.name, arm.lower_joint_bound, arm.upper_joint_bound))


mechanical_units = rws.getCFGMechanicalUnits()
mechanical_unit_groups = rws.getCFGMechanicalUnitGroups()
present_options = rws.getCFGPresentOptions()
robots = rws.getCFGRobots()
singles = rws.getCFGSingles()
transmissions = rws.getCFGTransmission()
# deprecated
# robotware_option_info = rws.getPresentRobotWareOptions()

mechanical_unit_static_info = abb.MechanicalUnitStaticInfo()
success = rws.getMechanicalUnitStaticInfo("ROB_1", mechanical_unit_static_info)
if success:
    print("")
    print("Mechanical Unit Static Info")
    print("----------------------------")
    print("Type: {}".format(mechanical_unit_static_info.type))
    print("Task name: {}".format(mechanical_unit_static_info.task_name))
    print("Axes: {}".format(mechanical_unit_static_info.axes))
    print("Axes total: {}".format(mechanical_unit_static_info.axes_total))
    print("is Integrated Unit: {}".format(mechanical_unit_static_info.is_integrated_unit))
    print("has Integrated Unit: {}".format(mechanical_unit_static_info.has_integrated_unit))
    print("")
    pass

mechanical_unit_dynamic_info = abb.MechanicalUnitDynamicInfo()
success = rws.getMechanicalUnitDynamicInfo("ROB_1", mechanical_unit_dynamic_info)
if success:
    print("")
    print("Mechanical Unit Dynamic Info")
    print("----------------------------")
    print("Tool name: {}".format(mechanical_unit_dynamic_info.tool_name))
    print("Wobj name: {}".format(mechanical_unit_dynamic_info.wobj_name))
    print("Payload name: {}".format(mechanical_unit_dynamic_info.payload_name))
    print("Total payload name: {}".format(mechanical_unit_dynamic_info.total_payload_name))
    print("Status: {}".format(mechanical_unit_dynamic_info.status))
    print("Mode: {}".format(mechanical_unit_dynamic_info.mode))
    print("Jog mode: {}".format(mechanical_unit_dynamic_info.jog_mode))
    print("Coordinate System: {}".format(mechanical_unit_dynamic_info.coord_system))
    print("")

jointtarget = abb.JointTarget()
success = rws.getMechanicalUnitJointTarget("ROB_1", jointtarget)
if success:
    print("Jointtarget is {}".format(jointtarget))

robtarget = abb.RobTarget()
success = rws.getMechanicalUnitRobTarget("ROB_1", robtarget)
if success:
    print("RobTarget is {}".format(robtarget))
    pass

# Iterate through all tasks and go through modules and print their info
rapid_tasks = rws.getRAPIDTasks();
for rapid_task in rapid_tasks:
    module_infos = rws.getRAPIDModulesInfo(rapid_task.name)
    for module_info in module_infos:
        print("Module: {}, type: {}".format(module_info.name, module_info.type))

speed_ratio = rws.getSpeedRatio()
print("Speed ratio: {}".format(speed_ratio))

logText = rws.getLogText()
print("log Text: {}".format(logText))

logTextverbose = rws.getLogText(True)
print("log Text Verbose: {}".format(logTextverbose))

# --------------------------
# tests for data types
# --------------------------
#bool
testBool = abb.RAPIDBool()
success = rws.getRAPIDSymbolData("T_ROB1", "ABB_LIBRWS", "testBool", testBool)
if success:
    print("Value of testBool is: {}".format(testBool))
#num
testNum = abb.RAPIDNum()
success = rws.getRAPIDSymbolData("T_ROB1", "ABB_LIBRWS", "testNum", testNum)
if success:
    print("Value of testNum is: {}".format(testNum))
#string
testString = abb.RAPIDString()
success = rws.getRAPIDSymbolData("T_ROB1", "ABB_LIBRWS", "testString", testString)
if success:
    print("Value of testString is: {}".format(testString))
#RobJoint
testRobJoint = abb.RobJoint()
success = rws.getRAPIDSymbolData("T_ROB1", "ABB_LIBRWS", "testRobJoint", testRobJoint)
if success:
    print("Value of testRobJoint is: {}".format(testRobJoint))
#ExtJoint
testExtJoint = abb.ExtJoint()
success = rws.getRAPIDSymbolData("T_ROB1", "ABB_LIBRWS", "testExtJoint", testExtJoint)
if success:
    print("Value of testNum is: {}".format(testExtJoint))
#JointTarget
testJointTarget = abb.JointTarget()
success = rws.getRAPIDSymbolData("T_ROB1", "ABB_LIBRWS", "testJointTarget", testJointTarget)
if success:
    print("Value of testJointTarget is: {}".format(testJointTarget))
#Pos
testPos = abb.Pos()
success = rws.getRAPIDSymbolData("T_ROB1", "ABB_LIBRWS", "testPos", testPos)
if success:
    print("Value of testPos is: {}".format(testPos))
#Orient
testOrient = abb.Orient()
success = rws.getRAPIDSymbolData("T_ROB1", "ABB_LIBRWS", "testOrient", testOrient)
if success:
    print("Value of testOrient is: {}".format(testOrient))
#Pose
testPose = abb.Pose()
success = rws.getRAPIDSymbolData("T_ROB1", "ABB_LIBRWS", "testPose", testPose)
if success:
    print("Value of testPose is: {}".format(testPose))
#ConfData
testConfData = abb.ConfData()
success = rws.getRAPIDSymbolData("T_ROB1", "ABB_LIBRWS", "testConfData", testConfData)
if success:
    print("Value of testConfData is: {}".format(testConfData))
#RobTarget
testRobTarget = abb.RobTarget()
success = rws.getRAPIDSymbolData("T_ROB1", "ABB_LIBRWS", "testRobTarget", testRobTarget)
if success:
    print("Value of testRobTarget is: {}".format(testRobTarget))
#ToolData
testToolData = abb.ToolData()
success = rws.getRAPIDSymbolData("T_ROB1", "ABB_LIBRWS", "testToolData", testToolData)
if success:
    print("Value of testToolData is: {}".format(testToolData))
#WObjData
testWObjData = abb.WObjData()
success = rws.getRAPIDSymbolData("T_ROB1", "ABB_LIBRWS", "testWObjData", testWObjData)
if success:
    print("Value of testWObjData is: {}".format(testWObjData))
#SpeedData
testSpeedData = abb.SpeedData()
success = rws.getRAPIDSymbolData("T_ROB1", "ABB_LIBRWS", "testSpeedData", testSpeedData)
if success:
    print("Value of testSpeedData is: {}".format(testSpeedData))
#LoadData
testLoadData = abb.LoadData()
success = rws.getRAPIDSymbolData("T_ROB1", "ABB_LIBRWS", "testLoadData", testLoadData)
if success:
    print("Value of testLoadData is: {}".format(testLoadData))
#CustomRecord
testRecord = rws.getRAPIDSymbolData("T_ROB1", "ABB_LIBRWS", "testRecord")
print("Value of testRecord is: {}".format(testRecord))

record = TRecord()
record.parseString(testRecord)
print("Value of record is: {}".format(record))

print("Total time is {}".format(time.time() - start))
