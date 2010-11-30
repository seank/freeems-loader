/*
 * FreeEMS_LoaderSREC.h
 *
 *  Created on: Nov 29, 2010
 *      Author: seank
 */

#ifndef FREEEMS_LOADERSREC_H_
#define FREEEMS_LOADERSREC_H_

class FreeEMS_LoaderSREC {
public:
	FreeEMS_LoaderSREC();
	virtual ~FreeEMS_LoaderSREC();
	bool verifyFile(int *file);
	bool verifyRecord(char *buffer);

};

#endif /* FREEEMS_LOADERSREC_H_ */
