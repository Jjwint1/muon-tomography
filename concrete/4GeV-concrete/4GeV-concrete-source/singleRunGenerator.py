
for i in range(500000):
    file = open("singleRunMacro.mac", "a");
    file.write("/run/beamOn 1\n");
    file.close();

