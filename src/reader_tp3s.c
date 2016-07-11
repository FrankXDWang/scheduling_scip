#include <assert.h>
#include <stdlib.h>

#include <scip/cons_setppc.h>

#include "reader_tp3s.h"

#include "json_read.h"
#include "data_structure.h"
#include "reader_tp3s.h"

#define READER_NAME			"tp3sreader"
#define READER_DESC			"file reader for tp3s problems"
#define READER_EXTENSION	"tp3s"

static 
SCIP_DECL_READERREAD(readerReadTP3S)
{
	int numTests;
	int numVehicles;

	/* buffers */
	TEST* tests;
	VEHICLE* vehicles;
	int** rehits;

	*result = SCIP_DIDNOTRUN;

	printf("data file path %s\n", filename);

	numTests = get_tests_size(filename);
	numVehicles = get_vehicle_size(filename);

	printf("num tests: %d, num vehicles: %d\n", numTests, numVehicles);

	/* allocate memory */
	tests = malloc(sizeof(TEST) * numTests);
	vehicles = malloc(sizeof(VEHICLE) * numVehicles);
	rehits = malloc(sizeof(int) * numTests);
	for (int i=0; i<numTests; i++)
	{
		rehits[i] = malloc(sizeof(int) * numTests);
	}

	read_in_tests(filename, tests);
	read_in_vehicles(filename, vehicles);
	read_in_rehit_rules(filename, rehits);


	/* free of mem */
	free(tests);
	free(vehicles);
	for (int i=0; i<numTests; i++)
	{
		free(rehits[i]);
	}
	free(rehits);


	*result = SCIP_SUCCESS;

	return SCIP_OKAY;
}

SCIP_RETCODE SCIPincludeReaderTP3S(
	SCIP*				scip)
{
	SCIP_READERDATA* readerData;
	SCIP_READER* reader;

	readerData = NULL;

	SCIP_CALL( SCIPincludeReaderBasic(scip, &reader, READER_NAME,
		READER_DESC, READER_EXTENSION, readerData));
	assert(reader != NULL);

	SCIP_CALL( SCIPsetReaderRead(scip, reader, readerReadTP3S));

	return SCIP_OKAY;
}