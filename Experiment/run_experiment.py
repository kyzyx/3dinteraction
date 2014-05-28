import subprocess
import json

inputs = ['mousekbd', '3dmouse', 'leap']
outputs = ['2d', '3dHeadtracked']

name = raw_input("subject: ")

for outputType in outputs:
	for inputType in inputs:

		experiment = {
		'subject' : name,
		'input'   : inputType,
		'output'  : outputType,
		'scenes'  : 2
		}

		experimentConfigName = 'experiment_{}_{}_{}.json".format(name, inputType, outputType)
		experimentConfigFile = open(experimentConfigName, "w")
		experimentConfigFile.write(json.dumps(experiment))
		experimentConfigFile.close()

		print "Running experiment with config file {}".format(experimentConfigName)
		subprocess.call(["Debug/Experiment.exe", experimentConfigName])
