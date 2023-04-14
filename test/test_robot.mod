MODULE ABB_LIBRWS
    RECORD tRecord
        Num Value;
        String stringValue;
    ENDRECORD
   
    TASK PERS bool testBool := TRUE;
    TASK PERS num testNum := 123;
    TASK PERS string testString := "Hello World";
    TASK PERS robJoint testRobJoint := [1,2,3,4,5,6];
    TASK PERS ExtJoint testExtJoint := [1,2,3,4,5,6];
    TASK PERS JointTarget testJointTarget := [[1,2,3,4,5,6],[1,2,3,4,5,6]];
    TASK PERS Pos testPos :=[1,2,3];
    TASK PERS Orient testOrient := [1,0,0,0];
    TASK PERS Pose testPose :=[[1,2,3],[1,0,0,0]];
    TASK PERS ConfData testConfData := [1,2,3,4];
    TASK PERS RobTarget testRobTarget := [[1,2,3],[1,0,0,0],[0,0,0,0],[0,0,0,0,0,0]];
    TASK PERS ToolData testToolData := [TRUE, [[0, 0, 0], [1, 0, 0, 0]], [0.001, [0, 0, 0.001],[1, 0, 0, 0], 0, 0, 0]];
    TASK PERS WobjData testWobjData := [FALSE, TRUE, "", [[1, 2, 3],[1, 0, 0, 0]],[[0, 0, 0],[1, 0, 0, 0]]];
    TASK PERS SpeedData testSpeedData := [1,0,0,0];
    TASK PERS LoadData testLoadData := [0.001, [0, 0, 0.001],[1, 0, 0, 0], 0, 0, 0];
   
    TASK PERS tRecord testREcord := [1, "Hello World"];
ENDMODULE