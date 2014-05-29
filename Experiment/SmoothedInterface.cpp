#include "SmoothedInterface.h"

SmoothedInterface::SmoothedInterface(InputInterface* inputinterface, Filter<Eigen::Vector3d>* filter) 
: input(inputinterface), f(filter)
{	
	f->reset();
}
SmoothedInterface::~SmoothedInterface(void) {
	delete input;
	delete f;
}

InputStatus SmoothedInterface::_getStatus(void) {
	status = input->getStatus();
	f->update(status.pos);
	status.pos = f->get();
	status.rot = status.rot;
	return status;
}
