# invoke SourceDir generated makefile for empty_min.pem4f
empty_min.pem4f: .libraries,empty_min.pem4f
.libraries,empty_min.pem4f: package/cfg/empty_min_pem4f.xdl
	$(MAKE) -f /home/josepablocb/Workspace/VoltaRobotics/Tests/RAP_Robot/src/makefile.libs

clean::
	$(MAKE) -f /home/josepablocb/Workspace/VoltaRobotics/Tests/RAP_Robot/src/makefile.libs clean

