import subprocess
import json

from Tkinter import *
from ttk import *

class TopLevel (Frame):
	inputs = ['mousekbd', '3dmouse', 'leap']
	outputs = ['2d', '3dHeadtracked']

	def __init__ (self, parent):
		Frame.__init__(self, parent)
		self.parent = parent
		self.inputIdx = 0
		self.outputIdx = 0
		self.initUI()

	def initUI (self):
		self.parent.title("3D Interaction Experiments")
		self.style = Style()
		self.style.theme_use("default")

		#frame = Frame(self, relief=RAISED, borderwidth=1)
		#frame.pack(fill=BOTH, expand=1)
		line1 = Frame(self)
		line1.pack()
		self.subjectLabel = Label(line1, text="Subject")
		self.subjectLabel.pack(side=LEFT)
		self.subjectText = Text(line1, height=1)
		self.subjectText.pack(side=RIGHT)

		line2 = Frame(self)
		line2.pack()
		self.runsLabel = Label(line2, text="# of Scenes")
		self.runsLabel.pack(side=LEFT)
		self.runsText = Text(line2, height=1)
		self.runsText.insert(END, "2")
		self.runsText.pack(side=RIGHT)

		line3 = Frame(self, relief=RAISED, borderwidth=1)
		line3.pack(fill=BOTH)
		self.experimentLabel = Label(line3, text="{} / {}".format(self.outputs[self.outputIdx], self.inputs[self.inputIdx]))
		self.experimentLabel.pack(padx=5, pady=5)

		self.runButton = Button(self, text="Run Next", command=self.runClicked)
		self.runButton.pack(padx=5, pady=5)

		self.pack(fill=BOTH, expand=1)

	def runClicked (self):
		inputType = self.inputs[self.inputIdx]
		outputType = self.outputs[self.outputIdx]
		name = self.subjectText.get("0.0", END)
		runs = self.runsText.get("0.0", END)
		if len(name) > 1:
			if len(runs) > 0:
				runs = int(runs.split()[0])
			else:
				runs = 5
				print "Defaulting to 5 runs"
			name = name.split()[0]
			experiment = {
			'subject' : name,
			'input'   : inputType,
			'output'  : outputType,
			'scenes'  : runs
			}
			experimentConfigName = 'experiment_{}_{}_{}.json'.format(name, inputType, outputType)
			experimentConfigFile = open(experimentConfigName, "w")
			experimentConfigFile.write(json.dumps(experiment))
			experimentConfigFile.close()

			self.inputIdx += 1
			if self.inputIdx == len(self.inputs):
				self.inputIdx = 0
				self.outputIdx += 1
				if self.outputIdx == len(self.outputs):
					self.outputIdx = 0
			self.experimentLabel.config(text="{} / {}".format(self.outputs[self.outputIdx], self.inputs[self.inputIdx]))

			print "Running experiment with config file {}".format(experimentConfigName)
			subprocess.call(["Debug/Experiment.exe", experimentConfigName])



def main ():
	root = Tk()
	root.geometry("250x200")
	app = TopLevel(root)
	root.mainloop()

if __name__ == "__main__":
	main()

'''

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
'''