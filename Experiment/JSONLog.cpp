#include <sstream>
#include <direct.h>

#include "JSONLog.h"
#include "timestamp.h"

JSONLog::JSONLog(std::string filename) : m_logFile(filename), m_sceneCounter(0) {
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

void JSONLog::startScene() {
	json::Object sceneObj;
	sceneObj["seq"] = m_sceneCounter;

	json::Object startPos;
	startPos["x"] = NULL;
	startPos["y"] = NULL;
	sceneObj["startPos"] = startPos; // TODO
	json::Object endPos;
	endPos["x"] = NULL;
	endPos["y"] = NULL;
	sceneObj["endPos"] = endPos; // TODO
	sceneObj["startTime"] = timestamp();
	sceneObj["inputs"] = json::Array();
	inputs.clear();
	m_object["scenes"].ToArray().push_back(sceneObj);
}

void JSONLog::endScene() {
	getSceneObj()["endTime"] = timestamp();
	++m_sceneCounter;
}

void JSONLog::logInput(InputStatus &input) {
	size_t rawIdx = 0;
	RawInput raw;
	raw[rawIdx++] = input.timestamp;
	raw[rawIdx++] = input.pos.x();
	raw[rawIdx++] = input.pos.y();
	raw[rawIdx++] = input.pos.z();
	raw[rawIdx++] = input.rot.w();
	raw[rawIdx++] = input.rot.x();
	raw[rawIdx++] = input.rot.y();
	raw[rawIdx++] = input.rot.z();
	raw[rawIdx++] = *((float*)&input.flags); // interpret as float
	inputs.push_back(raw);
	
	//getSceneObj()["inputs"].ToArray().push_back(inputObj);
}

json::Object& JSONLog::getSceneObj (void) {
	return m_object["scenes"].ToArray()[m_sceneCounter].ToObject();
}
