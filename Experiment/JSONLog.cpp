#include <Windows.h>
#include <sstream>
#include <iomanip>
#include <direct.h>

#include "JSONLog.h"
#include "timestamp.h"

static const char *LOG_DIR = "log";
static const char *DAT_DIR = "log\\dat";

JSONLog::JSONLog(std::string basename)
	: m_basename(basename),
	m_sceneCounter(0),
	m_inputLastTime(0.0),
	m_inputSavePeriod(0.03333) // 30Hz
{
	// Create directories to store logs and raw data files
	CreateDirectory(LOG_DIR, NULL);
	CreateDirectory(DAT_DIR, NULL);
	std::stringstream filename;
	filename << LOG_DIR << "\\" << basename << ".json";
	m_logFile = std::ofstream(filename.str());	
}

JSONLog::~JSONLog(void) {
	m_logFile << json::Serialize(m_object);
	m_logFile.close();
}

void JSONLog::startExperiment(json::Value config) {
	// Create a copy of the config that specified this experiment
	m_object["config"] = config;
	m_object["scenes"] = json::Array();
}

void JSONLog::endExperiment() {
}

// Convenience
json::Object input2json (const InputStatus &input) {
	json::Object pose;
	pose["px"] = input.pos.x();
	pose["py"] = input.pos.y();
	pose["pz"] = input.pos.z();
	pose["rw"] = input.rot.w();
	pose["rx"] = input.rot.x();
	pose["ry"] = input.rot.y();
	pose["rz"] = input.rot.z();
	return pose;
}

void JSONLog::startScene(const InputStatus &start, const InputStatus &end) {
	// Create the binary file for saving input data
	std::stringstream sceneFile;
	sceneFile 
		<< DAT_DIR << "\\" << m_basename << "_scene" 
		<< std::setw(3) << std::setfill('0') << m_sceneCounter << std::setfill(' ')
		<< ".dat";
	m_datFile = std::ofstream(sceneFile.str(), std::ofstream::binary);
	m_sceneStartTime = timestamp();
	// Set up the JSON object to represent this scene
	json::Object sceneObj;
	sceneObj["seq"] = m_sceneCounter;
	sceneObj["startPos"] = input2json(start);
	sceneObj["endPos"] = input2json(end);
	sceneObj["startTime"] = m_sceneStartTime;
	sceneObj["inputs"] = sceneFile.str();
	m_object["scenes"].ToArray().push_back(sceneObj);

	// flush previously stored values from a previous scene
	m_inputs.clear();
}

void JSONLog::endScene() {
	getSceneObj()["endTime"] = timestamp();
	getSceneObj()["numInputs"] = (int)m_inputs.size();
	++m_sceneCounter;

	// Write binary data to disk
	int rawSize = m_inputs.size() * std::tuple_size<RawInput>::value * sizeof(RawInput::value_type);
	if (rawSize > 0) {
		RawInput::value_type *rawStart = (RawInput::value_type*)&(m_inputs.front()[0]);
		m_datFile.write((char*)rawStart, rawSize);
	}
	m_datFile.close();
}

void JSONLog::logInput(InputStatus &input) {
	double diff = input.timestamp - m_inputLastTime;
	if (diff < m_inputSavePeriod) {
		return;
	} else {
		m_inputLastTime = input.timestamp;
	}
	RawInput raw;
	raw[0] = input.timestamp - m_sceneStartTime;
	raw[1] = input.pos.x();
	raw[2] = input.pos.y();
	raw[3] = input.pos.z();
	raw[4] = input.rot.w();
	raw[5] = input.rot.x();
	raw[6] = input.rot.y();
	raw[7] = input.rot.z();
	raw[8] = *((RawInput::value_type*)&input.flags); // interpret as float
	m_inputs.push_back(raw);
	return;
}

json::Object& JSONLog::getSceneObj (void) {
	return m_object["scenes"].ToArray()[m_sceneCounter].ToObject();
}
;