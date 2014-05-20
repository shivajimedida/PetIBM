#include "FlowDescription.h"
#include "CartesianMesh.h"
#include "SimulationParameters.h"
#include "createSolver.h"
#include <petscsys.h>
#include <string>
#include <memory>

int main(int argc,char **argv)
{
	PetscErrorCode ierr;
	const PetscInt dim = 2;
	char           caseFolder[PETSC_MAX_PATH_LEN];
	
	ierr = PetscInitialize(&argc, &argv, NULL, NULL); CHKERRQ(ierr);
	
	ierr = PetscOptionsGetString(NULL, "-caseFolder", caseFolder, sizeof(caseFolder), NULL); CHKERRQ(ierr);
	
	std::string             folder(caseFolder);
	FlowDescription         FD(folder+"/flowDescription.yaml");
	CartesianMesh           CM(folder+"/cartesianMesh.yaml");
	SimulationParameters    SP(folder+"/simulationParameters.yaml");
	
	std::unique_ptr< NavierStokesSolver<dim> > solver = createSolver<dim>(&FD, &SP, &CM);
	
	solver->initialise();
	
	while(!solver->finished())
	{
		solver->stepTime();
	}
	solver->writeData();
	solver->finalise();

	ierr = PetscFinalize(); CHKERRQ(ierr);
	return 0;
}